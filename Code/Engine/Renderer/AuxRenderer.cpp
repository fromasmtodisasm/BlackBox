#include "pch.hpp"
#include "AuxRenderer.hpp"
#include "D3D/Renderer.h"
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/ConsoleRegistration.h>

#include "Common/Include_HLSL_CPP_Shared.h"

const UINT NUM_MAT = 4;

#include <array>

#define V_RETURN(cond) \
	if (!(cond)) return;

ID3D11DepthStencilState*          CRenderAuxGeom::m_pDSStateLines;

_smart_ptr<ID3D11RasterizerState> g_pRasterizerStateSolid{};
_smart_ptr<ID3D11RasterizerState> g_pRasterizerStateWire{};
_smart_ptr<ID3D11RasterizerState> g_pRasterizerStateForMeshCurrent{};
_smart_ptr<ID3D11RasterizerState> g_pRasterizerStateForZPrePass{};
_smart_ptr<ID3D11BlendState>      m_pBlendState;

int                               CV_r_DrawWirefame = 0;
int                               CV_r_DisableZP    = 1;

// auto BB_VERTEX_FORMAT = VERTEX_FORMAT_P3F_C4B_T2F;
auto                              BB_VERTEX_FORMAT  = VERTEX_FORMAT_P3F_N_T2F;

std::vector<HLSL_MaterialCB>      g_Materials       = {
    {},
    {},
    {},
    {},
};

inline CStateManager& StateManager()
{
	return gcpRendD3D->StateManager().m_StateManager;
}


void CreateBlendState()
{
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));

	BlendState.RenderTarget[0].BlendEnable           = TRUE;
	BlendState.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_pBlendState = GetDevice()->CreateBlendState(BlendState);
}

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR4 Color;
};

D3DXMATRIX  g_MVP;
D3DXMATRIX  g_World;
D3DXMATRIX  g_View;
D3DXMATRIX  g_Projection;
D3DXMATRIX  g_ViewProjection;

ID3DBuffer* g_pPerDrawCB;
ID3DBuffer* g_pMaterialCB;

namespace
{
	inline uint32 PackColor(const UCol& col)
	{
		return (((uint8)(col.bcolor[0]) << 24) +
		        ((uint8)(col.bcolor[1]) << 16) +
		        ((uint8)(col.bcolor[2]) << 8) +
		        ((uint8)(col.bcolor[3])));
	}
} // namespace

HRESULT CRenderAuxGeom::InitCube()
{
	HRESULT hr{};

	m_IllumShader = (CShader*)gRenDev->Sh_Load("illum.Render", 0, 0);
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	// Create the constant buffer
	bd.Usage          = D3D11_USAGE_DEFAULT;
	bd.ByteWidth      = Memory::AlignedSizeCB<HLSL_PerDrawCB>::value;
	bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr                = GetD3DDevice()->CreateBuffer(&bd, NULL, &g_pPerDrawCB);
	if (FAILED(hr))
		return hr;
	bd.ByteWidth = NUM_MAT * Memory::AlignedSizeCB<HLSL_MaterialCB>::value;
	hr           = GetD3DDevice()->CreateBuffer(&bd, NULL, &g_pMaterialCB);
	if (FAILED(hr))
		return hr;

	// VERTEX_FORMAT_P3F_C4B_T2F
	D3D11_INPUT_ELEMENT_DESC layout[] = {
	    {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	    {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	    {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}};
	UINT                  numElements = sizeof(layout) / sizeof(layout[0]);

	// Set up rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroStruct(rasterizerDesc);
	rasterizerDesc.CullMode              = D3D11_CULL_BACK;
	rasterizerDesc.FillMode              = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias             = false;
	rasterizerDesc.DepthBiasClamp        = 0;
	rasterizerDesc.SlopeScaledDepthBias  = 0;
	rasterizerDesc.DepthClipEnable       = true;
	rasterizerDesc.ScissorEnable         = false;
	rasterizerDesc.MultisampleEnable     = true;
	rasterizerDesc.AntialiasedLineEnable = true;

	g_pRasterizerStateSolid = GetDevice()->CreateRasterizerState(rasterizerDesc);
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	g_pRasterizerStateWire = GetDevice()->CreateRasterizerState(rasterizerDesc);
	return S_OK;
}

void CRenderAuxGeom::DrawElementToZBuffer(const SDrawElement& DrawElement)
{
	// NOTE: to avoid matrix transpose need follow specific order of arguments in mul function in HLSL
	HLSL_PerDrawCB cb;
	cb.World = DrawElement.m_Transform;
	cb.MVP   = g_ViewProjection * cb.World;
	cb.MV    = g_View * cb.World;

	m_ZPShader->Bind();
	::GetDeviceContext()->UpdateSubresource(g_pPerDrawCB, 0, nullptr, &cb, sizeof(cb), 0);
	::GetDeviceContext()->VSSetConstantBuffers(PERDRAW_SLOT, 1, &g_pPerDrawCB);
	auto ib         = DrawElement.m_Inices;
	auto numindices = 0;
	if (ib)
	{
		numindices = ib->m_nItems;
	}

	Env::Renderer()->DrawBuffer(DrawElement.m_pBuffer, DrawElement.m_Inices, numindices, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, 0, (CMatInfo*)-1);
}

void CRenderAuxGeom::DrawElement(const SDrawElement& DrawElement)
{
	// Update our time
	static DWORD   dwTimeStart = 0;
	DWORD          dwTimeCur   = GetTickCount();

	// NOTE: to avoid matrix transpose need follow specific order of arguments in mul function in HLSL
	HLSL_PerDrawCB cb;
	cb.World = DrawElement.m_Transform;
	cb.MVP   = g_ViewProjection * cb.World;
	cb.MV    = g_View * cb.World;
	cb.Model = cb.World;

	m_IllumShader->Bind();
	ID3DBuffer* pBuffers[] = {
	    g_pPerDrawCB,
	    g_pMaterialCB,
	};

	auto shader = m_IllumShader;
	auto ds = shader->m_pDepthStencilState;

	if (CV_r_DrawWirefame)
	{
		StateManager().SetRasterizerState(g_pRasterizerStateWire);
	}
	else
	{
		StateManager().SetRasterizerState(shader->m_pRasterizerState);
		//g_pRasterizerStateSolid);
	}

	if (DrawElement.m_Material.m_bZWrite)
	{
		StateManager().SetDepthStencilState(ds);
	}

	//::GetDeviceContext()->OMSetBlendState(m_IllumShader->m_pBlendState, 0, 0xffffffff);
	StateManager().SetBlendState(m_IllumShader->m_pBlendState);
	::GetDeviceContext()->UpdateSubresource(g_pPerDrawCB, 0, nullptr, &cb, sizeof(cb), 0);
	::GetDeviceContext()->VSSetConstantBuffers(PERDRAW_SLOT, 2, pBuffers);
	Env::Renderer()->SetTexture(DrawElement.m_DiffuseMap);
	auto ib         = DrawElement.m_Inices;
	auto numindices = 0;
	if (ib)
	{
		numindices = ib->m_nItems;
	}

	Env::Renderer()->DrawBuffer(DrawElement.m_pBuffer, DrawElement.m_Inices, numindices, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, 0, (CMatInfo*)-1);
}

CRenderAuxGeom::CRenderAuxGeom()
    : m_BoundingBox((_CrtCheckMemory(), nullptr))
    , m_auxPushBuffer((_CrtCheckMemory(), AuxPushBuffer{}))
    , m_VB((_CrtCheckMemory(), AuxVertexBuffer{}))
    , m_HardwareVB((_CrtCheckMemory(), nullptr))
    , m_BoundingBoxShader((_CrtCheckMemory(), nullptr))
    , m_AuxGeomShader((_CrtCheckMemory(), nullptr))

    , m_Images((_CrtCheckMemory(), std::vector<SRender2DImageDescription>{}))
    , m_Meshes((_CrtCheckMemory(), std::vector<SDrawElement>{}))
    , m_BBVerts((_CrtCheckMemory(), std::vector<BoundingBox>{}))

    //, m_IllumShader((_CrtCheckMemory(), nullptr))
    //, m_ZPShader((_CrtCheckMemory(), nullptr))

{
	_CrtCheckMemory();
	CreateBlendState();

	///////////////////////////////////////////////////////////////////////////////
	m_BoundingBox = Env::Renderer()->CreateBuffer(36, BB_VERTEX_FORMAT, "BoundingBox", false);
	///////////////////////////////////////////////////////////////////////////////
	m_HardwareVB  = Env::Renderer()->CreateBuffer(INIT_VB_SIZE, VERTEX_FORMAT_P3F_C4B_T2F, "AuxGeom", true);
	if (!(m_AuxGeomShader = Env::Renderer()->Sh_Load("auxgeom", 0)))
	{
		Env::Log()->Log("Error of loading auxgeom shader");
	}

	REGISTER_CVAR(dbg_mode, 3, 0, "");
	REGISTER_CVAR2("r_stop", &stop, 1, 0, "");

	InitCube();
	// Initialize the world matrix
	D3DXMatrixIdentity(&g_World);

	// Initialize the view matrix
	D3DXVECTOR3 Eye(0.0f, 1.0f, -5.0f);
	D3DXVECTOR3 At(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 Up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&g_View, &Eye, &At, &Up);

	// Initialize the projection matrix
	D3DXMatrixPerspectiveFovLH(&g_Projection, (float)D3DX_PI * 0.5f, Env::Renderer()->GetWidth() / (FLOAT)Env::Renderer()->GetHeight(), 0.1f, 100.0f);

	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroStruct(desc);
	// desc.BackFace
	desc.DepthEnable    = true;
	desc.StencilEnable  = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc      = D3D11_COMPARISON_LESS;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc      = D3D11_COMPARISON_ALWAYS;
	m_pDSStateLines = GetDevice()->CreateDepthStencilState(desc);

	//m_ZPShader = (CShader*)gRenDev->Sh_Load("z.Main", 0, 0);
	REGISTER_CVAR2("r_DrawWirefame", &CV_r_DrawWirefame, 0, 0, "[0/1] Draw in wireframe mode");
	REGISTER_CVAR2("r_DisableZP", &CV_r_DisableZP, 1, 0, "[0/1] off/on zprepass");
}

CRenderAuxGeom::~CRenderAuxGeom()
{
	SAFE_DELETE(m_BoundingBox);
	//SAFE_DELETE(m_BB_IndexBuffer);
	SAFE_DELETE(m_HardwareVB);
}

bool first_draw = true;

// TODO: Довести до ума, нужно учитывать трансформации объекта
void CRenderAuxGeom::DrawAABB(Legacy::Vec3 min, Legacy::Vec3 max, const UCol& col)
{
	// #unreferenced
	// auto& shader = m_BoundingBoxShader;

	const auto angle     = !stop ? static_cast<float>(0.01 * Env::Renderer()->GetFrameID()) : 0.f;
	const auto size      = glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	const auto center    = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	const auto transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	if (first_draw)
	{
#if 0
		m_aabbBufferPtr->Model	  = transform;
		m_aabbBufferPtr->LightPos = Legacy::Vec3(300);
		m_aabbBufferPtr->Update();
#endif
	}

	std::array<BB_VERTEX, 36> verts = {

	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
	    BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

	};
	m_BBVerts.emplace_back(verts);
}
void CRenderAuxGeom::DrawAABBs()
{
	D3DPERF_BeginEvent(D3DC_Blue, L"DrawAABB");
	{
		auto m_Camera    = Env::System()->GetViewCamera();
		g_View           = m_Camera.GetViewMatrix();
		g_Projection     = m_Camera.GetProjectionMatrix();
		g_ViewProjection = g_Projection * g_View;
		// V_RETURN(m_BBVerts.size() > 0 && !m_Meshes.size());
		// m_BoundingBoxShader->Bind();

		StateManager().SetSamplerState(GlobalResources::LinearSampler, 0);
		StateManager().SetBlendState(m_pBlendState);
		if (!m_BBVerts.empty())
		{
			Env::Renderer()->ReleaseBuffer(m_BoundingBox);
			auto size     = m_BBVerts.size() * sizeof BoundingBox;
			m_BoundingBox = Env::Renderer()->CreateBuffer(size, BB_VERTEX_FORMAT, "BoundingBox", false);
			Env::Renderer()->UpdateBuffer(m_BoundingBox, m_BBVerts.data(), size, false);
			DrawElement({m_BoundingBox, nullptr, glm::mat4(1), -1});
		}

		StateManager().SetRasterizerState(g_pRasterizerStateForMeshCurrent);
		{
			D3DPERF_BeginEvent(D3DC_Blue, L"DrawMeshes");
			for (auto const& mesh : m_Meshes)
			{
				DrawElement(mesh);
			}
			D3DPERF_EndEvent();
		}
	}
	D3DPERF_EndEvent();
	m_Meshes.resize(0);
	m_BBVerts.resize(0);
}

void CRenderAuxGeom::DrawLines()
{
	if (!m_auxPushBuffer.empty())
	{
		m_AuxGeomShader->Bind();
		Env::Renderer()->UpdateBuffer(m_HardwareVB, m_VB.data(), m_VB.size(), false);
		int offset = 0;
		StateManager().SetDepthStencilState(m_pDSStateLines);
		for (auto& pb : m_auxPushBuffer)
		{
			Env::Renderer()->DrawBuffer(m_HardwareVB, nullptr, 0, 0, static_cast<int>(pb.m_primitive), offset, offset + pb.m_numVertices);
			offset += pb.m_numVertices;
		}
	}
	m_VB.resize(0);
	m_auxPushBuffer.resize(0);
}

void CRenderAuxGeom::PushImage(const SRender2DImageDescription& image)
{
	auto& i = image;
	Env::Renderer()->Draw2dImage(
	    i.x, i.y, i.w, i.h,
	    i.textureId,
	    i.uv[0].x, i.uv[0].y, i.uv[1].x, i.uv[1].y,
	    0,
	    i.color.r,
	    i.color.g,
	    i.color.b,
	    i.color.a);
}

void CRenderAuxGeom::DrawTriangle(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, const Legacy::Vec3& v2, const UCol& colV2)
{
	SAuxVertex* pVertices(nullptr);
	AddPrimitive(pVertices, 3, RenderPrimitive::TRIANGLES);

	pVertices[0].xyz          = v0;
	pVertices[0].color.dcolor = PackColor(colV0);

	pVertices[1].xyz          = v1;
	pVertices[1].color.dcolor = PackColor(colV1);

	pVertices[2].xyz          = v2;
	pVertices[2].color.dcolor = PackColor(colV2);
}

void CRenderAuxGeom::DrawLine(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, float thickness)
{
	if (thickness <= 1.0f)
	{
		SAuxVertex* pVertices(nullptr);
		AddPrimitive(pVertices, 2, RenderPrimitive::LINES);

		pVertices[0].xyz          = v0;
		pVertices[0].color.dcolor = PackColor(colV0);
		pVertices[1].xyz          = v1;
		pVertices[1].color.dcolor = PackColor(colV1);
	}
}

void CRenderAuxGeom::DrawLines(const Legacy::Vec3* v, uint32 numPoints, const UCol& col, float thickness)
{
	// return;
	if (thickness <= 1.0f)
	{
		SAuxVertex* pVertices(nullptr);
		AddPrimitive(pVertices, numPoints, RenderPrimitive::LINE_STRIP);
		for (size_t i = 0; i < numPoints; i++)
		{
			pVertices[i].xyz          = v[i];
			pVertices[i].color.dcolor = PackColor(col);
		}
	}
}

void CRenderAuxGeom::AddPrimitive(SAuxVertex*& pVertices, uint32 numVertices, RenderPrimitive primitive)
{
	assert(numVertices > 0);
	{
		m_auxPushBuffer.emplace_back(AuxPushBuffer::value_type(numVertices, primitive));
	}
	// get vertex ptr
	AuxVertexBuffer&           auxVertexBuffer(m_VB);
	AuxVertexBuffer::size_type oldVBSize(auxVertexBuffer.size());
	auxVertexBuffer.resize(oldVBSize + numVertices);
	pVertices = &auxVertexBuffer[oldVBSize];
}

void CRenderAuxGeom::DrawMesh(CVertexBuffer* pVertexBuffer, SVertexStream* pIndices, glm::mat4 transform, int texture)
{
	m_Meshes.push_back({pVertexBuffer, pIndices, transform, texture});
}

void CRenderAuxGeom::Flush()
{
	if (CV_r_DrawWirefame)
		g_pRasterizerStateForMeshCurrent = g_pRasterizerStateWire;
	else
		g_pRasterizerStateForMeshCurrent = g_pRasterizerStateSolid;
	//D3DPERF_BeginEvent(D3DC_Blue, L"DrawLines");
	DrawLines();
	//D3DPERF_EndEvent();
	DrawAABBs();
}
