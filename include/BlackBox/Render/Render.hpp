#pragma once
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Window.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Quad.hpp>
#include <BlackBox/IConsole.hpp>

class CRender : 
	public IRender,
	public IConsoleVarSink
{
public:
	CRender(ISystem *engine);
	~CRender();

public:
	// Inherited via IRender
	virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow *window = nullptr) override;

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

	virtual void DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) override;

	virtual void PrintLine(const char* szText, SDrawTextInfo& info) override;
private:
	void glInit();

private:
	IWindow* m_Window;
	ISystem *m_Engine;

	bool is_fullscreen = false;
	Rect m_viewPort;
	unsigned int cbpp;
	int zbpp;
	int sbits;

	//============
	const GLuint majorVersion = 4;
	const GLuint minorVersion = 3;
	OpenglDebuger *glDebug;
	//============
	CCamera *m_Camera;
	Quad *m_ScreenQuad;
	// Shaders 
	BaseShaderProgramRef m_ScreenShader;
#if defined(_DEBUG) || defined(GL_DEBUG)
	sf::ContextSettings::Attribute glContextType = sf::ContextSettings::Attribute::Debug;
#else
	sf::ContextSettings::Attribute glContextType = sf::ContextSettings::Attribute::Core;
#endif 
	// DEBUG
	ICVar* translateImageX;
	ICVar* translateImageY;

	ICVar* scaleImageX;
	ICVar* scaleImageY;

	ICVar* needTranslate;
	ICVar* needFlipY;

	ICVar* r_debug;
	ICVar* test_proj;
	ICVar* render_via_viewport;

  // Inherited via IRender
  virtual void DrawFullScreenImage(int texture_id) override;

	// Inherited via IConsoleVarSink
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;
};

