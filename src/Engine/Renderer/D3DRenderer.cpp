#include "D3DRenderer.h"

CD3DRenderer::~CD3DRenderer()
{
}

void CD3DRenderer::ShareResources(IRenderer* renderer)
{
}

void CD3DRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
}

void CD3DRenderer::ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz)
{
}

int CD3DRenderer::UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4])
{
	return 0;
}

int CD3DRenderer::UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz)
{
	return 0;
}

void CD3DRenderer::GetModelViewMatrix(float* mat)
{
}

void CD3DRenderer::GetModelViewMatrix(double* mat)
{
}

void CD3DRenderer::GetProjectionMatrix(double* mat)
{
}

void CD3DRenderer::GetProjectionMatrix(float* mat)
{
}

Vec3 CD3DRenderer::GetUnProject(const Vec3& WindowCoords, const CCamera& cam)
{
	return Vec3();
}

void CD3DRenderer::PushProfileMarker(char* label)
{
}

void CD3DRenderer::PopProfileMarker(char* label)
{
}

IRenderAuxGeom* CD3DRenderer::GetIRenderAuxGeom()
{
	return nullptr;
}

int CD3DRenderer::CreateRenderTarget()
{
	return 0;
}

void CD3DRenderer::DrawFullscreenQuad()
{
}

void CD3DRenderer::Set2DMode(bool enable, int ortox, int ortoy)
{
}

ITechniqueManager* CD3DRenderer::GetITechniqueManager()
{
	return nullptr;
}

IFont* CD3DRenderer::GetIFont()
{
	return nullptr;
}

IGraphicsDeviceConstantBuffer* CD3DRenderer::CreateConstantBuffer(int size)
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

IShaderProgram* CD3DRenderer::Sh_Load(const char* name, int flags, uint64 nMaskGen)
{
	return nullptr;
}

void CD3DRenderer::Sh_Reload()
{
}

ITexture* CD3DRenderer::LoadTexture(const char* nameTex, uint flags, byte eTT)
{
	return nullptr;
}

bool CD3DRenderer::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
	return false;
}

void CD3DRenderer::OnAfterVarChange(ICVar* pVar)
{
}

void CD3DRenderer::OnVarUnregister(ICVar* pVar)
{
}

bool CD3DRenderer::OnInputEvent(const SInputEvent& event)
{
	return false;
}

void CD3DRenderer::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
}

IWindow* CD3DRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	return nullptr;
}

bool CD3DRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CD3DRenderer::Release()
{
}

void CD3DRenderer::BeginFrame(void)
{
}

void CD3DRenderer::Update(void)
{
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

void CD3DRenderer::SetCamera(const CCamera& cam)
{
}

const CCamera& CD3DRenderer::GetCamera()
{
	// TODO: insert return statement here
	return m_Camera;
}

bool CD3DRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CD3DRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void CD3DRenderer::Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info)
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

int CD3DRenderer::GetWidth()
{
	return 0;
}

int CD3DRenderer::GetHeight()
{
	return 0;
}

void CD3DRenderer::ScreenShot(const char* filename)
{
}

void CD3DRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

void CD3DRenderer::PrintLine(const char* szText, SDrawTextInfo& info)
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

int CD3DRenderer::GetFrameID(bool bIncludeRecursiveCalls)
{
	return 0;
}
