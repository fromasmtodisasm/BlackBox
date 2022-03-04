#pragma once
#include <algorithm>
#include <memory>

struct SFileVersion
{
	SFileVersion() = default;
	SFileVersion(byte major, byte minor, byte patch)
	    : version(0)
	{
		version = major << 16 | minor << 8 | patch;
	}

public:
	std::uint32_t version;
};

struct MemoryArena
{
	static constexpr std::uint32_t _size           = 10 * 0xffff;
	static constexpr float         _growing_factor = 1.6f;
	size_t                         _offset{};
	std::vector<char>              _data{};
	MemoryArena()
	{
		_data.resize(this->_size);
	}

	void* alloc(size_t size)
	{
		//assert(_offset + size < this->_size && "Not enough memory in arena!!!");

		bool enough_mem = (_offset + size) < this->_size;
		if (!enough_mem)
		{
			_data.resize(size_t(_data.size() * _growing_factor));
		}

		auto result = &_data[_offset];
		_offset += size;

		return result;
	}
	template<class T>
	T* alloc(size_t size)
	{
		return reinterpret_cast<T*>(alloc(size));
	}

	size_t size() { return _offset; }
	void*  data() { return &_data[0]; }
};

struct SVariableString
{
	std::uint16_t size;
	char          data[1];

	SVariableString(std::uint16_t size, char* data)
	    : size(size)
	{
		strcpy((this->data), data);
	}
};

struct SDirEntry
{
	std::int32_t    offset;
	std::uint32_t   size;
	char            test[256]{0};
	SVariableString file_name;

	SDirEntry(std::string_view file_name, std::int32_t offset, std::uint32_t size)
	    : file_name(std::uint16_t(file_name.length()), (char*)file_name.data())
	    , offset(offset)
	    , size(size)
	{
	}

	string_view Name()
	{
		return string_view{file_name.data, file_name.size};
	}

	static MemoryArena arena;

	static SDirEntry*  alloc(size_t size)
	{
		return (SDirEntry*)arena.alloc(size);
	}
};

struct SArchive
{
	std::uint32_t magic;
	SFileVersion  version;
	std::uint32_t number_of_files;
	std::uint32_t toc_offset;
};

struct SArchiveHandle
{
	CFileMapping fm;
	SArchive*    header{};
	SArchiveHandle(std::string_view file)
	    : fm(file.data())
	{
		header = (SArchive*)fm.getData();
	}

	//SArchiveHandle(SArchiveHandle&& other)
	//{
	//}

	operator SArchive&()
	{
		return *header;
	}

	operator bool()
	{
		return header != nullptr;
	}

	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = SDirEntry;
		using pointer           = value_type*;
		using reference         = value_type&;

		Iterator(pointer ptr)
		    : m_ptr(ptr)
		{
		}
		reference operator*() const { return *m_ptr; }
		pointer   operator->() { return m_ptr; }

		// Prefix increment
		Iterator& operator++()
		{
			m_ptr = (SDirEntry*)((byte*)m_ptr->file_name.data + m_ptr->file_name.size + 1);
			return *this;
		}

		// Postfix increment
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
		/////////////////////////////////////////////

	private:
		pointer m_ptr;
	};

	Iterator begin() { return Iterator(reinterpret_cast<SDirEntry*>((byte*)header + header->toc_offset)); }
	Iterator end() { return Iterator((SDirEntry*)((byte*)header + fm.getSize())); }
};

std::string_view remove_leading_ups(std::string_view str);
void             write_archive_recursive(SArchive& ar, const std::string& file, std::ofstream& of);
SArchive         write_archive(const std::string& pattern, const std::string out_file);
SArchiveHandle   archive_open(std::string_view file);
void             list(const string& file);
void             create_file(SArchive& ar, fs::path filename, std::int32_t offset, std::uint32_t size);
void             extract(const string& file, const string& base, const string& pattern);
int              config_handler(void* user, const char* section, const char* name, const char* value);
void             parse_cmd(int argc, char* argv[]);

template<typename F>
void foreach (SArchive& ar, F func)
{
	auto* entry = reinterpret_cast<SDirEntry*>((byte*)&ar + ar.toc_offset);
	for (size_t i = 0; i < ar.number_of_files; i++)
	{
		func(ar, entry);
		entry = (SDirEntry*)((byte*)entry->file_name.data + entry->file_name.size + 1);
	}
}

#include <unordered_map>

struct File
{
	std::int32_t  offset;
	std::uint32_t size;
	string_view   name;
	char*         base;
};

struct MyFile
{
	File*        m_File;
	std::int32_t pointer{};

	//
public:
	void Read(void* dst, size_t size)
	{
		size_t left = m_File->size - pointer;
		if (left > 0)
		{
			left = std::min(size_t(left), size);
			auto offset = m_File->base + m_File->offset + pointer;
			memcpy(dst, offset, left);
		}
	}

	size_t Size()
	{
		return m_File->size;
	}
};

class CPak
{
public:
	///////////////////
	using KeyType =
#if 0
	    string;
#else
	    string_view;
#endif
	    using FileList = std::unordered_map<KeyType, File>;

	bool OpenPak(string_view pak)
	{
		SArchiveHandle ar{pak};
		if (!ar) return false;

		for (auto& entry : ar)
		{
			File file{.offset = entry.offset, .size = entry.size, .name = entry.Name(), .base = (char*)ar.header};

			if (auto it = m_Files.find(file.name); it != m_Files.end())
			{
				printf("Eroror, file %s already mapped\n", file.name.data());
			}

			m_Files[file.name] = file;
		}
		m_Archives.emplace_back(std::move(ar));
		return true;
	}

	std::shared_ptr<MyFile> FOpen(string_view fname)
	{
		if (auto it = m_Files.find(fname); it != m_Files.end())
		{
			return std::make_shared<MyFile>(&it->second);
		}

		printf("Eroror, file %s already mapped\n", fname.data());
		return nullptr;
	}

	void Dump()
	{
		for (auto& f : m_Files)
		{
			printf("%s\n", f.first.data());
		}
	}

public:
	///////////////////
	FileList                    m_Files;
	std::vector<SArchiveHandle> m_Archives;
};
