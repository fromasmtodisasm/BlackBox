#pragma once
#include <BlackBox/Render/IRender.hpp>

class CRender : public IRender
{
public:
	CRender();
	~CRender();

private:

	// Inherited via IRender
	virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen) override;

	virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) override;

	virtual void Release() override;

	virtual void BeginFrame(void) override;

	virtual void Update(void) override;

	virtual void GetViewport(int* x, int* y, int* width, int* height) override;

	virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) override;

	virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) override;

	virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) override;

	virtual void SetCamera(const CCamera& cam) override;

	virtual const CCamera& GetCamera() override;

	virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) override;

	virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

	virtual void Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info) override;

	virtual int SetPolygonMode(int mode) override;

	virtual int GetWidth() override;

	virtual int GetHeight() override;

	virtual void ScreenShot(const char* filename = nullptr) override;

	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;

};

