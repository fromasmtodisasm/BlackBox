#include <BlackBox/Render/CRender.hpp>

CRender::CRender()
{
}

CRender::~CRender()
{
}

IWindow* CRender::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen)
{
	return nullptr;
}

bool CRender::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CRender::Release()
{
}

void CRender::BeginFrame(void)
{
}

void CRender::Update(void)
{
}

void CRender::GetViewport(int* x, int* y, int* width, int* height)
{
}

void CRender::SetViewport(int x = 0, int y = 0, int width = 0, int height = 0)
{
}

void CRender::SetScissor(int x = 0, int y = 0, int width = 0, int height = 0)
{
}

void CRender::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

void CRender::SetCamera(const CCamera& cam)
{
}

const CCamera& CRender::GetCamera()
{
	// TODO: insert return statement here
}

bool CRender::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CRender::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void CRender::Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info)
{
}

int CRender::SetPolygonMode(int mode)
{
	return 0;
}

int CRender::GetWidth()
{
	return 0;
}

int CRender::GetHeight()
{
	return 0;
}

void CRender::ScreenShot(const char* filename = nullptr)
{
}

void CRender::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}
