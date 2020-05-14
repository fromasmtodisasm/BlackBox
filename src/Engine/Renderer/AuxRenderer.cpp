#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>

using P3F	= SVF_P3F;
using VecPos = std::vector<P3F>;

namespace
{
	static inline uint32 PackColor(const UCol& col)
	{
		return (((uint8)(col.bcolor[0]) << 24) +
				((uint8)(col.bcolor[1]) << 16) +
				((uint8)(col.bcolor[2]) << 8) +
				((uint8)(col.bcolor[3])));
	}
} // namespace

CRenderAuxGeom::CRenderAuxGeom()
{
	// Cube 1x1x1, centered on origin
	static P3F vertices[] = {
		Vec3{-0.5f, -0.5f, -0.5f},
		Vec3{0.5, -0.5, -0.5},
		Vec3{0.5, 0.5, -0.5},
		Vec3{-0.5, 0.5, -0.5},
		Vec3{-0.5, -0.5, 0.5},
		Vec3{0.5, -0.5, 0.5},
		Vec3{0.5, 0.5, 0.5},
		Vec3{-0.5, 0.5, 0.5}};
	static uint16 elements[] = {
#if 1
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		0,
		4,
		1,
		5,
		2,
		6,
		3,
		7
#else
		0,
		3,
		2,
		1,
		2,
		3,
		7,
		6,
		0,
		4,
		7,
		3,
		1,
		2,
		6,
		5,
		4,
		5,
		6,
		7,
		0,
		1,
		5,
		4
#endif
	};
	///////////////////////////////////////////////////////////////////////////////
	int cnt		  = sizeof vertices / sizeof P3F;
	m_BoundingBox = gEnv->pRenderer->CreateBuffer(cnt, VERTEX_FORMAT_P3F, "BoundingBox", false);
	gEnv->pRenderer->UpdateBuffer(m_BoundingBox, vertices, cnt, false);

	m_BB_IndexBuffer = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_BB_IndexBuffer, elements, (sizeof elements / sizeof uint16));
	///////////////////////////////////////////////////////////////////////////////
	m_HardwareVB		= gEnv->pRenderer->CreateBuffer(INIT_VB_SIZE, VERTEX_FORMAT_P3F_C4B_T2F, "AuxGeom", true);
	m_BoundingBoxShader = gEnv->pRenderer->Sh_Load("bb.vs", "bb.frag");
}

CRenderAuxGeom::~CRenderAuxGeom()
{
	SAFE_DELETE(m_BoundingBox);
	SAFE_DELETE(m_BB_IndexBuffer);
}

//TODO: Довести до ума, нужно учитывать трансформации объекта
void CRenderAuxGeom::DrawAABB(Vec3 min, Vec3 max, UCol& col)
{
	auto& shader = m_BoundingBoxShader;

	glm::vec3 size		= glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	glm::vec3 center	= glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	const auto& cam = gEnv->pRenderer->GetCamera();
	Vec4 color		= Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3]);
	shader->Use();
	shader->Uniform(transform, "model");
	shader->Uniform(cam.getViewMatrix(), "view");
	shader->Uniform(cam.getProjectionMatrix(), "projection");
	shader->Uniform(0.1f, "alpha");
	shader->Uniform(color, "color");

	{
		RSS(gEnv->pRenderer, BLEND, true);
		RSS(gEnv->pRenderer, CULL_FACE, false);
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 0, static_cast<int>(RenderPrimitive::LINE_LOOP));
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 4, static_cast<int>(RenderPrimitive::LINE_LOOP));
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 8, 8, static_cast<int>(RenderPrimitive::LINES));
	}
	shader->Unuse();
	//gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 18, static_cast<int>(RenderPrimitive::LINES));
}

void CRenderAuxGeom::DrawTriangle(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, const Vec3& v2, const UCol& colV2)
{
	SAuxVertex* pVertices(nullptr);
	AddPrimitive(pVertices, 3, RenderPrimitive::TRIANGLES);

	pVertices[0].xyz		  = v0;
	pVertices[0].color.dcolor = PackColor(colV0);

	pVertices[1].xyz		  = v1;
	pVertices[1].color.dcolor = PackColor(colV1);

	pVertices[2].xyz		  = v2;
	pVertices[2].color.dcolor = PackColor(colV2);
}

void CRenderAuxGeom::DrawLine(const Vec3& v0, const UCol& colV0, const Vec3& v1, const UCol& colV1, float thickness)
{
	if (thickness <= 1.0f)
	{
		SAuxVertex* pVertices(nullptr);
		AddPrimitive(pVertices, 2, RenderPrimitive::LINES);

		pVertices[0].xyz		  = v0;
		pVertices[0].color.dcolor = PackColor(colV0);
		pVertices[1].xyz		  = v1;
		pVertices[1].color.dcolor = PackColor(colV1);
	}
}

void CRenderAuxGeom::DrawLines(const Vec3* v, uint32 numPoints, const UCol& col, float thickness)
{
}

void CRenderAuxGeom::AddPrimitive(SAuxVertex*& pVertices, uint32 numVertices, RenderPrimitive primitive)
{
	;
	assert(numVertices > 0);
	{
		m_auxPushBuffer.emplace_back(AuxPushBuffer::value_type(numVertices, primitive));
	}
	// get vertex ptr
	AuxVertexBuffer& auxVertexBuffer(m_VB);
	AuxVertexBuffer::size_type oldVBSize(auxVertexBuffer.size());
	auxVertexBuffer.resize(oldVBSize + numVertices);
	pVertices = &auxVertexBuffer[oldVBSize];
}

void CRenderAuxGeom::Flush()
{
	//RSS(gEnv->pRenderer, DEPTH_TEST, false);
	RSS(gEnv->pRenderer, CULL_FACE, false);
	//RSS(gEnv->pRenderer, BLEND, true);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gEnv->pRenderer->UpdateBuffer(m_HardwareVB, m_VB.data(), m_VB.size(), false);
	int offset = 0;
	for (auto& pb : m_auxPushBuffer)
	{
		gEnv->pRenderer->DrawBuffer(m_HardwareVB, nullptr, 0, 0, static_cast<int>(pb.m_primitive), offset, offset + pb.m_numVertices);
		offset += pb.m_numVertices;
	}
	m_VB.resize(0);
	m_auxPushBuffer.resize(0);
}
