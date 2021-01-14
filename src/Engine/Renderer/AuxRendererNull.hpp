#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
#include <BlackBox/Renderer/IShader.hpp>
class CRenderAuxGeomNull : public IRenderAuxGeom
{
  public:
	void DrawAABB(Vec3 min, Vec3 max, const UCol& col) override
	{
	}
	void DrawTriangle(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, const Vec3& v2, const UCol& colV2) override
	{
	}
	void DrawLine(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, float thickness = 1.0f) override
	{
	}
	void DrawLines(const Vec3* v, uint32 numPoints, const UCol& col, float thickness = 1.0f) override
	{
	}
	void Flush() override
	{
	}
};

struct CTextureNull : ITexture
{
	// Inherited via ITexture
	virtual bool load(const char* name) override;
	virtual void bind() override;
	virtual void setUnit(uint unit) override;
	virtual uint getUnit() const override;
	virtual void setType(const char* TextureType) override;
	virtual const char* typeToStr() override;
	virtual int getWidth() const override;
	virtual int getHeight() const override;
	virtual int getId() const override;
	virtual uint64 getBindlesId() const override;
	virtual void bind() const override;
};

struct CShaderNull : public IShaderProgram
{
	// Inherited via IShaderProgram
	virtual void AddRef() override;
	virtual int Release() override;
	virtual int Reload() override;
	virtual bool Create(const char* label) override;
	virtual void Attach(ShaderInfo& shader) override;
	virtual ShaderInfo& AttachInternal(ShaderInfo& src, ShaderInfo& dst) override;
	virtual void Detach(ShaderInfo& shader) override;
	virtual bool Dispatch(int x, int y, int z, uint barriers) override;
	virtual bool DispatchIndirect() override;
	virtual bool Link() override;
	virtual void Use() override;
	virtual void Unuse() override;
	virtual void DeleteProgram() override;
	virtual int GetUniformLocation(const char* format, ...) override;
	virtual int GetUniformLocation(string& name) override;
	virtual const char* GetShaderName(IShader::Type type) override;
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
	virtual void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) override;
	virtual void BindTexture2D(uint texture, int unit, const char* sampler) override;
	virtual void BindTextureUnit2D(uint texture, int unit) override;
	virtual uint Get() override;
	virtual void setup() override;
	virtual void Dump() override;
};
