#pragma once
#include "../BaseRenderer.hpp"
#include <BlackBox/Renderer/Camera.hpp>
#include <d3d10.h>
class CD3DRenderer;
extern CD3DRenderer* gD3DRender;

class CD3DRenderer : public CRenderer
{
public:
	CD3DRenderer(ISystem* pSystem);
	~CD3DRenderer();
	// Inherited via CRenderer
	virtual void ShareResources(IRenderer* renderer) override;
	virtual void SetRenderCallback(IRenderCallback* pCallback) override;
	virtual void PushProfileMarker(char* label) override;
	virtual void PopProfileMarker(char* label) override;
	virtual int CreateRenderTarget() override;
	virtual void DrawFullscreenQuad() override;
	virtual float GetDepthValue(int x, int y) override;
	virtual void Sh_Reload() override;
	virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) override;
	virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) override;
	virtual void BeginFrame(void) override;
	virtual void Update(void) override;
	virtual void GetViewport(int* x, int* y, int* width, int* height) override;
	virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) override;
	virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) override;
	virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
	virtual void DrawFullScreenImage(int texture_id) override;
	virtual int SetPolygonMode(int mode) override;
	virtual void ScreenShot(const char* filename = nullptr) override;
	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;
	virtual void SetState(State state, bool enable) override;
	virtual void SetCullMode(CullMode mode = CullMode::BACK) override;
	virtual bool DeleteContext(WIN_HWND hWnd) override;
	virtual bool CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) override;
	virtual bool SetCurrentContext(WIN_HWND hWnd) override;
	virtual void MakeMainContextActive() override;
	virtual WIN_HWND GetCurrentContextHWND() override;
	virtual bool IsCurrentContextMainVP() override;
	virtual int GetCurrentContextViewportHeight() const override;
	virtual int GetCurrentContextViewportWidth() const override;
	virtual void SetClearColor(const Vec3& vColor) override;
	virtual void ClearDepthBuffer() override;
	virtual void ClearColorBuffer(const Vec3 vColor) override;
	virtual void SetRenderTarget(int nHandle) override;
	// Inherited via CRenderer
	virtual bool InitOverride() override;
	static auto GetDevice(IRenderer* pThis) { return static_cast<CD3DRenderer*>(pThis)->m_pd3dDevice; }
  private:

	ID3D10Device* m_pd3dDevice					= NULL;
	IDXGISwapChain* m_pSwapChain				= NULL;
	ID3D10RenderTargetView* m_pRenderTargetView = NULL;
};

inline auto GetDevice()
{
	return CD3DRenderer::GetDevice(gEnv->pRenderer);
}

