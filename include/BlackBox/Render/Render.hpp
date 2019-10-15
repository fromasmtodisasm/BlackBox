#pragma once
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Window.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Quad.hpp>
#include <BlackBox/IConsole.hpp>

class CRender : 
	public IRender,
	public IConsoleVarSink,
	public IInputEventListener 
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

	virtual int EnumDisplayFormats(SDispFormat* formats) override;

	virtual void SetState(State state, bool enable) override;
  // Inherited via IRender
  virtual void DrawFullScreenImage(int texture_id) override;

	// Inherited via IConsoleVarSink
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;
	// Inherited via IInputEventListener
	virtual bool OnInputEvent(sf::Event& event) override;
private:
	void glInit();
	void fillSates();
	void initConsoleVariables();

private:
	IWindow* m_Window = nullptr;
	ISystem *m_pSystem = nullptr;

	bool is_fullscreen = false;
	Vec4 m_viewPort;
	unsigned int cbpp = 0;
	int zbpp = 0;
	int sbits = 0;

	//============
	const GLuint majorVersion = 4;
	const GLuint minorVersion = 3;
	const int antialiassing = 8;
	OpenglDebuger *glDebug;
	//============
	CCamera *m_Camera = nullptr;
	Quad *m_ScreenQuad = nullptr;
	// Shaders 
	BaseShaderProgramRef m_ScreenShader;
#if defined(_DEBUG) || defined(GL_DEBUG)
	bool isDebug = true;
#else
	bool isDebug = false;
#endif 
#if defined(_DEBUG) || defined(GL_DEBUG)
	sf::ContextSettings::Attribute glContextType = sf::ContextSettings::Attribute::Debug;
#else
	sf::ContextSettings::Attribute glContextType = sf::ContextSettings::Attribute::Core;
#endif 
	// DEBUG
	ICVar* translateImageX = nullptr;
	ICVar* translateImageY = nullptr;

	ICVar* scaleImageX = nullptr;
	ICVar* scaleImageY = nullptr;

	ICVar* needTranslate = nullptr;
	ICVar* needFlipY = nullptr;

	ICVar* r_debug = nullptr;
	ICVar* test_proj = nullptr;
	ICVar* render_via_viewport = nullptr;

	ICVar* cam_width;
	ICVar* cam_height;

	std::map<State, GLenum> stateMap;




	// Inherited via IRender
	virtual void SetCullMode(CullMode mode = CullMode::BACK) override;

};

