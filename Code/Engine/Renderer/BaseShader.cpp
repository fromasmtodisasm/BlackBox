#include <BlackBox/Renderer/BaseShader.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>

#include <fstream>
#include <string>
#include <cstdarg>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

std::array<char, CBaseShaderProgram::BUFFER_SIZE> CBaseShaderProgram::buffer;

const char* CBaseShaderProgram::BuildName(const char* format, va_list args)
{
	vsprintf(buffer.data(), format, args);
	return buffer.data();
}

#define BUILD_NAME(a, b) format

CShader::Type str2typ(std::string type)
{
	if (type == "vertex")
		return CShader::Type::E_VERTEX;
	else if (type == "fragment")
		return CShader::Type::E_FRAGMENT;
	else if (type == "geometry")
		return CShader::Type::E_GEOMETRY;
	else if (type == "compute")
		return CShader::Type::E_COMPUTE;
	else
		return CShader::Type::E_UNKNOWN;
}

ShaderStatus::ShaderStatus(CShader* shader) : m_Shader(shader)
{
}

bool ShaderStatus::Get(GLenum statusType)
{
	//GLenum err;
	glCheck(glGetShaderiv(m_Shader->Get(), statusType, &m_Status));
	if (m_Status == GL_FALSE)
	{
		CryError("Pizdec Shader %s", m_Shader->GetName());
		GLsizei length = 0;
		glGetShaderiv(m_Shader->Get(), GL_INFO_LOG_LENGTH, &length);
		std::vector<GLchar> errorLog(length);
		glCheck(glGetShaderInfoLog(m_Shader->Get(), length, &length, &errorLog[0]));
		CryError("Shader %s ErrorLog: [%s]\n", m_Shader->GetName(), errorLog.data());
		glDeleteShader(m_Shader->Get());
		return false;
	}
	return true;
}

ShaderProgramStatus::ShaderProgramStatus(CBaseShaderProgram* program) : m_Program(program)
{
}

bool ShaderProgramStatus::Get(GLenum statusType)
{
	GLsizei size;
	glCheck(glGetProgramiv(m_Program->Get(), statusType, &m_Status));
	if (m_Status == GL_FALSE)
	{
		glCheck(glGetProgramInfoLog(m_Program->Get(), sizeof(m_InfoLog), &size, m_InfoLog));
		auto log = GetISystem()->GetILog();
		if (log != nullptr)
		{
			CryError("Shader::programm: %s\n", m_InfoLog);
			std::vector<char> label(1);
			GLsizei length = 0;
			glCheck(glGetObjectLabel(GL_PROGRAM, this->m_Program->Get(), 1, &length, label.data()));
			if (length > 0)
			{
				label.resize(length);
				glCheck(glGetObjectLabel(GL_PROGRAM, this->m_Program->Get(), length, &length, label.data()));
				CryLog("Shader::programm label: %s\n", label.data());
			}
		}
		else
			CryError((std::string("Shader::programm: ") + m_InfoLog).c_str());
		return false;
	}
	return true;
}
CShader::CShader(CShader::Type Type) : m_Type(Type), m_Status(this), m_Empty(true)
{
}

CShader::~CShader()
{
	glCheck(glDeleteShader(m_Shader));
}

static int get_gl_enum(IShader::Type Type)
{
	switch (Type)
	{
	case IShader::E_VERTEX:
		return GL_VERTEX_SHADER;
	case IShader::E_FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case IShader::E_GEOMETRY:
		return GL_GEOMETRY_SHADER;
	case IShader::E_COMPUTE:
		return GL_COMPUTE_SHADER;
	case IShader::E_UNKNOWN:
		return -1;
	default:
		return -1;
	}
}

bool CShader::Create()
{
	m_Shader = glCreateShader(get_gl_enum(m_Type));
	if (m_Shader != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
	// return m_Status.get(GL_VALIDATE_STATUS);
}

bool CShader::Compile(std::vector<std::string> code)
{
	//assert(code.size() == 2);
	auto text_size	   = sizeof(char*) * code.size();
	auto text = static_cast<char**>(alloca(text_size));

	for (int i = 0; i < code.size(); i++)
	{
		text[i] = const_cast<char*>(code[i].data());
	}

	auto lengths_size	   = sizeof(char*) * code.size();
	auto lengths = static_cast<int*>(alloca(lengths_size));

	for (int i = 0; i < code.size(); i++)
	{
		lengths[i] = code[i].size();
	}

	if (auto cv = gEnv->pConsole->GetCVar("dump_shaders_on_load"); cv)
	{
		if (cv->GetIVal())
		{
			CryLog("Dump shader strings");
			for (int i = 0; i < code.size(); i++)
			{
				CryLog(text[i]);
			}
		}
	}
	glCheck(glShaderSource(m_Shader, code.size(), text, lengths));
	glCheck(glCompileShader(m_Shader));
	//glCompileShaderIncludeARB(m_Shader, )
	return m_Status.Get(GL_COMPILE_STATUS);
}

bool CShader::Bind()
{
	return true;
}

bool CShader::Empty()
{
	return m_Empty;
}

void CShader::Print()
{
#ifdef _DEBUG

#endif
}

const char* CShader::TypeToStr()
{
	switch (m_Type)
	{
	case E_VERTEX:
		return "vertex";
	case E_GEOMETRY:
		return "geometry";
	case E_COMPUTE:
		return "compute";
	case E_FRAGMENT:
		return "fragment";
	default:
		return "unknown";
	}
}

const char* CShader::GetName()
{
	return m_Path.c_str();
}

GLuint CShader::Get()
{
	return m_Shader;
}

IShader::Type CShader::GetType()
{
	return m_Type;
}

void CShader::AddRef()
{
	m_Refs++;
}

int CShader::Release()
{
	m_Refs--;
	if (m_Refs == 0)
	{
		delete this;
	}
	return m_Refs;
}

CBaseShaderProgram::CBaseShaderProgram() : m_Status(this)
{
	m_Program = glCreateProgram();
	created	  = true;
}
CBaseShaderProgram::CBaseShaderProgram(uint binary) : m_Status(this)
{
	m_Vertex   = ShaderInfo(nullptr, "binary");
	m_Fragment = ShaderInfo(nullptr, "binary");
	created	  = true;
}

CBaseShaderProgram::CBaseShaderProgram(
	ShaderRef vs, ShaderRef fs)
	: CBaseShaderProgram()
{
	
	m_Vertex   = ShaderInfo(vs, "");
	m_Fragment = ShaderInfo(fs, "");
}

CBaseShaderProgram::CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs)
	: CBaseShaderProgram()
{
	m_Vertex   = vs;
	m_Fragment = fs;
}

CBaseShaderProgram::CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs, const ShaderInfo& gs)
	: CBaseShaderProgram()
{
	m_Vertex   = vs;
	m_Fragment = fs;
	m_Geometry = gs;
}

CBaseShaderProgram::CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs, const ShaderInfo& gs, const ShaderInfo& cs)
	: CBaseShaderProgram()
{
	m_Vertex   = vs;
	m_Fragment = fs;
	m_Geometry = gs;
	m_Compute  = cs;
}

CBaseShaderProgram::~CBaseShaderProgram()
{
	glDeleteProgram(m_Program);
}

bool CBaseShaderProgram::Create(const char* label)
{
	if (!created)
	{
		created	  = true;
		m_Program = glCreateProgram();
	}
	debuger::program_label(m_Program, label);
	Attach(m_Vertex);
	Attach(m_Fragment);
	Attach(m_Geometry);
	Attach(m_Compute);
	glProgramParameteri(m_Program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);
	auto status = Link();
	Detach(m_Vertex);
	Detach(m_Fragment);
	Detach(m_Geometry);
	Detach(m_Compute);
	return status;
}

void CBaseShaderProgram::Attach(ShaderInfo& info)
{
	ShaderInfo& attached = info;

	if (!info.used)
		return;
	switch (info.shader->GetType())
	{
	case CShader::Type::E_VERTEX:
		attached = AttachInternal(info, m_Vertex);
		break;
	case CShader::Type::E_TESSELATION_CONTROL:
		break;
	case CShader::Type::E_TESSELATION_EVALUATION:
		break;
	case CShader::Type::E_FRAGMENT:
		attached = AttachInternal(info, m_Fragment);
		break;
	case CShader::Type::E_GEOMETRY:
		attached = AttachInternal(info, m_Geometry);
		break;
	case CShader::Type::E_COMPUTE:
		attached = AttachInternal(info, m_Compute);
		break;
	case CShader::Type::E_UNKNOWN:
		assert(0);
		break;
	}
	attached.attached = true;
	glCheck(glAttachShader(m_Program, info.shader->Get()));
}

CBaseShaderProgram::ShaderInfo& CBaseShaderProgram::AttachInternal(ShaderInfo& src, ShaderInfo& dst)
{
	if (dst.shader == nullptr)
		dst.shader = src.shader;
	if (dst.attached)
		Detach(src);
	return dst;
}

void CBaseShaderProgram::Detach(ShaderInfo& info)
{
	if (info.attached && info.shader != nullptr)
		glCheck(glDetachShader(m_Program, info.shader->Get()));
}

bool CBaseShaderProgram::Dispatch(int x, int y, int z, GLbitfield barriers)
{
	if (m_Compute.attached)
	{
		glCheck(glDispatchCompute(x, y, z));
		if (barriers)
			glCheck(glMemoryBarrier(barriers));
		return true;
	}
	return false;
}

bool CBaseShaderProgram::DispatchIndirect()
{
	return false;
}

bool CBaseShaderProgram::Link()
{
	glCheck(glLinkProgram(m_Program));
	return m_Status.Get(GL_LINK_STATUS);
}

void CBaseShaderProgram::Use()
{
	glCheck(glUseProgram(m_Program));
}

void CBaseShaderProgram::Unuse()
{
	glCheck(glUseProgram(0));
}

void CBaseShaderProgram::DeleteProgram()
{
	glDeleteProgram(m_Program);
	created = false;
}

GLint CBaseShaderProgram::GetUniformLocation(const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = -1;
//if (use_cache->GetIVal())
#if 0
  {
    auto it = m_Cache.find(name);
    if (it != m_Cache.end())
      loc = it->second;
    else
    {
      loc = glGetUniformLocation(m_Program, name);
      m_Cache[name] = loc;
    }
  }
#else
	{
		loc			  = glGetUniformLocation(m_Program, name);
		m_Cache[name] = loc;
	}
#endif
	return loc;
}

GLint CBaseShaderProgram::GetUniformLocation(string& name)
{
	return GetUniformLocation(name.c_str());
}

UniformValue CBaseShaderProgram::GetUniformValue(const char* name)
{
	//UniformValue result(name);
	//auto location = GetUniformLocation(name);

	//result.location = location;
	////result.program = m_Program;
	//return result;
	return UniformValue();
}

void CBaseShaderProgram::Uniform(const int value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform1i(loc, value));
	}
}

void CBaseShaderProgram::Uniform(const unsigned int value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform1ui(loc, value));
	}
}

void CBaseShaderProgram::Uniform(const float value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform1f(loc, value));
	}
}

void CBaseShaderProgram::Uniform(const Vec1 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform1f(loc, value[0]));
	}
}

void CBaseShaderProgram::Uniform(const Vec2 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform2fv(loc, 1, glm::value_ptr(value)));
	}
}

void CBaseShaderProgram::Uniform(const Vec3 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform3fv(loc, 1, glm::value_ptr(value)));
	}
}

void CBaseShaderProgram::Uniform(const Vec4 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform4fv(loc, 1, glm::value_ptr(value)));
	}
}

void CBaseShaderProgram::Uniform(const glm::ivec4 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniform4iv(loc, 1, glm::value_ptr(value)));
	}
}

void CBaseShaderProgram::Uniform(const ITexture* texture, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		BindTexture2D(texture->getId(), texture->getUnit(), name);
	}
}

void CBaseShaderProgram::Uniform(const Mat2 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniformMatrix2fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
	}
}

void CBaseShaderProgram::Uniform(const Mat3 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
	}
}

void CBaseShaderProgram::Uniform(const Mat4 value, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto name = BUILD_NAME(format, ptr);
	va_end(ptr);

	GLint loc = GetUniformLocation(name);
	if (loc != -1)
	{
		glCheck(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
	}
}

void CBaseShaderProgram::BindTexture2D(GLuint texture, GLint unit, const char* sampler)
{
	BindTextureUnit2D(texture, unit);
	Uniform(unit, sampler);
}

void CBaseShaderProgram::BindTextureUnit2D(GLuint texture, GLint unit)
{
	gl::ActiveTexture(GL_TEXTURE0 + unit);
	gl::BindTexture2D(GL_TEXTURE_2D, texture);
}

GLuint CBaseShaderProgram::Get()
{
	return m_Program;
}

void CBaseShaderProgram::Dump()
{
	int len = 0;
	GLenum bFormat;
	glGetProgramiv(m_Program, GL_PROGRAM_BINARY_LENGTH, &len);

	std::unique_ptr<uint8_t> binary(new uint8_t[len]);
	glGetProgramBinary(m_Program, len, &len, &bFormat, binary.get());

	FILE* shader = fopen("dump.bin", "wb");
	if (!shader)
	{
		//GetISystem()->Log("Cannot open file for dump");
		return;
	}

	fwrite(binary.get(), 1, len, shader);
	fclose(shader);
}

void CBaseShaderProgram::Reset(ShaderInfo src)
{
	if (src.shader != nullptr)
		//src.shader.reset();
		src.shader = nullptr;
}

const char* CBaseShaderProgram::GetShaderName(IShader::Type Type)
{
	switch (Type)
	{
	case IShader::E_VERTEX:
		return m_Vertex.name.data();
	case IShader::E_FRAGMENT:
		return m_Fragment.name.data();
	case IShader::E_GEOMETRY:
		return m_Geometry.name.data();
	case IShader::E_COMPUTE:
		return m_Compute.name.data();
	case IShader::E_UNKNOWN:
		return "Unknown";
	default:
		assert(0);
		return "";
	}
}

void CBaseShaderProgram::Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label)
{
	Detach(m_Vertex);
	//reset(m_Vertex);
	Detach(m_Fragment);
	//reset(m_Fragment);
	Detach(m_Geometry);
	//reset(m_Geometry);
	Detach(m_Compute);
	//reset(m_Compute);
	DeleteProgram();

	m_Vertex.shader	  = v;
	m_Fragment.shader = f;
	if (!g)
		m_Geometry.shader = g;
	if (!c)
		m_Compute.shader = c;
	Create(label);
}

int CBaseShaderProgram::Reload()
{
	return 0;
}

void CBaseShaderProgram::AddRef()
{
	m_Refs++;
}

int CBaseShaderProgram::Release()
{
	m_Refs--;
	if (m_Refs == 0)
	{
		delete this;
	}
	return m_Refs;
}
