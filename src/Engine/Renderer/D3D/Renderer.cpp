#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include "Renderer.h"

#include <BlackBox/System/IWindow.hpp>

#include <BlackBox/Core/Platform/platform_impl.inl>

struct ITechniqueManager;

CD3DRenderer::CD3DRenderer(ISystem* pSystem) : CRenderer(pSystem)
{
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

ITechniqueManager* CD3DRenderer::GetITechniqueManager()
{
	return nullptr;
}

float CD3DRenderer::GetDepthValue(int x, int y)
{
	return 0.0f;
}

void CD3DRenderer::Flush()
{
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
}

void CD3DRenderer::Update(void)
{
	// Очистка рендер-таргета
	float ClearColor[4] = { 0.0f, 0.9f, 0.5f, 1.0f }; //red,green,blue,alpha
	m_pd3dDevice->ClearRenderTargetView( m_pRenderTargetView, ClearColor );
	// Отображение геометрии на рендер-таргете
	// Вывод содержимого рендер-таргета на экран
	m_pSwapChain->Present( 1, 0 );
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

void CD3DRenderer::DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)
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
	#if 0
	m_Window->close();

	auto g_hInst = GetModuleHandle(0);
	RECT rc = {0, 0, 640, 480};
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(LWCaption, LWName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
	#endif


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount						  = 1;
	sd.BufferDesc.Width					  = GetWidth();
	sd.BufferDesc.Height				  = GetHeight();
	sd.BufferDesc.Format				  = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator	  = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage						  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow						  = (HWND)m_hWnd;
	sd.SampleDesc.Count					  = 1;
	sd.SampleDesc.Quality				  = 0;
	sd.Windowed							  = TRUE;

	if (FAILED(D3D10CreateDeviceAndSwapChain(
		NULL,
		D3D10_DRIVER_TYPE::D3D10_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D10_CREATE_DEVICE_SINGLETHREADED,
		D3D10_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pd3dDevice
	)))
	{
		gEnv->pSystem->FatalError("Cannot create d3d device");	
		return false;
	}

	gEnv->pLog->Log("Created d3d device");

	// Создание рендер-таргет
	ID3D10Texture2D* pBackBuffer;
	auto hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	//pBackBuffer->Release();

	m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	return true;
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new CD3DRenderer(pSystem);
}
