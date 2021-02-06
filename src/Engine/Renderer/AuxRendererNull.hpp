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

