#pragma once
#if 0
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

using BB_VERTEX	 = SVF_P3F_C4B;

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
};

#if 0
using SAABBBuffer = gl::ConstantBuffer<SAABBConstantBuffer>;
using SAABBBufferPtr = std::shared_ptr<SAABBBuffer>;
#endif


using AuxPushBuffer	  = std::vector<SAuxPushBufferEntry>;
using AuxVertexBuffer = std::vector<SAuxVertex>;
using AuxIndexBuffer = std::vector<vtx_idx>;


class CRenderAuxGeom : public IRenderAuxGeom
{
	const int INIT_VB_SIZE = 1024 * 4;
	using BoundingBox = std::array<BB_VERTEX, 36>;

  public:
	CRenderAuxGeom();
	~CRenderAuxGeom();
	void DrawAABB(Vec3 min, Vec3 max, const UCol& col) override;
	void DrawTriangle(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, const Vec3& v2, const UCol& colV2) override;
	void DrawLine(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, float thickness = 1.0f) override;
	void DrawLines(const Vec3* v, uint32 numPoints, const UCol& col, float thickness = 1.0f) override;
	void DrawQuad(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, const Vec3& v2, const UCol& colV2, const Vec3& v3, const UCol& colV3)
	{
		DrawTriangle(v0, colV0, v1, colV1, v2, colV2);
		DrawTriangle(v0, colV0, v2, colV2, v3, colV3);
	}
	void Flush() override;

  private:
	void AddPrimitive(SAuxVertex*& pVertices, uint32 numVertices, RenderPrimitive primitive);
	void DrawAABBs();
	void DrawLines();

  private:
	CVertexBuffer* m_BoundingBox	= nullptr;
	SVertexStream* m_BB_IndexBuffer = nullptr;
	AuxPushBuffer m_auxPushBuffer;
	AuxVertexBuffer m_VB;
	CVertexBuffer* m_HardwareVB = nullptr;
	_smart_ptr<IShader> m_BoundingBoxShader;
	_smart_ptr<IShader> m_AuxGeomShader;

	//SAABBBufferPtr  m_aabbBufferPtr;

	std::vector<BoundingBox> m_BBVerts;

	int m_CurrentVB_Size = INIT_VB_SIZE;
	int dbg_mode		 = 0;
	int stop			 = 0;

	static ID3D10DepthStencilState* m_pDSState;

	// Inherited via IRenderAuxGeom
	virtual void PushImage(const SRender2DImageDescription& image) override;
	virtual void DrawMesh(CVertexBuffer* pVertexBuffer) override;
};
#endif
