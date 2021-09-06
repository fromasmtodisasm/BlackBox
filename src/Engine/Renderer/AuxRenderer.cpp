#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/IShader.hpp>
//#include <BlackBox/Renderer/BaseShader.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/System/IConsole.hpp>
//#include <BlackBox/Renderer/OpenGL/Core.hpp>

#define V_RETURN(cond) \
	if (!(cond)) return;

#include <d3dx10.h>

ID3D10Device* GetDevice();

using D3DXMATRIX_  = glm::mat4;
using D3DXVECTOR3_ = glm::vec3;
using D3DXVECTOR4_ = glm::vec4;

#define D3DXMATRIX D3DXMATRIX_
#define D3DXVECTOR3 D3DXVECTOR3_
#define D3DXVECTOR4 D3DXVECTOR4_

#define D3DX_PI glm::pi<double>()

inline void D3DXMatrixIdentity_(D3DXMATRIX* mat)
{
	*mat = glm::mat4(1.f);
}

inline void D3DXMatrixRotationY_(D3DXMATRIX* mat, float y)
{
	*mat = glm::rotate(D3DXMATRIX(1.f), y, D3DXVECTOR3(0.f, 1.f, 0.f));
}

inline D3DXMATRIX* D3DXMatrixLookAtLH_(D3DXMATRIX* mat, D3DXVECTOR3* Eye, D3DXVECTOR3* At, D3DXVECTOR3* Up)
{
	*mat = glm::lookAtLH(*Eye, *At, *Up);
	return mat;
}

// Build a perspective projection matrix. (left-handed)
D3DXMATRIX* WINAPI D3DXMatrixPerspectiveFovLH_(D3DXMATRIX* pOut, FLOAT fovy, FLOAT Aspect, FLOAT zn, FLOAT zf)
{
	*pOut = glm::perspectiveLH_ZO(fovy, Aspect, zn, zf);
	return pOut;
}

inline D3DXMATRIX* D3DXMatrixTranspose(D3DXMATRIX* pOut, CONST D3DXMATRIX* pM)
{
	*pOut = glm::transpose(*pM);
	return pOut;
}

#define D3DXMatrixIdentity D3DXMatrixIdentity_
#define D3DXMatrixRotationY D3DXMatrixRotationY_
#define D3DXMatrixLookAtLH D3DXMatrixLookAtLH_
#define D3DXMatrixPerspectiveFovLH D3DXMatrixPerspectiveFovLH_
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
ID3D10EffectTechnique*		g_pTechnique		  = NULL;
ID3D10InputLayout*			g_pVertexLayout		  = NULL;
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

#include <array>

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
	auto hr = D3DX10CreateEffectFromFile("res/shaders/fx/test.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0, GetDevice(), NULL, NULL, &g_pEffect, NULL, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL,
				   "The FX file cannot be located.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
		return hr;
	}

	// Obtain the technique
	g_pTechnique	  = g_pEffect->GetTechniqueByName("Render");
	g_pConstantBuffer = g_pEffect->GetConstantBufferByName("cbChangesEveryFrame");

// Obtain the variables
#	if 1
	g_pMVP				  = g_pEffect->GetVariableByName("MVP")->AsMatrix();
	g_pWorldVariable	  = g_pEffect->GetVariableByName("World")->AsMatrix();
	g_pViewVariable		  = g_pEffect->GetVariableByName("View")->AsMatrix();
	g_pProjectionVariable = g_pEffect->GetVariableByName("Projection")->AsMatrix();
#	endif

	// Define the input layout
	D3D10_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
		};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	// Create the input layout
	D3D10_PASS_DESC PassDesc;
	g_pTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
	hr = GetDevice()->CreateInputLayout(layout, numElements, PassDesc.pIAInputSignature,
										PassDesc.IAInputSignatureSize, &g_pVertexLayout);
	if (FAILED(hr))
		return hr;

	// Set the input layout
	GetDevice()->IASetInputLayout(g_pVertexLayout);

	// Create vertex buffer
	SimpleVertex vertices[] =
		{
			{D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f)},
			{D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f)},
			{D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f)},
		};
	D3D10_BUFFER_DESC bd;
	bd.Usage		  = D3D10_USAGE_DEFAULT;
	bd.ByteWidth	  = sizeof(SimpleVertex) * 8;
	bd.BindFlags	  = D3D10_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags	  = 0;
	D3D10_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	hr				 = GetDevice()->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	// Set vertex buffer
	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	GetDevice()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

	// Create index buffer
	DWORD indices[] =
		{
			3,
			1,
			0,
			2,
			1,
			3,

			0,
			5,
			4,
			1,
			5,
			0,

			3,
			4,
			7,
			0,
			4,
			3,

			1,
			6,
			5,
			2,
			6,
			1,

			2,
			7,
			6,
			3,
			7,
			2,

			6,
			4,
			5,
			7,
			4,
			6,
		};
	bd.Usage		  = D3D10_USAGE_DEFAULT;
	bd.ByteWidth	  = sizeof(DWORD) * 36; // 36 vertices needed for 12 triangles in a triangle list
	bd.BindFlags	  = D3D10_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags	  = 0;
	InitData.pSysMem  = indices;
	hr				  = GetDevice()->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
	if (FAILED(hr))
		return hr;

	// Set index buffer
	GetDevice()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology
	GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#endif

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
	::GetDevice()->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	::GetDevice()->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	::GetDevice()->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//
	// Renders a triangle
	//
	D3D10_TECHNIQUE_DESC techDesc;
	g_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		g_pTechnique->GetPassByIndex(p)->Apply(0);
		::GetDevice()->UpdateSubresource(pEveryFrameBuffer, 0, NULL, &cb, sizeof(cb), 0);
		//GetDevice()->DrawIndexed( 36, 0, 0 );        // 36 vertices needed for 12 triangles in a triangle list
		gEnv->pRenderer->DrawBuffer(m_BoundingBox, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
	}
#endif
}

CRenderAuxGeom::CRenderAuxGeom()
{
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
	m_BoundingBox = gEnv->pRenderer->CreateBuffer(vertices.size(), VERTEX_FORMAT_P3F_C4B, "BoundingBox", false);
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
void CRenderAuxGeom::DrawAABB(Vec3 min, Vec3 max, const UCol& col)
{
	auto& shader = m_BoundingBoxShader;

	const auto angle	 = !stop ? static_cast<float>(0.01 * gEnv->pRenderer->GetFrameID()) : 0.f;
	const auto size		 = glm::vec3(max.x - min.x, max.y - min.y, max.z - min.z);
	const auto center	 = glm::vec3((min.x + max.x) / 2, (min.y + max.y) / 2, (min.z + max.z) / 2);
	const auto transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	if (first_draw)
	{
#if 0
		m_aabbBufferPtr->Model	  = transform;
		m_aabbBufferPtr->LightPos = Vec3(300);
		m_aabbBufferPtr->Update();
#endif
	}

	std::array<BB_VERTEX, 36> verts = {
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, -0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, 0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
		BB_VERTEX{Vec3(transform * Vec4{-0.5f, 0.5f, -0.5f, 1.f}), UCol{Vec4(col.bcolor[0], col.bcolor[1], col.bcolor[2], col.bcolor[3])}},
	};
	m_BBVerts.emplace_back(verts);
}
void CRenderAuxGeom::DrawAABBs()
{
	V_RETURN(m_BBVerts.size() > 0);
	//m_BoundingBoxShader->Bind();
	gEnv->pRenderer->ReleaseBuffer(m_BoundingBox);
	auto size	  = m_BBVerts.size() * 36;
	m_BoundingBox = gEnv->pRenderer->CreateBuffer(size, VERTEX_FORMAT_P3F_C4B, "BoundingBox", false);
	gEnv->pRenderer->UpdateBuffer(m_BoundingBox, m_BBVerts.data(), size, false);
	DrawCube(m_BoundingBox);
	//gEnv->pRenderer->DrawBuffer(m_BoundingBox, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES));

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
	AuxVertexBuffer&		   auxVertexBuffer(m_VB);
	AuxVertexBuffer::size_type oldVBSize(auxVertexBuffer.size());
	auxVertexBuffer.resize(oldVBSize + numVertices);
	pVertices = &auxVertexBuffer[oldVBSize];
}

void CRenderAuxGeom::Flush()
{
	//RSS(gEnv->pRenderer, CULL_FACE, false);
	DrawAABBs();
	DrawLines();
	{
		//RSS(gEnv->pRenderer, DEPTH_TEST, true);
	}
	//first_draw = true;
}
