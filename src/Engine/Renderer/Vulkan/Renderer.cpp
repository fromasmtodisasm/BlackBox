#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include "Renderer.h"

#include <BlackBox/System/IWindow.hpp>

CVKRenderer* gD3DRender;

VkDevice GetDevice()
{
	return CVKRenderer::GetDevice(gD3DRender);
}


CVKRenderer::CVKRenderer(ISystem* pSystem)
	: CRenderer(pSystem)
{
	gD3DRender = this;
}

CVKRenderer::~CVKRenderer()
{
}

void CVKRenderer::ShareResources(IRenderer* renderer)
{
}

void CVKRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
}

void CVKRenderer::PushProfileMarker(char* label)
{
}

void CVKRenderer::PopProfileMarker(char* label)
{
}

int CVKRenderer::CreateRenderTarget()
{
	return 0;
}

void CVKRenderer::DrawFullscreenQuad()
{
}

float CVKRenderer::GetDepthValue(int x, int y)
{
	return 0.0f;
}

void CVKRenderer::Sh_Reload()
{
}

bool CVKRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CVKRenderer::BeginFrame(void)
{
}

void CVKRenderer::Update(void)
{
}

void CVKRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
}

void CVKRenderer::SetViewport(int x, int y, int width, int height)
{
}

void CVKRenderer::SetScissor(int x, int y, int width, int height)
{
}

void CVKRenderer::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

bool CVKRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CVKRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void CVKRenderer::DrawFullScreenImage(int texture_id)
{
}

int CVKRenderer::SetPolygonMode(int mode)
{
	return 0;
}

void CVKRenderer::ScreenShot(const char* filename)
{
}

void CVKRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

void CVKRenderer::SetState(State state, bool enable)
{
}

void CVKRenderer::SetCullMode(CullMode mode)
{
}

bool CVKRenderer::DeleteContext(WIN_HWND hWnd)
{
	return false;
}

bool CVKRenderer::CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX, int SSY)
{
	return false;
}

bool CVKRenderer::SetCurrentContext(WIN_HWND hWnd)
{
	return false;
}

void CVKRenderer::MakeMainContextActive()
{
}

WIN_HWND CVKRenderer::GetCurrentContextHWND()
{
	return WIN_HWND();
}

bool CVKRenderer::IsCurrentContextMainVP()
{
	return false;
}

int CVKRenderer::GetCurrentContextViewportHeight() const
{
	return 0;
}

int CVKRenderer::GetCurrentContextViewportWidth() const
{
	return 0;
}

void CVKRenderer::SetClearColor(const Vec3& vColor)
{
}

void CVKRenderer::ClearDepthBuffer()
{
}

void CVKRenderer::ClearColorBuffer(const Vec3 vColor)
{
}

void CVKRenderer::SetRenderTarget(int nHandle)
{
}

bool CVKRenderer::InitOverride()
{
	VK_LOG("Initialize");

	uint32 extensions_count;
	RETURN_B_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, nullptr),	"Could not get the number of Instance extensions.");

	std::vector<VkExtensionProperties> available_extensions(extensions_count);
	RETURN_B_IF_FAILED(vkEnumerateInstanceExtensionProperties(nullptr, &extensions_count, &available_extensions[0]), "Could not enumerate %d instance extensions", extensions_count);

	VK_LOG("Dump %d extensions", extensions_count);
	for (const auto& ext : available_extensions)
	{
		VK_LOG("%s", ext.extensionName);	
	}

	return true;
}

bool CVKRenderer::OnResizeSwapchain(int newWidth, int newHeight)
{
	return false;
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new CVKRenderer(pSystem);
}

