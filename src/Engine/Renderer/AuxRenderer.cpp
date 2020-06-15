#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>

using P3F	= SVF_P3F_N;
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
	std::vector<P3F> reference = {
		
    // front
		P3F{{-1.0, -1.0, 1.0},{0, 0, 0}},
		P3F{{1.0, -1.0,  1.0},{0,0,0}},
		P3F{{1.0,  1.0,  1.0},{0,0,0}},
    P3F{{-1.0,  1.0,  1.0},{0,0,0}},
    // back
    P3F{{-1.0, -1.0, -1.0},{0,0,0}},
    P3F{{1.0, -1.0, -1.0},{0,0,0}},
    P3F{{1.0,  1.0, -1.0},{0,0,0}},
    P3F{{-1.0,  1.0, -1.},{0,0,0}}
	};


	std::vector<P3F> vertices(24);
	auto get_idx = [&](glm::vec3 pos, glm::vec3 n) -> uint16 {
		int idx = 0;
		for (int i = 0; i < 24; i++)
		{
			if (vertices[i].xyz == pos && vertices[i].normal == n)
			{
				idx = i;	
				break;
			}
		}
		return idx;
	};
	
	std::vector<glm::u16vec3> reference_elements = {
#if 0
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
		// front
		{0, 1, 2},
		{2, 3, 0},
		// right
		{1, 5, 6},
		{6, 2, 1},
		// back
		{7, 6, 5},
		{5, 4, 7},
		// left
		{4, 0, 3},
		{3, 7, 4},
		// bottom
		{4, 5, 1},
		{1, 0, 4},
		// top
		{3, 2, 6},
		{6, 7, 3}
#endif
	};
	for (int i = 0, j = 0; i < reference_elements.size(); i+=2, j+=4)
	{
		auto n = glm::normalize(glm::cross(
			reference[reference_elements[i][1]].xyz - reference[reference_elements[i][0]].xyz, 
			reference[reference_elements[i][2]].xyz - reference[reference_elements[i][1]].xyz)
		);
		vertices[j		]	= P3F{reference[reference_elements[i][0]].xyz, n};
		vertices[j + 1] = P3F{reference[reference_elements[i][1]].xyz, n};
		vertices[j + 2] = P3F{reference[reference_elements[i][2]].xyz, n};
		vertices[j + 3] = P3F{reference[reference_elements[i + 1][1]].xyz, n};
	}
	std::vector<glm::u16vec3> elements(12);
	std::cout << "elments:" << std::endl;
	for (int i = 0; i < 12; i++)
	{
		elements[i] = {
			(get_idx(reference[reference_elements[i][0]].xyz, vertices[2 * i].normal)),
			(get_idx(reference[reference_elements[i][1]].xyz, vertices[2 * i].normal)),
			(get_idx(reference[reference_elements[i][2]].xyz, vertices[2 * i].normal))
		};
		std::cout << elements[i][0] << ", " << elements[i][1] << ", " << elements[i][2] << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < 24; i++)
	{
		vertices[i].xyz *= 0.5;		
	}
	///////////////////////////////////////////////////////////////////////////////
	//int cnt		  = sizeof vertices / sizeof P3F;
	m_BoundingBox = gEnv->pRenderer->CreateBuffer(vertices.size(), VERTEX_FORMAT_P3F_N, "BoundingBox", false);
	gEnv->pRenderer->UpdateBuffer(m_BoundingBox, vertices.data(), vertices.size(), false);

	m_BB_IndexBuffer = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_BB_IndexBuffer, elements.data(), (3 * elements.size()));
	///////////////////////////////////////////////////////////////////////////////
	m_HardwareVB		= gEnv->pRenderer->CreateBuffer(INIT_VB_SIZE, VERTEX_FORMAT_P3F_C4B_T2F, "AuxGeom", true);
	m_BoundingBoxShader = gEnv->pRenderer->Sh_Load("bb.vs", "bb.frag");
}

CRenderAuxGeom::~CRenderAuxGeom()
{
	SAFE_DELETE(m_BoundingBox);
	SAFE_DELETE(m_BB_IndexBuffer);
	SAFE_DELETE(m_HardwareVB);
}

//TODO: Довести до ума, нужно учитывать трансформации объекта
void CRenderAuxGeom::DrawAABB(Vec3 min, Vec3 max, const UCol& col)
{
	auto& shader = m_BoundingBoxShader;

	glm::vec3 size		= glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	glm::vec3 center	= glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	const auto& cam = gEnv->pRenderer->GetCamera();
	Vec4 color		= Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3]);
	shader->Use();
	shader->Uniform(transform, "model");
	shader->Uniform(cam.GetViewMatrix(), "view");
	shader->Uniform(cam.getProjectionMatrix(), "projection");
	shader->Uniform(0.1f, "alpha");
	shader->Uniform(color, "color");
	shader->Uniform(gEnv->pConsole->GetCVar("r_Tonemap")->GetIVal(), "bTonemap");
	shader->Uniform(Vec3(300), "lightPos");
	shader->Uniform(gEnv->pRenderer->GetFrameID(), "fid");
	shader->Uniform(gEnv->pSystem->GetViewCamera().GetPos(), "eye");

	{
		RSS(gEnv->pRenderer, BLEND, true);
		RSS(gEnv->pRenderer, CULL_FACE, false);
		#if 0
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 0, static_cast<int>(RenderPrimitive::LINE_LOOP));
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 4, 4, static_cast<int>(RenderPrimitive::LINE_LOOP));
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, 8, 8, static_cast<int>(RenderPrimitive::LINES));
		#else

		gEnv->pRenderer->DrawBuffer(m_BoundingBox, m_BB_IndexBuffer, m_BB_IndexBuffer->m_nItems, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
		#endif
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
	if (thickness <= 1.0f)
	{
		SAuxVertex* pVertices(nullptr);
		AddPrimitive(pVertices, numPoints, RenderPrimitive::LINE_STRIP);
		for (size_t i = 0; i < numPoints; i++)
		{
			pVertices[i].xyz		  = v[i];
			pVertices[i].color.dcolor = PackColor(col);
		}
	}
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
