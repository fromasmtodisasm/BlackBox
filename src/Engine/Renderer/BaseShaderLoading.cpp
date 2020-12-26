#include "Shaders/FxParser.h"
#include <BlackBox/Renderer/Shader.hpp>

#include <sstream>
using std::string_view;
using std::stringstream;

namespace
{
	bool match(std::string_view input, std::string_view with, size_t& offset)
	{
		if ((offset + with.size()) > input.size())
			return false;
		if (input.substr(offset, with.size()) == with)
		{
			offset += with.size();
			return true;
		}
		return false;
	}

	void skip_spaces(string_view buffer, size_t& begin)
	{
		size_t pos = begin;
		if ((pos = buffer.find_first_of(begin, ' ')) != buffer.npos)
		{
			begin = buffer.find_first_not_of(begin, ' ');
		}
	}

	std::string_view get_attr_name(string_view buffer, string_view attr, size_t begin, size_t end)
	{
		if (match(buffer, attr, begin))
		{
			//begin += attr.length();
			if (match(buffer, ",", begin))
			{
				if ((begin = buffer.find_first_of(' ', begin - 1)) != buffer.npos)
				{
					begin++;
					if ((end = buffer.find_first_of(')', begin)) != buffer.npos)
					{
						string_view attr_name(buffer.substr(begin, end - begin));
						return attr_name;
					}
				}
			}
		}
		return string_view();
	}
} // namespace

CShader* CShader::load(ShaderDesc const& desc)
{
	string text;
	auto path = desc.root + desc.name;
	gEnv->pLog->Log("Loading %s shader", path.data());
	if (!loadInternal(path, text))
		return nullptr;

	if (desc.macro.size() > 0)
	{
		auto pos = text.find("#version");
		if (pos != string::npos)
		{
			auto end = text.find_first_of('\n', pos + 1);
			string defines;
			for (auto& define : desc.macro)
			{
				defines = "#define " + define.first + " " + define.second + "\n";
			}
			text.insert(end + 1, defines);
		}
	}

	auto shader = new CShader(desc.type);
	if (shader->Create())
	{
		shader->Compile({"", text});
		shader->m_Empty = false;
		debuger::shader_label(shader->get(), path);
	}
	return shader;
}

CShader* CShader::load(std::string_view source)
{
	return nullptr;
}

bool CShader::parseLine(string& buffer)
{
	size_t pos = 0;
	if ((pos = buffer.find("#include")) != buffer.npos)
	{
		size_t begin, end;
		if ((begin = buffer.find_first_of('<')) != buffer.npos)
			end = buffer.find_first_of('>');
		else if ((begin = buffer.find_first_of('"')) != buffer.npos)
			end = buffer.find('"', begin + 1);
		else
			return false;

		string file(buffer.substr(begin + 1, end - begin - 1));
		string buff;

		if (!loadInternal(string("res/shaders/") + file, buff))
			return false;
		buffer.clear();
		buffer += buff;
	}
	else if ((pos = buffer.find("#version")) != buffer.npos)
	{
		buffer = "#version 330";
	}
	else if ((pos = buffer.find("#pragma")) != buffer.npos)
	{
		size_t begin = pos, end = buffer.size();
		if ((begin = buffer.find_first_of(' ')) != buffer.npos)
		{
			begin++;
			skip_spaces(buffer, begin);
			if (buffer.substr(begin, std::strlen("attribute")) == "attribute")
			{
				begin += sizeof("attribute") - 1;
				if (buffer[begin] == '(')
				{
					begin++;
					stringstream ss;
					bool status = false;
					if (auto name = get_attr_name(buffer, "position", begin, end); name.size() != 0)
					{
						ss << "POSITION_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					else if (auto name = get_attr_name(buffer, "tc", begin, end); name.size() != 0)
					{
						ss << "TC_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					else if (auto name = get_attr_name(buffer, "normal", begin, end); name.size() != 0)
					{
						ss << "NORMAL_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					else if (auto name = get_attr_name(buffer, "color", begin, end); name.size() != 0)
					{
						ss << "COLOR_ATTR(" << name << ");";
						buffer = ss.str();
						status = true;
					}
					return status;
				}
			}
		}
		return false;
	}

	return true;
}

string grab_line(string& buffer)
{
	if (CShader::parseLine(buffer))
	{
		return buffer + '\n';
	}
	return string();
}

bool CShader::loadInternal(string const& path, string& buffer)
{
	std::ifstream fin(path);
	string buff;

	if (!fin.is_open())
		return false;

	while (getline(fin, buff))
	{
		buffer += grab_line(buff);
	}
	fin.close();
	return true;
}

bool CShader::loadFromStream(const std::stringstream stream, string& buffer)
{
	return false;
}
ShaderRef CShader::loadFromEffect(IEffect* pEffect, CShader::Type type)
{
	auto tech = pEffect->GetTechnique(0);
	auto pass = tech->GetPass(0);
	auto code = pass->CommonCode;

	if (type == IShader::E_VERTEX)
	{
		for (auto& in : pass->InputLayout)
			code.push_back(in);
	}
	code.push_back(pass->Shaders[type]);
	return CShader::loadFromMemory(code, type);
}

ShaderRef CShader::loadFromMemory(const std::vector<std::string_view>& text, IShader::Type type)
{
	auto shader = ShaderRef(new CShader(type));
	if (!shader->Create())
		return nullptr;
	shader->Compile(text);
	return shader;
}

IShaderProgram* GetGLSLSandBoxShader()
{
	return nullptr;
}


namespace GlslProgram
{
	struct Data
	{
		void* ptr = nullptr;
		uint length = 0;
		const char* tag = nullptr;
		Data(void* ptr, uint length) : ptr(ptr), length(length) {}
		Data(const char* t) : tag(t) {}

		void saveToFile(const char* name)
		{
			std::ofstream fo(name, std::ios::binary);	
			if (fo.is_open())
			{
				fo.write((char*)ptr, length);
			}
		}
	};
	bool getLinkStatus(GLuint program)
	{
		GLint linked;

		glGetProgramiv(program, GL_OBJECT_LINK_STATUS_ARB, &linked);

		if (!linked)
			return false;

		return true;
	}

	Data* getBinary(GLuint program, GLenum* binaryFormat)
	{
		if (program == 0 || !getLinkStatus(program))
			return NULL;

		if (glGetProgramBinary == NULL || glProgramBinary == NULL || glProgramParameteri == NULL)
			return NULL;

		GLint binaryLength;
		void* binary;

		glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &binaryLength);

		if (binaryLength < 1)
			return NULL;

		binary = (GLvoid*)malloc(binaryLength);

		if (binary == NULL)
			return NULL;

		glGetProgramBinary(program, binaryLength, NULL, binaryFormat, binary);

		return new Data(binary, binaryLength);
	}
	bool saveBinary(GLuint program, const char* fileName, GLenum* binaryFormat)
	{
		Data* data = getBinary(program, binaryFormat);

		if (data == NULL)
			return false;

		data->saveToFile(fileName);

		delete data;

		return true;
	}

	bool loadBinary(GLuint& program, Data* data, GLenum binaryFormat)
	{
		if (program == 0)
			return false;

		if (glGetProgramBinary == NULL || glProgramBinary == NULL || glProgramParameteri == NULL)
			return false;

		if (data == NULL || data->length < 1)
			return false;

		glProgramBinary(program, binaryFormat, data->ptr, data->length);

		return getLinkStatus(program);
	}

	IShaderProgram* loadBinary(const char* fileName, GLenum binaryFormat)
	{
		Data* data = new Data(fileName);
		uint binary;
		CShaderProgram* program{};
		if (bool res   = loadBinary(binary, data, binaryFormat); res)
		{
			program = new CShaderProgram(binary);	
		}

		delete data;

		return program;
	}
} // namespace GlslProgram

template<typename T>
std::ostream& binary_write(std::ostream& stream, const T& value)
{
	return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template<typename T>
std::istream& binary_read(std::istream& stream, T& value)
{
	return stream.read(reinterpret_cast<char*> (&value), sizeof(T));
}

bool SaveBinaryShader(const char* name, IShaderProgram* program, int flags){
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	if (formats < 1)
	{
		CryError("Driver does not support any binary formats.");
		return false;
	}
	// Get the binary length
	GLint length = 0;
	glGetProgramiv(program->Get(), GL_PROGRAM_BINARY_LENGTH, &length);

	// Retrieve the binary code
	std::vector<GLubyte> buffer(length);
	GLenum format = 0;
	glGetProgramBinary(program->Get(), length, NULL, &format, buffer.data());

	// Write the binary to a file.
	std::stringstream path;
	path << "bin/shadercache/" << name << ".fxb";
	CryLog("Writing to %s , binary format = %d", path.str().c_str(), format);
	std::ofstream out(path.str().c_str(), std::ios::binary);
	out.write(reinterpret_cast<char*>(&format), sizeof(format));
	out.write(reinterpret_cast<char*>(&length), sizeof(length));
	out.write(reinterpret_cast<char*>(buffer.data()), length);
	out.close();
	return true;
}
IShaderProgram* LoadBinaryShader(const char* name, int flags)
{
	//GLuint program = glCreateProgram();
	CShaderProgram* program = new CShaderProgram();

	ShaderProgramStatus st(program);

	// Load binary from file
	std::ifstream inputStream(name, std::ios::binary);
	if (!inputStream.is_open())
	{
		return nullptr;
	}
	std::istreambuf_iterator<char> startIt(inputStream), endIt;

	GLenum format = 0;
	uint length	  = 0;
	binary_read(inputStream, format);
	binary_read(inputStream, length);
	CryLog("Loading binary shader %s , binary format = %d, binary length = %d", name, format, length);
	std::vector<char> buffer(startIt, endIt); // Load file
	inputStream.close();

	// Install shader binary
	glProgramBinary(program->Get(), format, buffer.data(), length);

	// Check for success/failure
	#if 0
	GLint status;
	glGetProgramiv(program->Get(), GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		CryError("Cannot link binary program.");
		delete program;
		program = nullptr;
	}
	#endif
	st.get(GL_LINK_STATUS);
	return program;
}
