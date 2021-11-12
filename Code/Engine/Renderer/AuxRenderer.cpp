#include "D3D/Renderer.h"
#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/ConsoleRegistration.h>

#include <array>

#define V_RETURN(cond) \
	if (!(cond)) return;

ID3D11DepthStencilState* CRenderAuxGeom::m_pDSState;
ID3D11RasterizerState*	 g_pRasterizerStateSolid{};
ID3D11RasterizerState*	 g_pRasterizerStateWire{};
ID3D11RasterizerState*	 g_pRasterizerStateForMeshCurrent{};
ID3D11BlendState*		 m_pBlendState;

int CV_r_DrawWirefame;

// auto BB_VERTEX_FORMAT = VERTEX_FORMAT_P3F_C4B_T2F;
auto BB_VERTEX_FORMAT = VERTEX_FORMAT_P3F_N_T2F;

void CreateBlendState()
{
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));

	BlendState.RenderTarget[0].BlendEnable			 = TRUE;
	BlendState.RenderTarget[0].SrcBlend				 = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend			 = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp				 = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha		 = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha		 = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	GetDevice()->CreateBlendState(&BlendState, &m_pBlendState);
}

//--------------------------------------------------------------------------------------
// Structures
//--------------------------------------------------------------------------------------
struct SimpleVertex
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR4 Color;
};

struct CBChangesEveryFrame
{
	D3DXMATRIX World;
	D3DXMATRIX MVP;
	bool	   ApplyGrayScale;
};

ID3D10Effect*	   g_pEffect	   = nullptr;
ID3D11InputLayout* g_pVertexLayout = nullptr;
D3DXMATRIX		   g_MVP;
D3DXMATRIX		   g_World;
D3DXMATRIX		   g_View;
D3DXMATRIX		   g_Projection;
D3DXMATRIX		   g_ViewProjection;

ID3DBuffer* g_pConstantBuffer;

CShader* g_pShader{};

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

HRESULT InitCube()
{
	// Create the effect
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

#ifndef VK_RENDERER
	HRESULT hr{};

	g_pShader = (CShader*)gRenDev->Sh_Load("test.Render", 0, 0);
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	// Create the constant buffer
	bd.Usage		  = D3D11_USAGE_DEFAULT;
	bd.ByteWidth	  = Memory::AlignedSizeCB<CBChangesEveryFrame>::value;
	bd.BindFlags	  = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr				  = GetDevice()->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
	if (FAILED(hr))
		return hr;

	// VERTEX_FORMAT_P3F_C4B_T2F
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

#endif
	// Set up rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroStruct(rasterizerDesc);
	rasterizerDesc.CullMode				 = D3D11_CULL_BACK;
	rasterizerDesc.FillMode				 = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthBias			 = false;
	rasterizerDesc.DepthBiasClamp		 = 0;
	rasterizerDesc.SlopeScaledDepthBias	 = 0;
	rasterizerDesc.DepthClipEnable		 = true;
	rasterizerDesc.ScissorEnable		 = false;
	rasterizerDesc.MultisampleEnable	 = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	GetDevice()->CreateRasterizerState(&rasterizerDesc, &g_pRasterizerStateSolid);
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	GetDevice()->CreateRasterizerState(&rasterizerDesc, &g_pRasterizerStateWire);
	return S_OK;
}

void DrawCube(const SDrawElement& DrawElement)
{
	// Update our time
	static DWORD dwTimeStart = 0;
	DWORD		 dwTimeCur	 = GetTickCount();

	// NOTE: to avoid matrix transpose need follow specific order of arguments in mul function in HLSL
	CBChangesEveryFrame cb;
	cb.World		  = DrawElement.transform;
	cb.MVP		  = g_ViewProjection * cb.World;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;


	g_pShader->Bind();
	::GetDeviceContext()->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, sizeof(cb), 0);
	::GetDeviceContext()->VSSetConstantBuffers(2, 1, &g_pConstantBuffer);
	gEnv->pRenderer->SetTexture(DrawElement.m_DiffuseMap);
	auto ib			= DrawElement.m_Inices;
	auto numindices = 0;
	if (ib)
	{
		numindices = ib->m_nItems;
	}

	gEnv->pRenderer->DrawBuffer(DrawElement.m_pBuffer, DrawElement.m_Inices, numindices, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, 0, (CMatInfo*)-1);
}

CRenderAuxGeom::CRenderAuxGeom()
{
	CreateBlendState();
	std::vector<BB_VERTEX> reference = {

// front
#if 0
		BB_VERTEX{{-1.0, -1.0, 1.0}, {0, 0, 0}},
		BB_VERTEX{{1.0, -1.0, 1.0}, {0, 0, 0}},
		BB_VERTEX{{1.0, 1.0, 1.0}, {0, 0, 0}},
		BB_VERTEX{{-1.0, 1.0, 1.0}, {0, 0, 0}},
		// back
		BB_VERTEX{{-1.0, -1.0, -1.0}, {0, 0, 0}},
		BB_VERTEX{{1.0, -1.0, -1.0}, {0, 0, 0}},
		BB_VERTEX{{1.0, 1.0, -1.0}, {0, 0, 0}},
		BB_VERTEX{{-1.0, 1.0, -1.}, {0, 0, 0}}
#endif
	};

	std::vector<BB_VERTEX> vertices(24);
	auto				   get_idx = [&](glm::vec3 pos, glm::vec3 n) -> uint16
	{
		int idx = 0;
#if 0
		for (int i = 0; i < 24; i++)
		{
			if (vertices[i].xyz == pos && vertices[i].normal == n)
			{
				idx = i;
				break;
			}
		}
#endif
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
	};
#endif
#if 0
	for (int i = 0, j = 0; i < reference_elements.size(); i += 2, j += 4)
	{
		auto n			= glm::normalize(glm::cross(
			 reference[reference_elements[i][1]].xyz - reference[reference_elements[i][0]].xyz,
			 reference[reference_elements[i][2]].xyz - reference[reference_elements[i][1]].xyz));
		vertices[j]		= BB_VERTEX{reference[reference_elements[i][0]].xyz, n};
		vertices[j + 1] = BB_VERTEX{reference[reference_elements[i][1]].xyz, n};
		vertices[j + 2] = BB_VERTEX{reference[reference_elements[i][2]].xyz, n};
		vertices[j + 3] = BB_VERTEX{reference[reference_elements[i + 1][1]].xyz, n};
	}
#endif
		std::vector<glm::u16vec3> elements(12);
	std::cout << "elments:" << std::endl;
#if 0
	for (int i = 0; i < 12; i++)
	{
		elements[i] = {
			(get_idx(reference[reference_elements[i][0]].xyz, vertices[2 * i].normal)),
			(get_idx(reference[reference_elements[i][1]].xyz, vertices[2 * i].normal)),
			(get_idx(reference[reference_elements[i][2]].xyz, vertices[2 * i].normal))};
		std::cout << elements[i][0] << ", " << elements[i][1] << ", " << elements[i][2] << std::endl;
	}
	std::cout << std::endl;
	for (int i = 0; i < 24; i++)
	{
		vertices[i].xyz *= 0.5;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////
	// int cnt		  = sizeof vertices / sizeof BB_VERTEX;
	m_BoundingBox = gEnv->pRenderer->CreateBuffer(vertices.size(), BB_VERTEX_FORMAT, "BoundingBox", false);
	gEnv->pRenderer->UpdateBuffer(m_BoundingBox, vertices.data(), vertices.size(), false);

	m_BB_IndexBuffer = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_BB_IndexBuffer, elements.data(), (3 * elements.size()));
	///////////////////////////////////////////////////////////////////////////////
	m_HardwareVB		= gEnv->pRenderer->CreateBuffer(INIT_VB_SIZE, VERTEX_FORMAT_P3F_C4B_T2F, "AuxGeom", true);
	m_BoundingBoxShader = gEnv->pRenderer->Sh_Load("bb", 0);
	if (!(m_AuxGeomShader = gEnv->pRenderer->Sh_Load("auxgeom", 0)))
	{
		gEnv->pLog->Log("Error of loading auxgeom shader");
	}

	// m_aabbBufferPtr = SAABBBuffer::Create(10);

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
	D3DXMatrixPerspectiveFovLH(&g_Projection, (float)D3DX_PI * 0.5f, gEnv->pRenderer->GetWidth() / (FLOAT)gEnv->pRenderer->GetHeight(), 0.1f, 100.0f);

	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroStruct(desc);
	// desc.BackFace
	desc.DepthEnable	= true;
	desc.StencilEnable	= false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc		= D3D11_COMPARISON_LESS;

	GetDevice()->CreateDepthStencilState(&desc, &m_pDSState);

	REGISTER_CVAR2("r_DrawWirefame", &CV_r_DrawWirefame, 0, 0, "[0/1] Draw in wireframe mode");
}

CRenderAuxGeom::~CRenderAuxGeom()
{
	SAFE_DELETE(m_BoundingBox);
	SAFE_DELETE(m_BB_IndexBuffer);
	SAFE_DELETE(m_HardwareVB);
}

bool first_draw = true;

// TODO: Довести до ума, нужно учитывать трансформации объекта
void CRenderAuxGeom::DrawAABB(Legacy::Vec3 min, Legacy::Vec3 max, const UCol& col)
{
	// #unreferenced
	// auto& shader = m_BoundingBoxShader;

	const auto angle	 = !stop ? static_cast<float>(0.01 * gEnv->pRenderer->GetFrameID()) : 0.f;
	const auto size		 = glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	const auto center	 = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
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

#if 0
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	UCol{Legacy::Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}, Legacy::Vec2{0, 0}},
#else
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
#endif

	};
	m_BBVerts.emplace_back(verts);
}
void CRenderAuxGeom::DrawAABBs()
{
	auto m_Camera	 = gEnv->pSystem->GetViewCamera();
	g_View			 = m_Camera.GetViewMatrix();
	g_Projection	 = m_Camera.GetProjectionMatrix();
	g_ViewProjection = g_Projection * g_View;
	// V_RETURN(m_BBVerts.size() > 0 && !m_Meshes.size());
	// m_BoundingBoxShader->Bind();

	::GetDeviceContext()->PSSetSamplers(0, 1, &GlobalResources::LinearSampler);
	::GetDeviceContext()->OMSetDepthStencilState(CRenderAuxGeom::m_pDSState, 0);
	::GetDeviceContext()->RSSetState(g_pRasterizerStateWire);
	::GetDeviceContext()->OMSetBlendState(m_pBlendState, 0, 0xffffffff);
	if (!m_BBVerts.empty())
	{
		gEnv->pRenderer->ReleaseBuffer(m_BoundingBox);
		auto size	  = m_BBVerts.size() * 36;
		m_BoundingBox = gEnv->pRenderer->CreateBuffer(size, BB_VERTEX_FORMAT, "BoundingBox", false);
		gEnv->pRenderer->UpdateBuffer(m_BoundingBox, m_BBVerts.data(), size, false);
		DrawCube({m_BoundingBox, nullptr, glm::mat4(1), -1});
	}

	::GetDeviceContext()->RSSetState(g_pRasterizerStateForMeshCurrent);
	for (auto const& mesh : m_Meshes)
	{
		DrawCube(mesh);
	}

	m_Meshes.resize(0);
	m_BBVerts.resize(0);
}

void CRenderAuxGeom::DrawLines()
{
	// m_AuxGeomShader->Bind();
	gEnv->pRenderer->UpdateBuffer(m_HardwareVB, m_VB.data(), m_VB.size(), false);
	int offset = 0;
	for (auto& pb : m_auxPushBuffer)
	{
		gEnv->pRenderer->DrawBuffer(m_HardwareVB, nullptr, 0, 0, static_cast<int>(pb.m_primitive), offset, offset + pb.m_numVertices);
		offset += pb.m_numVertices;
	}
	// m_AuxGeomShader->Unuse();
	m_VB.resize(0);
	m_auxPushBuffer.resize(0);
}

void CRenderAuxGeom::PushImage(const SRender2DImageDescription& image)
{
	auto& i = image;
	gEnv->pRenderer->Draw2dImage(
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

	pVertices[0].xyz		  = v0;
	pVertices[0].color.dcolor = PackColor(colV0);

	pVertices[1].xyz		  = v1;
	pVertices[1].color.dcolor = PackColor(colV1);

	pVertices[2].xyz		  = v2;
	pVertices[2].color.dcolor = PackColor(colV2);
}

void CRenderAuxGeom::DrawLine(const Legacy::Vec3& v0, const UCol& colV0, const Legacy::Vec3& v1, const UCol& colV1, float thickness)
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

void CRenderAuxGeom::DrawLines(const Legacy::Vec3* v, uint32 numPoints, const UCol& col, float thickness)
{
	// return;
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
	assert(numVertices > 0);
	{
		m_auxPushBuffer.emplace_back(AuxPushBuffer::value_type(numVertices, primitive));
	}
	// get vertex ptr
	AuxVertexBuffer&		   auxVertexBuffer(m_VB);
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
	DrawAABBs();
	DrawLines();
}
