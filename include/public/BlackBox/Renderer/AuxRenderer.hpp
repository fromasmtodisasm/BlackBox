#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>

#if 0
using BB_VERTEX	 = SVF_P3F_C4B;
#else
//using BB_VERTEX	 = SVF_P3F_C4B_T2F;
using BB_VERTEX	 = SVF_P3F_N_T2F;

#endif

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
	Legacy::Mat4 Model;
	Legacy::Vec3 LightPos;
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
	void DrawAABB(Legacy::Vec3 min, Legacy::Vec3 max, const UCol& col) override;
	void DrawTriangle(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, const Legacy::Vec3& v2, const UCol& colV2) override;
	void DrawLine(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, float thickness = 1.0f) override;
	void DrawLines(const Legacy::Vec3* v, uint32 numPoints, const UCol& col, float thickness = 1.0f) override;
	void PushImage(const SRender2DImageDescription& image) override;
	void Flush() override;
	void DrawMesh(CVertexBuffer* pVertexBuffer) override;

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

	std::vector<SRender2DImageDescription> m_Images;

	//SAABBBufferPtr  m_aabbBufferPtr;

	std::vector<CVertexBuffer*> m_Meshes;

	std::vector<BoundingBox> m_BBVerts;

	int m_CurrentVB_Size = INIT_VB_SIZE;
	int dbg_mode		 = 0;
	int stop			 = 0;

public:
	static ID3D10DepthStencilState* m_pDSState;
};
