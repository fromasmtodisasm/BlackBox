#include "Shaders/FxParser.h"
#include <BlackBox/Renderer/Shader.hpp>
#if BB_PLATFORM_WINDOWS
#endif

#include <sstream>
using std::string_view;
using std::stringstream;

inline std::string GetBinaryPath(const char* name, uint64 mask)
{
	std::stringstream path;
	path << "bin/shadercache/" << name << "(" << mask << ").fxb";
	return path.str();
}

auto GetSpirvEntry(ShaderLangId id, IShader::Type type)
{
	//return /*id == ShaderLangId::Glsl ? "main" : */type == CShader::E_VERTEX ? "VSMain" : CShader::E_FRAGMENT ? "PSMain" : "GSMain";
	return "main";
}

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

CShader* CShader::Load(ShaderDesc const& desc)
{
	string text;
	auto path = desc.root + desc.name;
	gEnv->pLog->Log("Loading %s shader", path.data());
	if (!LoadInternal(path, text))
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
		debuger::shader_label(shader->Get(), path);
	}
	return shader;
}

CShader* CShader::Load(std::string source)
{
	return nullptr;
}

bool CShader::ParseLine(string& buffer)
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

		if (!LoadInternal(string("res/shaders/") + file, buff))
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
	if (CShader::ParseLine(buffer))
	{
		return buffer + '\n';
	}
	return string();
}

bool CShader::LoadInternal(string const& path, string& buffer)
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

bool CShader::LoadFromStream(const std::stringstream stream, string& buffer)
{
	return false;
}

const char* GetGLSLANGTargetName(IShader::Type target)
{
	switch (target)
	{
	case IShader::E_VERTEX:
		return "vert";
		
	case IShader::E_TESSELATION_CONTROL:
		return "tesc";
		
	case IShader::E_TESSELATION_EVALUATION:
		return "tese";
		
	case IShader::E_GEOMETRY:
		return "geom";
		
	case IShader::E_FRAGMENT:
		return "frag";
		
	case IShader::E_COMPUTE:
		return "comp";
	default:
		return "1111";
	}

} 

void DumpWarning(const char* str)
{
	CryLogAlways("[Vk Compiler] Warning: %s", str);
	fwrite(str, sizeof(char), strlen(str), stderr);
}

void DumpError(const char* str)
{
	CRY_ASSERT(false, "[Vk Compiler] ERROR: %s", str);
	CryLogAlways("[Vk Compiler] ERROR: %s", str);
	fwrite(str, sizeof(char), strlen(str), stderr);
}

bool ShellExecute(const std::string& file, const std::string& parameters, const std::string& workingDir = "")
{
#ifdef WIN32
	HANDLE childStdInRead;
	HANDLE childStdInWrite;
	HANDLE childStdOutRead;
	HANDLE childStdOutWrite;

	SECURITY_ATTRIBUTES securityAttributes;
	securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttributes.bInheritHandle = true;
	securityAttributes.lpSecurityDescriptor = nullptr;

	CreatePipe(&childStdInRead, &childStdInWrite, &securityAttributes, 0);
	SetHandleInformation(childStdInWrite, HANDLE_FLAG_INHERIT, 0);

	CreatePipe(&childStdOutRead, &childStdOutWrite, &securityAttributes, 0);
	SetHandleInformation(childStdOutRead, HANDLE_FLAG_INHERIT, 0);

	PROCESS_INFORMATION processInfo = { 0 };
	STARTUPINFO startUpInfo = { 0 };

	startUpInfo.cb = sizeof(STARTUPINFO);
	startUpInfo.hStdInput = childStdInRead;
	startUpInfo.hStdOutput = childStdOutWrite;
	startUpInfo.hStdError = childStdOutWrite;
	startUpInfo.dwFlags |= STARTF_USESTDHANDLES;

	char parametersTemp[4096];
	sprintf_s(parametersTemp, 4096, "%s %s", file.c_str(), parameters.c_str());

	std::string output;

	CreateProcess(nullptr, parametersTemp, nullptr, nullptr, true, CREATE_NO_WINDOW, nullptr, workingDir.size() == 0 ? nullptr : workingDir.c_str(), &startUpInfo, &processInfo);
	while (WaitForSingleObject(processInfo.hProcess, 100) == WAIT_TIMEOUT)
	{
		unsigned long size = GetFileSize(childStdOutRead, nullptr);

		if (size != 0)
		{
			std::string outputTemp;
			outputTemp.resize(size);

			unsigned long readBytes;
			ReadFile(childStdOutRead, &outputTemp[0], size, &readBytes, nullptr);

			output += outputTemp;
		}
	}

	unsigned long size = GetFileSize(childStdOutRead, nullptr);

	if (size != 0)
	{
		std::string outputTemp;
		outputTemp.resize(size);

		unsigned long readBytes;
		ReadFile(childStdOutRead, &outputTemp[0], size, &readBytes, nullptr);

		output += outputTemp;
	}

	unsigned long exitCode;
	GetExitCodeProcess(processInfo.hProcess, &exitCode);

	if (exitCode != 0)
	{
		// convert 'ERROR' to 'error' as remote compiler 
		// doesn't report upper case version
		size_t curPos = output.find("ERROR:", 0);
		while (curPos != std::string::npos)
		{
			output.replace(curPos, 5, "error");
			curPos = output.find("ERROR:", curPos + 1);
		}

		DumpError(output.c_str());
	}
	else
	{
		if (output.size() > 0)
			DumpWarning(output.c_str());
	}

	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	CloseHandle(childStdInRead);
	CloseHandle(childStdInWrite);
	CloseHandle(childStdOutRead);
	CloseHandle(childStdOutWrite);

	return exitCode == 0;
#else
	CryFatalError("Cannot run the process! ShellExecute is implemented ONLY for Windows Platform.");
	return false;
#endif
}

void SaveHlslToDisk(const std::vector<std::string>& code, IShader::Type type)
{
	string stage(GetGLSLANGTargetName(type));
	std::ofstream output_file(string("bin/shadercache/shader_") + stage +  ".hlsl");
	std::ostream_iterator<std::string_view> output_iterator(output_file, "\n");
	std::copy(code.begin(), code.end(), output_iterator);
	output_file.close();
}

#define OUTPUT_SPIRV_FORMAT                  ".spv"
bool CompileToSpirv(const char* name, const char* pEntrypoint, IShader::Type type, ShaderLangId langId)
{
	//else if (vkShaderCompiler == STR_VK_SHADER_COMPILER_GLSLANG)
	std::string targetEnv = "opengl";
	string stage(GetGLSLANGTargetName(type));
	
	//const bool needsInvertingY = strncmp(pTarget, "vs", 2) == 0 || strncmp(pTarget, "ds", 2) == 0 || strncmp(pTarget, "gs", 2) == 0;

	char params[1001];
	const char* extra = nullptr;
	extra			  = langId == ShaderLangId::Hlsl ? "-D" : "";
	const char *ext	  = langId == ShaderLangId::Hlsl ? ".hlsl" : ".glsl";
	sprintf(params, "%s%s%s -o %s%s_%s%s -G --target-env %s -S %s -e %s %s",
				"bin/shadercache/shader_", stage.data(), ext,
				"bin/shadercache/", name, stage.data(), OUTPUT_SPIRV_FORMAT,
				targetEnv.c_str(),
				stage.data(),
				pEntrypoint,
				extra);

	CryLog("compiler string: %s", params);

	return ShellExecute("glslangValidator.exe", params, gEnv->pSystem->GetRootFolder());
}

CShader* CShader::LoadSpirvFromFile(const char* name, const char* entry, IShader::Type type)
{
	string stage(GetGLSLANGTargetName(type));
	char file[1001];
	sprintf(file, "%s%s_%s%s",
			"bin/shadercache/", name, stage.data(), OUTPUT_SPIRV_FORMAT);
	// Load binary from file
	std::ifstream inputStream(file, std::ios::binary);
	if (!inputStream.is_open())
	{
		CryError("Could not load SPIRV file: [%s]", file);
		return nullptr;
	}
	std::istreambuf_iterator<char> startIt(inputStream), endIt;

	spirv_bin buffer(startIt, endIt); // Load file
	inputStream.close();

	return LoadSpirvFromMemory(name, entry, buffer, type);

}

bool CompileToGlsl(const char* name, IShader::Type type, ShaderLangId langId)
{	
	string stage(GetGLSLANGTargetName(type));
	std::string targetEnv = "opengl";
	
	//const bool needsInvertingY = strncmp(pTarget, "vs", 2) == 0 || strncmp(pTarget, "ds", 2) == 0 || strncmp(pTarget, "gs", 2) == 0;

	char params[1001];
	const char* extra = nullptr;
	extra			  = langId == ShaderLangId::Hlsl ? "-D" : "";
	sprintf(params, "--no-es --version 460 core --output %s%s%s %s%s_%s%s",
				"bin/shadercache/shader_", stage.data(), ".glsl",
				"bin/shadercache/", name, stage.data(), OUTPUT_SPIRV_FORMAT
				);

	CryLog("compiler string: %s", params);

	return ShellExecute("spirv-cross.exe", params, gEnv->pSystem->GetRootFolder());


}

ShaderRef CShader::LoadFromEffect(IEffect* pEffect, CShader::Type type, bool compile_to_spirv)
{
	auto tech = pEffect->GetTechnique(0);
	auto pass = tech->GetPass(0);
	auto code = pass->Code;

	if (type == IShader::E_VERTEX)
	{
		for (auto& in : pass->InputLayout)
			code.push_back(in);
	}
	if (compile_to_spirv)
	{
		auto entry = pass->EntryPoints[type].data();
		SaveHlslToDisk(code, type);
		if (CompileToSpirv(pEffect->GetName(), entry, type, pEffect->GetLangId()))
		{
			if (pEffect->GetLangId() == ShaderLangId::Hlsl)
			{
				CompileToGlsl(pEffect->GetName(), type, pEffect->GetLangId());
				if (CompileToSpirv(pEffect->GetName(), "main", type, ShaderLangId::Glsl))
				{
					return LoadSpirvFromFile(pEffect->GetName(), "main", type);
				}
			}
			else
			{
				return LoadSpirvFromFile(pEffect->GetName(), entry, type);
			}
			return nullptr;
		}
		return nullptr;
	}
	else
	{
		return CShader::LoadFromMemory(code, type);
	}
}

ShaderRef CShader::LoadFromMemory(const std::vector<std::string>& text, IShader::Type type)
{
	auto shader = ShaderRef(new CShader(type));
	if (!shader->Create())
		return nullptr;
	shader->Compile(text);
	return shader;
}

const char* IShader::GetName()
{
	return "a";
}

ShaderDesc::ShaderDesc(std::string name, const IShader::Type type): name(std::move(name)), type(type)
{
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
std::ostream& BinaryWrite(std::ostream& stream, const T& value)
{
	return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}

template<typename T>
std::istream& BinaryRead(std::istream& stream, T& value)
{
	return stream.read(reinterpret_cast<char*> (&value), sizeof(T));
}
bool CShader::SaveNativeBinary(const char* name, IShaderProgram* program, int flags)
{
	return false;
}
bool CBaseShaderProgram::SaveBinaryShader(const char* name, int flags, uint64 nMaskGen)
{
	switch (static_cast<ShaderBinaryFormat>(flags))
	{
		case ShaderBinaryFormat::SPIRV:
		{
			return true;
		}
	}
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	if (formats < 1)
	{
		CryError("Driver does not support any binary formats.");
		return false;
	}
	// Get the binary length
	GLint length = 0;
	glGetProgramiv(Get(), GL_PROGRAM_BINARY_LENGTH, &length);

	// Retrieve the binary code
	std::vector<GLubyte> buffer(length);
	GLenum format = 0;
	glGetProgramBinary(Get(), length, NULL, &format, buffer.data());

	// Write the binary to a file.
	auto const path = GetBinaryPath(name, nMaskGen);
	CryLog("Writing to %s , binary format = %d", path.data(), format);
	std::ofstream out(path.data(), std::ios::binary);
	out.write(reinterpret_cast<char*>(&format), sizeof(format));
	out.write(reinterpret_cast<char*>(&length), sizeof(length));
	out.write(reinterpret_cast<char*>(buffer.data()), length);
	out.close();
	return true;
}
spirv_bin CShader::GetSPIRV(const char* name)
{
	std::ifstream code(name, std::ios::binary);
	if (!code.is_open())
	{
		return spirv_bin();
	}
	std::istreambuf_iterator<char> startIt(code), endIt;

	CryLog("Loading SPIRV shader %s", name);
	spirv_bin buffer(startIt, endIt); // Load file
	return buffer;
}
CShader * CShader::LoadSpirvFromMemory(const char* name, const char* entry, const spirv_bin& code, IShader::Type stage)
{
	auto shader = new CShader(stage);
	if (shader->Create())
	{
		glCheck(glShaderBinary(1, &shader->m_Shader, GL_SHADER_BINARY_FORMAT_SPIR_V, code.data(), code.size()));

		// Specialize the vertex shader.
		//std::string vsEntrypoint = "main"; // Get VS entry point name
		glCheck(glSpecializeShader(shader->m_Shader, entry, 0, nullptr, nullptr));

		// Specialization is equivalent to compilation.
		GLint isCompiled = 0;
		glGetShaderiv(shader->m_Shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader->m_Shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader->m_Shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader->m_Shader);

			// Use the infoLog as you see fit.
			CryError("Loading of SPIRV FAILED)");
			gEnv->pLog->LogToFile(infoLog.data());

			// In this simple program, we'll just leave
			return nullptr;
		}
		CryLog("$3SPIRV module accepted by OpenGL");

		shader->m_Empty = false;
		debuger::shader_label(shader->Get(), name);
	}
	return shader;
}
CBaseShaderProgram* CBaseShaderProgram::LoadSpirvProgram(const char* name)
{
	
	//auto entry = GetSpirvEntry(pEffect->GetLangId(), type);
	ShaderRef vs = CShader::LoadSpirvFromFile(name, "VSMain", IShader::E_VERTEX);
	if (!vs)
		return nullptr;
	ShaderRef fs = CShader::LoadSpirvFromFile(name, "PSMain", IShader::E_FRAGMENT);
	if (!fs)
		return nullptr;

	auto p =  new CShaderProgram(vs, fs);
	p->Create(name);
	return p;
}
CBaseShaderProgram* CBaseShaderProgram::LoadNativeBinary(const char* name, uint8* code, uint format, uint length)
{
	//GLuint program = glCreateProgram();
	auto program = new CShaderProgram();

	ShaderProgramStatus st(program);

	// Install shader binary
	glProgramBinary(program->Get(), format, code, length);

	// Check for success/failure
	GLint status;
	glGetProgramiv(program->Get(), GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		CryError("Cannot link binary program.");
		st.Get(GL_LINK_STATUS);
		delete program;
		program = nullptr;
	}
	return program;

}
CBaseShaderProgram* CBaseShaderProgram::LoadBinaryShader(const char* name, int flags, uint64 nMaskGen)
{
	switch (static_cast<ShaderBinaryFormat>(flags))
	{
	case ShaderBinaryFormat::SPIRV:
	{
		return LoadSpirvProgram(name);
	}
	case ShaderBinaryFormat::VENDOR:
	{
		// Load binary from file
		std::ifstream inputStream(name, std::ios::binary);
		if (!inputStream.is_open())
		{
			return nullptr;
		}
		std::istreambuf_iterator<char> startIt(inputStream), endIt;

		GLenum format = 0;
		uint length	  = 0;

		BinaryRead(inputStream, format);
		BinaryRead(inputStream, length);
		spirv_bin buffer(startIt, endIt); // Load file
		inputStream.close();
		CryLog("Loading binary shader %s , binary format = %d, binary length = %d", name, format, length);
		const auto path = GetBinaryPath(name, nMaskGen);
		return LoadNativeBinary(path.c_str(), (uint8*)(buffer.data()), format, length);
	}
	default:
		return nullptr;
	}
}
