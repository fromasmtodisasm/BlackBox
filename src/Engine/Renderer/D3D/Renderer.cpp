#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include "Renderer.h"

#include <BlackBox/System/IWindow.hpp>

#include <BlackBox/Core/Platform/platform_impl.inl>

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
};

struct ITechniqueManager;

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3D10_DRIVER_TYPE g_driverType = D3D10_DRIVER_TYPE_NULL;
CD3DRenderer* gD3DRender;

ID3D10Effect*               g_pEffect = NULL;
ID3D10EffectTechnique*      g_pTechnique = NULL;
ID3D10InputLayout*          g_pVertexLayout = NULL;
ID3D10Buffer*               g_pVertexBuffer = NULL;
ID3D10Buffer*               g_pIndexBuffer = NULL;
ID3D10Buffer*               g_pCBChangesEveryFrame = NULL;
ID3D10EffectMatrixVariable* g_pWorldVariable = NULL;
ID3D10EffectMatrixVariable* g_pViewVariable = NULL;
ID3D10EffectMatrixVariable* g_pProjectionVariable = NULL;
CShader*                    g_pCubeShader = NULL;

D3DXMATRIX                  g_World;
D3DXMATRIX                  g_View;
D3DXMATRIX                  g_Projection;

HRESULT CD3DRenderer::InitCube()
{
    // Create the effect
    DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3D10_SHADER_DEBUG;
    #endif

    auto hr = D3DX10CreateEffectFromFile( "res/shaders/fx/test.fx", NULL, NULL, "fx_4_0", dwShaderFlags, 0, m_pd3dDevice, NULL,
                                         NULL, &g_pEffect, NULL, NULL );
    if( FAILED( hr ) )
    {
        MessageBox( NULL,
                    "The FX file cannot be located.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
        return hr;
    }

    // Obtain the technique
    g_pTechnique = g_pEffect->GetTechniqueByName( "Render" );

    // Obtain the variables
    g_pWorldVariable = g_pEffect->GetVariableByName( "World" )->AsMatrix();
    g_pViewVariable = g_pEffect->GetVariableByName( "View" )->AsMatrix();
    g_pProjectionVariable = g_pEffect->GetVariableByName( "Projection" )->AsMatrix();

    // Define the input layout
    D3D10_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 },
    };
    UINT numElements = sizeof( layout ) / sizeof( layout[0] );

    // Create the input layout
    D3D10_PASS_DESC PassDesc;
    g_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
    hr = m_pd3dDevice->CreateInputLayout( layout, numElements, PassDesc.pIAInputSignature,
                                          PassDesc.IAInputSignatureSize, &g_pVertexLayout );
    if( FAILED( hr ) )
        return hr;

    // Set the input layout
    m_pd3dDevice->IASetInputLayout( g_pVertexLayout );

    // Create vertex buffer
    SimpleVertex vertices[] =
    {
        { D3DXVECTOR3( -1.0f, 1.0f, -1.0f ), D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, -1.0f ), D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 1.0f ) },
        { D3DXVECTOR3( 1.0f, 1.0f, 1.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, 1.0f, 1.0f ), D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, -1.0f, -1.0f ), D3DXVECTOR4( 1.0f, 0.0f, 1.0f, 1.0f ) },
        { D3DXVECTOR3( 1.0f, -1.0f, -1.0f ), D3DXVECTOR4( 1.0f, 1.0f, 0.0f, 1.0f ) },
        { D3DXVECTOR3( 1.0f, -1.0f, 1.0f ), D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) },
        { D3DXVECTOR3( -1.0f, -1.0f, 1.0f ), D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f ) },
    };
    D3D10_BUFFER_DESC bd;
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( SimpleVertex ) * 8;
    bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    D3D10_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = vertices;
    hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pVertexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set vertex buffer
    UINT stride = sizeof( SimpleVertex );
    UINT offset = 0;
    m_pd3dDevice->IASetVertexBuffers( 0, 1, &g_pVertexBuffer, &stride, &offset );

    // Create index buffer
    DWORD indices[] =
    {
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };
    bd.Usage = D3D10_USAGE_DEFAULT;
    bd.ByteWidth = sizeof( DWORD ) * 36;        // 36 vertices needed for 12 triangles in a triangle list
    bd.BindFlags = D3D10_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    InitData.pSysMem = indices;
    hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &g_pIndexBuffer );
    if( FAILED( hr ) )
        return hr;

    // Set index buffer
    m_pd3dDevice->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

    // Set primitive topology
    m_pd3dDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

    return S_OK;
}


CD3DRenderer::CD3DRenderer(ISystem* pSystem) : CRenderer(pSystem)
{
	gD3DRender			 = this;
	CRenderer::m_Backend = RenderBackend::DX;
}

CD3DRenderer::~CD3DRenderer()
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pd3dDevice);
}

void CD3DRenderer::ShareResources(IRenderer* renderer)
{
}

void CD3DRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
}

void CD3DRenderer::PushProfileMarker(char* label)
{
}

void CD3DRenderer::PopProfileMarker(char* label)
{
}

int CD3DRenderer::CreateRenderTarget()
{
	return 0;
}

void CD3DRenderer::DrawFullscreenQuad()
{
}

float CD3DRenderer::GetDepthValue(int x, int y)
{
	return 0.0f;
}

void CD3DRenderer::Sh_Reload()
{
}

IWindow* CD3DRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	return CRenderer::Init(x,y,width, height, cbpp, zbpp, sbits, fullscreen, window);
}

bool CD3DRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CD3DRenderer::BeginFrame(void)
{
	// Очистка рендер-таргета
	float ClearColor[4] = {0.3f, 0.3f, 0.5f, 1.0f}; //red,green,blue,alpha
	m_pd3dDevice->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
}

void CD3DRenderer::Update(void)
{
    // Update our time
    static float t = 0.0f;
    if( g_driverType == D3D10_DRIVER_TYPE_REFERENCE )
    {
        t += ( float )D3DX_PI * 0.0125f;
    }
    else
    {
        static DWORD dwTimeStart = 0;
        DWORD dwTimeCur = GetTickCount();
        if( dwTimeStart == 0 )
            dwTimeStart = dwTimeCur;
        t = ( dwTimeCur - dwTimeStart ) / 1000.0f;
    }

    //
    // Animate the cube
    //
    D3DXMatrixRotationY( &g_World, t );

    //
    // Clear the back buffer
    //
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    m_pd3dDevice->ClearRenderTargetView( m_pRenderTargetView, ClearColor );

    //
    // Update variables
    //
    g_pWorldVariable->SetMatrix( ( float* )&g_World );
    g_pViewVariable->SetMatrix( ( float* )&g_View );
    g_pProjectionVariable->SetMatrix( ( float* )&g_Projection );

    //
    // Renders a triangle
    //
    D3D10_TECHNIQUE_DESC techDesc;
    g_pTechnique->GetDesc( &techDesc );
    for( UINT p = 0; p < techDesc.Passes; ++p )
    {
        g_pTechnique->GetPassByIndex( p )->Apply( 0 );
        m_pd3dDevice->DrawIndexed( 36, 0, 0 );        // 36 vertices needed for 12 triangles in a triangle list
    }

    //
    // Present our back buffer to our front buffer
    //
    m_pSwapChain->Present( 0, 0 );
}


void CD3DRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
}

void CD3DRenderer::SetViewport(int x, int y, int width, int height)
{
}

void CD3DRenderer::SetScissor(int x, int y, int width, int height)
{
}

void CD3DRenderer::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

bool CD3DRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CD3DRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void CD3DRenderer::DrawFullScreenImage(int texture_id)
{
}

int CD3DRenderer::SetPolygonMode(int mode)
{
	return 0;
}

void CD3DRenderer::ScreenShot(const char* filename)
{
}

void CD3DRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

void CD3DRenderer::SetState(State state, bool enable)
{
}

void CD3DRenderer::SetCullMode(CullMode mode)
{
}

bool CD3DRenderer::DeleteContext(WIN_HWND hWnd)
{
	return false;
}

bool CD3DRenderer::CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX, int SSY)
{
	return false;
}

bool CD3DRenderer::SetCurrentContext(WIN_HWND hWnd)
{
	return false;
}

void CD3DRenderer::MakeMainContextActive()
{
}

WIN_HWND CD3DRenderer::GetCurrentContextHWND()
{
	return WIN_HWND();
}

bool CD3DRenderer::IsCurrentContextMainVP()
{
	return false;
}

int CD3DRenderer::GetCurrentContextViewportHeight() const
{
	return 0;
}

int CD3DRenderer::GetCurrentContextViewportWidth() const
{
	return 0;
}

void CD3DRenderer::SetClearColor(const Vec3& vColor)
{
}

void CD3DRenderer::ClearDepthBuffer()
{
}

void CD3DRenderer::ClearColorBuffer(const Vec3 vColor)
{
}

void CD3DRenderer::SetRenderTarget(int nHandle)
{
}

bool CD3DRenderer::InitOverride()
{
    HRESULT hr = S_OK;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    //createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    D3D10_DRIVER_TYPE driverTypes[] =
    {
		D3D10_DRIVER_TYPE_WARP,
        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = GetWidth();
    sd.BufferDesc.Height = GetHeight();
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)m_hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D10CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags,
                                            D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return false;

    // Create a render target view
    ID3D10Texture2D* pBuffer;
    hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBuffer );
    if( FAILED( hr ) )
        return false;

    hr = m_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &m_pRenderTargetView );
    pBuffer->Release();
    if( FAILED( hr ) )
        return false;

    m_pd3dDevice->OMSetRenderTargets( 1, &m_pRenderTargetView, NULL );

    // Setup the viewport
    D3D10_VIEWPORT vp;
    vp.Width = GetWidth();
    vp.Height = GetHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    m_pd3dDevice->RSSetViewports( 1, &vp );

    // Set up rasterizer
	D3D10_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode				 = D3D10_CULL_FRONT;
	rasterizerDesc.FillMode				 = D3D10_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias			 = false;
	rasterizerDesc.DepthBiasClamp		 = 0;
	rasterizerDesc.SlopeScaledDepthBias	 = 0;
	rasterizerDesc.DepthClipEnable		 = true;
	rasterizerDesc.ScissorEnable		 = false;
	rasterizerDesc.MultisampleEnable	 = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	ID3D10RasterizerState* rasterizerState;
	m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	m_pd3dDevice->RSSetState(rasterizerState);

    hr = InitCube();
    if (FAILED(hr))
        return false;
    
    // Initialize the world matrix
    D3DXMatrixIdentity( &g_World );

    // Initialize the view matrix
    D3DXVECTOR3 Eye( 0.0f, 1.0f, -5.0f );
    D3DXVECTOR3 At( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 Up( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &g_View, &Eye, &At, &Up );

    // Initialize the projection matrix
    D3DXMatrixPerspectiveFovLH( &g_Projection, ( float )D3DX_PI * 0.5f, GetWidth() / ( FLOAT )GetHeight(), 0.1f, 100.0f );


    return true;
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new CD3DRenderer(pSystem);
}
