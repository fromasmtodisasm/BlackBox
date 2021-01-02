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
protected:
	~CShader();
public:
	CShader(CShader::Type type);
	// Inherited via IShader
	void AddRef() override;
	int Release() override;

	static CShader* Load(ShaderDesc const& desc);
	static CShader* Load(std::string source);
	static CShader* LoadSpirvFromMemory(const char * name, const char * entry, const spirv_bin& code, IShader::Type stage);
	static CShader* LoadSpirvFromFile(const char * name, const char * entry, IShader::Type stage);
	static bool ParseLine(string& buffer);
	static bool LoadInternal(string const& path, string& buffer);
	static bool LoadFromStream(const std::stringstream stream, string& buffer);
	static ShaderRef LoadFromMemory(const std::vector<std::string>& text, CShader::Type type);
	static auto LoadFromEffect(struct IEffect* pEffect, CShader::Type type, bool compile_to_spirv) -> ShaderRef;

	bool Create() override;
	bool Compile(std::vector<std::string> code) override;
	bool Bind() override;
	bool Empty() override;
	void Print() override;
	const char* TypeToStr() override;
	const char* GetName() override;
	uint Get() override;

	static bool ComileSPIRV();
	static bool SaveNativeBinary(const char* name, IShaderProgram* program, int flags);
	static bool SaveBinaryShader(const char* name, IShaderProgram* program, int flags, uint64 nMaskGen);
	spirv_bin GetSPIRV(const char* name);

  private:
	GLuint m_Shader;
	ShaderStatus m_Status;
	bool m_Empty;
	int m_Refs = 0;

  public:
	string m_Path;
	IShader::Type m_Type;
	IShader::Type GetType() override;
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

	void AddRef() override;
	int Release() override;

	bool Create(const char* label) override;
	void Attach(ShaderInfo& shader) override;
	ShaderInfo& AttachInternal(ShaderInfo& src, ShaderInfo& dst) override;
	void Detach(ShaderInfo& shader) override;
	bool Dispatch(int x, int y, int z, GLbitfield barriers) override;
	bool DispatchIndirect() override;
	bool Link() override;
	void Use() override;
	void Unuse() override;
	void DeleteProgram() override;
	GLint GetUniformLocation(const char* format, ...) override;
	GLint GetUniformLocation(string& name) override;
	UniformValue GetUniformValue(const char* name);
	void Uniform(bool value, const char* format, ...) override { Uniform((int)value, format); }
	void Uniform(const int value, const char* format, ...) override;
	void Uniform(const unsigned int value, const char* format, ...) override;
	void Uniform(const float value, const char* format, ...) override;
	void Uniform(const Vec1 value, const char* format, ...) override;
	void Uniform(const Vec2 value, const char* format, ...) override;
	void Uniform(const Vec3 value, const char* format, ...) override;
	void Uniform(const Vec4 value, const char* format, ...) override;
	void Uniform(const Mat2 value, const char* format, ...) override;
	void Uniform(const Mat3 value, const char* format, ...) override;
	void Uniform(const Mat4 value, const char* format, ...) override;
	void Uniform(const glm::ivec4 value, const char* format, ...) override;
	void Uniform(const ITexture* texture, const char* format, ...) override;

	template<typename T>
	void Uniform(const T value, string name)
	{
		Uniform(value, name.c_str());
	}

	//virtual void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) override;

	void BindTexture2D(GLuint texture, GLint unit, const char* sampler) override;
	void BindTextureUnit2D(GLuint texture, GLint unit) override;
	uint Get() override;
	void setup() override{};
	void Dump() override;

	bool SaveBinaryShader(const char* name, int flags, uint64 nMaskGen);
	static CBaseShaderProgram* LoadSpirvProgram(const char* name);
	static CBaseShaderProgram* LoadNativeBinary(const char* name, uint8* code, uint format, uint length);
	static CBaseShaderProgram* LoadBinaryShader(const char* name, int flags, uint64 nMaskGen);


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
	const char* GetShaderName(IShader::Type type) override;

	// Inherited via IShaderProgram
	void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) override;

	// Inherited via IShaderProgram
	int Reload() override;
};

CShader::Type str2typ(string type);
