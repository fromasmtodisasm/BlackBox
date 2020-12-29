#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>

struct SAuxPushBufferEntry
{
	SAuxPushBufferEntry() = default;
	SAuxPushBufferEntry(uint32 nv, RenderPrimitive rpt)
		: m_numVertices(nv), m_primitive(rpt)
	{
	}
	uint32 m_numVertices;
	RenderPrimitive m_primitive;
};

struct alignas(16) SAABBConstantBuffer
{
	Mat4 Model;
	Vec3 LightPos;
	bool bTonemap;
	int dbgmode = 0;
	float Alpha;
	Vec4 Color;
	Vec3 Eye;
};

using SAABBBuffer = gl::ConstantBuffer<SAABBConstantBuffer>;
using SAABBBufferPtr = std::shared_ptr<SAABBBuffer>;


using AuxPushBuffer	  = std::vector<SAuxPushBufferEntry>;
using AuxVertexBuffer = std::vector<SAuxVertex>;

class CRenderAuxGeom : public IRenderAuxGeom
{
	const int INIT_VB_SIZE = 1024 * 4;

  public:
	CRenderAuxGeom();
	~CRenderAuxGeom();
	void DrawAABB(Vec3 min, Vec3 max, const UCol& col) override;
	void DrawTriangle(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, const Vec3& v2, const UCol& colV2) override;
	void DrawLine(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, float thickness = 1.0f) override;
	void DrawLines(const Vec3* v, uint32 numPoints, const UCol& col, float thickness = 1.0f) override;
	void Flush() override;

  private:
	void AddPrimitive(SAuxVertex*& pVertices, uint32 numVertices, RenderPrimitive primitive);

  private:
	CVertexBuffer* m_BoundingBox	= nullptr;
	SVertexStream* m_BB_IndexBuffer = nullptr;
	AuxPushBuffer m_auxPushBuffer;
	AuxVertexBuffer m_VB;
	CVertexBuffer* m_HardwareVB = nullptr;
	ShaderProgramRef m_BoundingBoxShader;

	SAABBBufferPtr  m_aabbBufferPtr;

	int m_CurrentVB_Size = INIT_VB_SIZE;
	int dbg_mode		 = 0;
	int stop			 = 0;
};
