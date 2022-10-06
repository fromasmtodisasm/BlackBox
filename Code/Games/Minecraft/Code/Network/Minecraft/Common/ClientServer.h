#include <vector>
#include <thread>
#include <mutex>
#include <sstream>
#include <memory>
#include <string_view>
#include <functional>

#include <Network/Minecraft/Common/Socket.h>

namespace network
{
	enum class Server
	{
		MESSAGE,
		NAME,
		JOIN,
		CONNECT,
		CONNECTED,
		DISCONNECT
	};
	enum class Client
	{
		AUTH,
		MESSAGE,
	};
}

struct BasicStream
{
	using DataType = char;
	using SizeType = size_t;

	DataType* data;
	SizeType len;
	SizeType pointer = 0;
};

#include <type_traits>

struct Stream : BasicStream
{
	Stream(char* data, size_t len) : BasicStream{ data,len }
	{

	}

	template<class T>
	T ConstructFromDataAndSize(DataType* data, SizeType size);

	template<>
	std::string_view ConstructFromDataAndSize<std::string_view>(DataType* data, SizeType size)
	{
		return std::string_view(&data[pointer], size);
	}

	template<>
	std::string ConstructFromDataAndSize<std::string>(DataType* data, SizeType size)
	{
		return std::string(&data[pointer], size);
	}

	template<class T>
	bool Read(T& val)
	{
		using pointer_type = T*;
		using value_type = T;
		if ((pointer + sizeof(value_type)) <= len)
		{
			val = *(pointer_type(&data[pointer]));
			pointer += sizeof(value_type);
			return true;
		}
		return false;

	}

	template<class T>
	bool ReadStr(T& val)
	{
		using value_type = T::value_type;
		using size_type = T::size_type;
		{
			size_type size;
			Read(size);
			val = ConstructFromDataAndSize<T>(data, size);
			pointer += val.length();
			return true;
		}
		return false;
	}

	template<>
	bool Read(std::string_view& val)
	{
		return ReadStr(val);
	}

	template<>
	bool Read(std::string& val)
	{
		return ReadStr(val);
	}



};

struct Writer
{
	size_t len = 0;
	std::vector<char> data;
	size_t pointer = 0;

	Writer(size_t len) : len(len)
	{
		data.resize(len);
	}

	template<class VAL_TYPE>
	bool Write(VAL_TYPE& val)
	{
		using pointer_type = VAL_TYPE*;
		using value_type = VAL_TYPE;
		if ((pointer + sizeof(value_type)) <= len)
		{
			*(pointer_type(&data[pointer])) = val;
			pointer += sizeof(value_type);
			return true;
		}
		return false;

	}

	template<>
	bool Write<std::string_view>(std::string_view& val)
	{
		using pointer_type = std::string_view::pointer;
		using value_type = std::string_view::value_type;
		if ((pointer + val.length()) <= len)
		{
			auto size = val.length();
			Write(size);
			strcpy(&data[pointer], val.data());
			pointer += val.length();//+1 for \0
			return true;
		}
		return false;

	}
};


namespace network
{
	bool InitSockets();
	void ShutdownSockets();
	struct System
	{
		System()
		{
			IsReady = InitSockets();
		}
		~System()
		{
			if (IsReady)
				ShutdownSockets();
		}
		bool IsReady = false;
	};
}

struct User
{
	std::string name;
	std::string mail;
	std::string password;
};
