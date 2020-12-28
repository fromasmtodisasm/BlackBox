#pragma once
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <map>
#include <memory>
#include <string>

enum class ShaderBinaryFormat
{
	SPIRV = 1,
	VENDOR = 1 << 2
};

struct ICVar;

class CShader;
class CBaseShaderProgram;
class CShaderProgram;

struct ShaderStatus
{
	char m_InfoLog[512] = {0};
	int m_Status		= GL_FALSE;
	CShader* m_Shader	= nullptr;

	ShaderStatus(CShader* shader);
	bool Get(GLenum statusType);
};

bool SaveBinaryShader(const char* name, IShaderProgram* program, int flags);
IShaderProgram* LoadBinaryShader(const char* name, int flags);

struct ShaderProgramStatus
{
	char m_InfoLog[1024]{};
	int m_Status{};
	CBaseShaderProgram* m_Program;

	ShaderProgramStatus(CBaseShaderProgram* program);
	bool Get(GLenum statusType);
};

using spirv_bin = std::vector<uint8>;
class CShader : public IShader
{
  public:
	CShader(CShader::Type type);
	~CShader();
	// Inherited via IShader
	virtual void AddRef() override;
	virtual int Release() override;

	static CShader* Load(ShaderDesc const& desc);
	static CShader* Load(std::string_view source);
	static CShader* LoadSpirv(const char * name, const char * entry, const spirv_bin& code, IShader::Type stage);
	static bool ParseLine(string& buffer);
	static bool LoadInternal(string const& path, string& buffer);
	static bool LoadFromStream(const std::stringstream stream, string& buffer);
	static ShaderRef LoadFromMemory(const std::vector<std::string_view>& text, CShader::Type type);
	static auto LoadFromEffect(struct IEffect* pEffect, CShader::Type type, bool compile_to_spirv) -> ShaderRef;

	virtual bool Create() override;
	virtual bool Compile(std::vector<std::string_view> code) override;
	virtual bool Bind() override;
	virtual bool Empty() override;
	virtual void Print() override;
	virtual const char* TypeToStr() override;
	virtual const char* GetName() override;
	virtual uint Get() override;

  private:
	GLuint m_Shader;
	ShaderStatus m_Status;
	bool m_Empty;
	int m_Refs = 0;

  public:
	string m_Path;
	IShader::Type m_Type;
	virtual IShader::Type GetType() override;
};

class CBaseShaderProgram : public IShaderProgram
{
public:
	CBaseShaderProgram();
	CBaseShaderProgram(uint binary);
	CBaseShaderProgram(ShaderRef vs, ShaderRef fs);
	CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs);
	CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs, const ShaderInfo& gs);
	CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs, const ShaderInfo& gs, const ShaderInfo& cs);
	virtual ~CBaseShaderProgram();

	virtual void AddRef() override;
	virtual int Release() override;

	virtual bool Create(const char* label) override;
	virtual void Attach(ShaderInfo& shader) override;
	virtual ShaderInfo& AttachInternal(ShaderInfo& src, ShaderInfo& dst) override;
	virtual void Detach(ShaderInfo& shader) override;
	virtual bool Dispatch(int x, int y, int z, GLbitfield barriers) override;
	virtual bool DispatchIndirect() override;
	virtual bool Link() override;
	virtual void Use() override;
	virtual void Unuse() override;
	virtual void DeleteProgram() override;
	virtual GLint GetUniformLocation(const char* format, ...) override;
	virtual GLint GetUniformLocation(string& name) override;
	UniformValue GetUniformValue(const char* name);
	virtual void Uniform(bool value, const char* format, ...) override { Uniform((int)value, format); }
	virtual void Uniform(const int value, const char* format, ...) override;
	virtual void Uniform(const unsigned int value, const char* format, ...) override;
	virtual void Uniform(const float value, const char* format, ...) override;
	virtual void Uniform(const Vec1 value, const char* format, ...) override;
	virtual void Uniform(const Vec2 value, const char* format, ...) override;
	virtual void Uniform(const Vec3 value, const char* format, ...) override;
	virtual void Uniform(const Vec4 value, const char* format, ...) override;
	virtual void Uniform(const Mat2 value, const char* format, ...) override;
	virtual void Uniform(const Mat3 value, const char* format, ...) override;
	virtual void Uniform(const Mat4 value, const char* format, ...) override;
	virtual void Uniform(const glm::ivec4 value, const char* format, ...) override;
	virtual void Uniform(const ITexture* texture, const char* format, ...) override;

	template<typename T>
	void Uniform(const T value, string name)
	{
		Uniform(value, name.c_str());
	}

	//virtual void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) override;

	virtual void BindTexture2D(GLuint texture, GLint unit, const char* sampler) override;
	virtual void BindTextureUnit2D(GLuint texture, GLint unit) override;
	virtual uint Get() override;
	virtual void setup() override{};
	virtual void Dump() override;

  private:
	void Reset(ShaderInfo src);
	static const char* BuildName(const char* format, va_list args);

  public:
	const size_t BUFFER_SIZE = 1024;
	ShaderInfo m_Vertex;
	ShaderInfo m_Fragment;
	ShaderInfo m_Geometry;
	ShaderInfo m_Compute;

	uint m_Program;
	char infoLog[512];
	ShaderProgramStatus m_Status;

	bool created = false;
	std::map<string, GLint> m_Cache;
	static char* buffer;
	string name;
	static ICVar* print_loc_name;
	static ICVar* use_cache;

	int m_Refs = 0;

	// Inherited via IShaderProgram
	virtual const char* GetShaderName(IShader::Type type) override;

	// Inherited via IShaderProgram
	virtual void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) override;

	// Inherited via IShaderProgram
	virtual int Reload() override;
};

CShader::Type str2typ(string type);
