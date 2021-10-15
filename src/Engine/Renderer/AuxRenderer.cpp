#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/IShader.hpp>
#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/System/IConsole.hpp>

#include <array>

#define V_RETURN(cond) \
	if (!(cond)) return;

ID3D10Device* GetDevice();
ID3D10DepthStencilState* CRenderAuxGeom::m_pDSState;
ID3D10RasterizerState*	 g_pRasterizerState{};
ID3D10BlendState* m_pBlendState;

//auto BB_VERTEX_FORMAT = VERTEX_FORMAT_P3F_C4B_T2F;
auto BB_VERTEX_FORMAT = VERTEX_FORMAT_P3F_N_T2F;

void CreateBlendState()
{

	D3D10_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));

	BlendState.BlendEnable[0]			= TRUE;
	BlendState.SrcBlend					= D3D10_BLEND_SRC_ALPHA;
	BlendState.DestBlend				= D3D10_BLEND_INV_SRC_ALPHA;
	BlendState.BlendOp					= D3D10_BLEND_OP_ADD;
	BlendState.SrcBlendAlpha			= D3D10_BLEND_ONE;
	BlendState.DestBlendAlpha			= D3D10_BLEND_ZERO;
	BlendState.BlendOpAlpha				= D3D10_BLEND_OP_ADD;
	BlendState.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

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
	D3DXMATRIX View;
	D3DXMATRIX Projection;
	D3DXMATRIX MVP;
};

ID3D10RenderTargetView*		g_pRenderTargetView	  = NULL;
ID3D10Effect*				g_pEffect			  = NULL;
ID3D10InputLayout*			g_pVertexLayout		  = NULL;
ID3D10InputLayout*			g_pVertexLayoutMesh	  = NULL;
ID3D10Buffer*				g_pVertexBuffer		  = NULL;
ID3D10Buffer*				g_pIndexBuffer		  = NULL;
ID3D10EffectMatrixVariable* g_pMVP				  = NULL;
ID3D10EffectMatrixVariable* g_pWorldVariable	  = NULL;
ID3D10EffectMatrixVariable* g_pViewVariable		  = NULL;
ID3D10EffectMatrixVariable* g_pProjectionVariable = NULL;
D3DXMATRIX					g_MVP;
D3DXMATRIX					g_World;
D3DXMATRIX					g_View;
D3DXMATRIX					g_Projection;

ID3D10EffectConstantBuffer* g_pConstantBuffer;

using VecPos = std::vector<BB_VERTEX>;

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
	ID3D10Blob* pErrors;
	auto hr = D3DX10CreateEffectFromFile("res/shaders/fx/test.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0, GetDevice(), NULL, NULL, &g_pEffect, &pErrors, NULL);
	if (FAILED(hr))
	{
		CryFatalError("D3DFX: %s", pErrors->GetBufferPointer());
		MessageBox(NULL,
				   "The FX file cannot be located.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Obtain the technique
	GlobalResources::BoxTechnique = g_pEffect->GetTechniqueByName("Render");
	g_pConstantBuffer			  = g_pEffect->GetConstantBufferByName("cbChangesEveryFrame");

// Obtain the variables
#	if 1
	g_pMVP				  = g_pEffect->GetVariableByName("MVP")->AsMatrix();
	g_pWorldVariable	  = g_pEffect->GetVariableByName("World")->AsMatrix();
	g_pViewVariable		  = g_pEffect->GetVariableByName("View")->AsMatrix();
	g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();
#	endif

	#if 0
	// Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		};
	#endif

	//VERTEX_FORMAT_P3F_C4B_T2F
	D3D10_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D10_INPUT_PER_VERTEX_DATA, 0}
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	GlobalResources::BoxTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	hr = GetDevice()->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
										PassDesc.IAInputSignatureSize, &g_pVertexLayout);
	if (FAILED(hr))
		return hr;
#endif
    // Set up rasterizer
	D3D10_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode				 = D3D10_CULL_NONE;
	rasterizerDesc.FillMode				 = D3D10_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias			 = false;
	rasterizerDesc.DepthBiasClamp		 = 0;
	rasterizerDesc.SlopeScaledDepthBias	 = 0;
	rasterizerDesc.DepthClipEnable		 = true;
	rasterizerDesc.ScissorEnable		 = false;
	rasterizerDesc.MultisampleEnable	 = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	GetDevice()->CreateRasterizerState(&rasterizerDesc, &g_pRasterizerState);
	return S_OK;
}

void DrawCube(CVertexBuffer* m_BoundingBox)
{
#ifndef VK_RENDERER
	// Update our time
	static float t			 = 0.0f;
	static DWORD dwTimeStart = 0;
	DWORD		 dwTimeCur	 = GetTickCount();
	if (dwTimeStart == 0)
		dwTimeStart = dwTimeCur;
	t = (dwTimeCur - dwTimeStart) / 1000.0f;

	//
	// Animate the cube
	//
	D3DXMatrixRotationY(&g_World, t);
	//g_World = glm::rotate(glm::mat4(1), t, glm::vec3(0, 1, 0));

	//
	// Update variables
	//
	//D3DXMatrixMultiply(&g_MVP, D3DXMatrixMultiply(&g_MVP, &g_World, &g_View), &g_Projection);
	//g_MVP = g_World * g_View * g_Projection;
	//g_MVP = g_Projection * g_View * g_World;
	auto m_Camera = gEnv->pSystem->GetViewCamera();
	g_MVP		  = m_Camera.getProjectionMatrix() * m_Camera.GetViewMatrix() * g_World;
	g_View		  = m_Camera.GetViewMatrix();
	g_Projection  = m_Camera.getProjectionMatrix();
#	if 1
//ng_pMVP->SetMatrix( ( float* )&g_MVP );
//g_pWorldVariable->SetMatrix( ( float* )&g_World );
//g_pViewVariable->SetMatrix( ( float* )&g_View );
//g_pProjectionVariable->SetMatrix( ( float* )&g_Projection );
#	endif
	ID3D10Buffer* pEveryFrameBuffer;
	g_pConstantBuffer->GetConstantBuffer(&pEveryFrameBuffer);
	CBChangesEveryFrame cb;
#	if 0
	cb.World	  = g_World;
	cb.View		  = g_View;
	cb.Projection = g_Projection;
#	else
	//D3DXMatrixTranspose(&cb.World, &g_World);
	D3DXMatrixTranspose(&cb.View, &g_View);
	D3DXMatrixTranspose(&cb.Projection, &g_Projection);
#	endif
	//cb.MVP		  = g_MVP;
	D3DXMatrixTranspose(&cb.MVP, &g_MVP);
	//::GetDevice()->UpdateSubresource(pEveryFrameBuffer, 0, NULL, &cb, 192, 0);

	//g_pConstantBuffer->SetRawValue();

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	//
	// Renders a triangle
	//
	D3D10_TECHNIQUE_DESC techDesc;
	GlobalResources::BoxTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		GlobalResources::BoxTechnique->GetPassByIndex(p)->Apply(0);
		::GetDevice()->UpdateSubresource(pEveryFrameBuffer, 0, NULL, &cb, sizeof(cb), 0);

		//::GetDevice()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
		::GetDevice()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		::GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		::GetDevice()->IASetInputLayout(g_pVertexLayout);

		::GetDevice()->PSSetShaderResources(0, 1, &GlobalResources::GreyTextureRV);
		::GetDevice()->PSSetSamplers(0, 1, &GlobalResources::LinearSampler);
		::GetDevice()->OMSetDepthStencilState(CRenderAuxGeom::m_pDSState, 0);
		GetDevice()->RSSetState(g_pRasterizerState);
		GetDevice()->OMSetBlendState(m_pBlendState, 0, 0xffffffff);
		//GetDevice()->DrawIndexed( 36, 0, 0 );        // 36 vertices needed for 12 triangles in a triangle list
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
	}
#endif
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
	//int cnt		  = sizeof vertices / sizeof BB_VERTEX;
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

	//m_aabbBufferPtr = SAABBBuffer::Create(10);

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


	D3D10_DEPTH_STENCIL_DESC desc;
	//desc.BackFace
	desc.DepthEnable = true;
	desc.StencilEnable = false;
	desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;

	GetDevice()->CreateDepthStencilState(&desc, &m_pDSState);
}

CRenderAuxGeom::~CRenderAuxGeom()
{
	SAFE_DELETE(m_BoundingBox);
	SAFE_DELETE(m_BB_IndexBuffer);
	SAFE_DELETE(m_HardwareVB);
}

struct AABBInstanceData
{
};

bool first_draw = true;

//TODO: Довести до ума, нужно учитывать трансформации объекта
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
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, -0.5f, -0.5f, 1.f}), Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},

		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 0}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{1, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, 0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 1}},
		BB_VERTEX{Legacy::Vec3(transform * Legacy::Vec4{-0.5f, 0.5f, -0.5f, 1.f}),	Legacy::Vec3{1, 0, 0}, Legacy::Vec2{0, 0}},
		#endif



	};
	m_BBVerts.emplace_back(verts);
}
void CRenderAuxGeom::DrawAABBs()
{
	V_RETURN(m_BBVerts.size() > 0);
	//m_BoundingBoxShader->Bind();
	gEnv->pRenderer->ReleaseBuffer(m_BoundingBox);
	auto size	  = m_BBVerts.size() * 36;
	m_BoundingBox = gEnv->pRenderer->CreateBuffer(size, BB_VERTEX_FORMAT, "BoundingBox", false);
	gEnv->pRenderer->UpdateBuffer(m_BoundingBox, m_BBVerts.data(), size, false);
	#if 1
	if (m_Meshes.size())
	{
		//ng_pMVP->SetMatrix( ( float* )&g_MVP );
		//g_pWorldVariable->SetMatrix( ( float* )&g_World );
		//g_pViewVariable->SetMatrix( ( float* )&g_View );
		//g_pProjectionVariable->SetMatrix( ( float* )&g_Projection );
		DrawCube(m_Meshes[0]);
	}
	#endif
	DrawCube(m_BoundingBox);

	m_Meshes.resize(0);
	m_BBVerts.resize(0);
}

void CRenderAuxGeom::DrawLines()
{
	//m_AuxGeomShader->Bind();
	gEnv->pRenderer->UpdateBuffer(m_HardwareVB, m_VB.data(), m_VB.size(), false);
	int offset = 0;
	for (auto& pb : m_auxPushBuffer)
	{
		gEnv->pRenderer->DrawBuffer(m_HardwareVB, nullptr, 0, 0, static_cast<int>(pb.m_primitive), offset, offset + pb.m_numVertices);
		offset += pb.m_numVertices;
	}
	//m_AuxGeomShader->Unuse();
	m_VB.resize(0);
	m_auxPushBuffer.resize(0);
}

void CRenderAuxGeom::PushImage(const SRender2DImageDescription& image)
{
	auto &i = image;
	gEnv->pRenderer->Draw2dImage(
		i.x, i.y, i.w, i.h,
		i.textureId,
		i.uv[0].x, i.uv[0].y, i.uv[1].x, i.uv[1].y,
		0,
		i.color.r,
		i.color.g,
		i.color.b,
		i.color.a
	);
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
	//return;
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
	AuxVertexBuffer&		   auxVertexBuffer(m_VB);
	AuxVertexBuffer::size_type oldVBSize(auxVertexBuffer.size());
	auxVertexBuffer.resize(oldVBSize + numVertices);
	pVertices = &auxVertexBuffer[oldVBSize];
}

void CRenderAuxGeom::DrawMesh(CVertexBuffer* pVertexBuffer)
{
	m_Meshes.push_back(pVertexBuffer);
}

void CRenderAuxGeom::Flush()
{
	//RSS(gEnv->pRenderer, CULL_FACE, false);
	DrawAABBs();
	DrawLines();
	{
		//RSS(gEnv->pRenderer, DEPTH_TEST, true);
	}

	for (auto img : m_Images)
	{
		img.angle;
		D3D10_TEXTURE2D_DESC desc;
		desc.Width;
		desc.Height;
		desc.MipLevels;
		desc.ArraySize;
		desc.Format;
		desc.SampleDesc;
		desc.Usage;
		desc.BindFlags;
		desc.CPUAccessFlags;
		desc.MiscFlags;

		//D3D10_SUBRESOURCE_DATA sd;
		//sd.

		//GetDevice()->CreateTexture2D(&desc, )
	}
	//first_draw = true;
}
