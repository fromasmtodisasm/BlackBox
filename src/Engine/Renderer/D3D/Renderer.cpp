#include "Renderer.h"

// Globals
ID3D10ShaderResourceView* GlobalResources::FontAtlasRV{};
ID3D10SamplerState*		  GlobalResources::LinearSampler{};

ID3D10ShaderResourceView* GlobalResources::WiteTextureRV;
ID3D10ShaderResourceView* GlobalResources::GreyTextureRV;

ID3D10EffectTechnique* GlobalResources::BoxTechnique;
ID3D10EffectTechnique* GlobalResources::MeshTechnique;

struct ITechniqueManager;
ID3D10Device* GetDevice()
{
	return CD3DRenderer::GetDevice(gEnv->pRenderer);
}

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3D10_DRIVER_TYPE g_driverType = D3D10_DRIVER_TYPE_NULL;
CD3DRenderer* gD3DRender;

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

	CRenderer::~CRenderer();
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
	m_pd3dDevice->ClearRenderTargetView(m_pRenderTargetView, &m_ClearColor[0]);
	m_pd3dDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.f, 0);
}

void CD3DRenderer::Update(void)
{
	m_FrameID++;	
    //
    // Clear the back buffer
    //
    //float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    //GetDevice()->ClearRenderTargetView( m_pRenderTargetView, ClearColor );

	::GetDevice()->OMSetDepthStencilState(m_pDepthStencilState, 0);
    Flush();

	for (const auto& rcl : m_RenderCallbackClients)
	{
		rcl->OnRenderer_BeforeEndFrame();	
	}

	if (IConsole* pConsole = gEnv->pSystem->GetIConsole())
		pConsole->Draw();
    //
    // Present our back buffer to our front buffer
    //
    m_pSwapChain->Present( 0, 0 );
}

void CD3DRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
	uint n = 1;
	D3D10_VIEWPORT viewports;
	m_pd3dDevice->RSGetViewports(&n, &viewports);
	*x = viewports.TopLeftX;
	*y = viewports.TopLeftY;
	*width = viewports.Width;
	*height = viewports.Height;
}

void CD3DRenderer::SetViewport(int x, int y, int width, int height)
{
}

void CD3DRenderer::SetScissor(int x, int y, int width, int height)
{
}

void CD3DRenderer::Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs)
{
}

bool CD3DRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CD3DRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    // Setup the viewport
    D3D10_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = x;
    vp.TopLeftY = y;
    m_pd3dDevice->RSSetViewports( 1, &vp );
	OnResizeSwapchain(width - x, height - y);	
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
	return m_hWnd;
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

void CD3DRenderer::ClearDepthBuffer()
{
}

void CD3DRenderer::ClearColorBuffer(const Legacy::Vec3 vColor)
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
        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 2;
    sd.BufferDesc.Width = GetWidth();
    sd.BufferDesc.Height = GetHeight();
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)m_Window->getNativeHandle();
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
	{
		CryLog("Failed create D3D10 device");
        return false;
	}


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

	m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
	m_pd3dDevice->RSSetState(m_pRasterizerState);

    {
		D3D10_DEPTH_STENCIL_DESC desc;
		//desc.BackFace
		desc.DepthEnable	= true;
		desc.StencilEnable	= false;
		desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc		= D3D10_COMPARISON_LESS ;

		::GetDevice()->CreateDepthStencilState(&desc, &m_pDepthStencilState);
		::GetDevice()->OMSetDepthStencilState(m_pDepthStencilState, 0);
	
    }

	/*if (!OnResizeSwapchain(GetWidth(), GetHeight()))
		return false;*/
	ChangeViewport(0, 0, GetWidth(), GetHeight());

    m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    //m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	Legacy::Vec3 c = Legacy::Vec3(2, 162, 246) / 255.f;
	SetClearColor(c);

    return true;
}

bool CD3DRenderer::OnResizeSwapchain(int newWidth, int newHeight)
{
	m_pSwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);

    // Create a render target view
    ID3D10Texture2D* pBuffer;
    auto hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBuffer );
    if( FAILED( hr ) )
        return false;

    hr = m_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &m_pRenderTargetView );
    pBuffer->Release();
    if( FAILED( hr ) )
        return false;

    D3D10_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width				= newWidth;
	depthStencilDesc.Height				= newHeight;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count	= 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage				= D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags		= 0;
	depthStencilDesc.MiscFlags			= 0;

	hr = (m_pd3dDevice->CreateTexture2D(
		&depthStencilDesc, 0, &m_DepthStencilBuffer));
    if( FAILED( hr ) )
        return false;
	hr = (m_pd3dDevice->CreateDepthStencilView(
		m_DepthStencilBuffer, 0, &m_pDepthStencilView));
    if( FAILED( hr ) )
        return false;

    m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	return true;
}

void CD3DRenderer::GetMemoryUsage(ICrySizer* Sizer) const
{
	Sizer->AddObject(this, sizeof(*this));
}

void CD3DRenderer::Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float angle, float r, float g, float b, float a, float z)
{
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new CD3DRenderer(pSystem);
}
