#pragma once
#include <BlackBox/Renderer/BaseRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>

class CD3DRenderer : public CRenderer
{
public:
	~CD3DRenderer();
	// Inherited via CRenderer
	virtual void ShareResources(IRenderer* renderer) override;
	virtual void SetRenderCallback(IRenderCallback* pCallback) override;
	virtual void ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz) override;
	virtual int UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4]) override;
	virtual int UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz) override;
	virtual void GetModelViewMatrix(float* mat) override;
	virtual void GetModelViewMatrix(double* mat) override;
	virtual void GetProjectionMatrix(double* mat) override;
	virtual void GetProjectionMatrix(float* mat) override;
	virtual Vec3 GetUnProject(const Vec3& WindowCoords, const CCamera& cam) override;
	virtual void PushProfileMarker(char* label) override;
	virtual void PopProfileMarker(char* label) override;
	virtual IRenderAuxGeom* GetIRenderAuxGeom() override;
	virtual int CreateRenderTarget() override;
	virtual void DrawFullscreenQuad() override;
	virtual void Set2DMode(bool enable, int ortox, int ortoy) override;
	virtual ITechniqueManager* GetITechniqueManager() override;
	virtual IFont* GetIFont() override;
	virtual IGraphicsDeviceConstantBuffer* CreateConstantBuffer(int size) override;
	virtual float GetDepthValue(int x, int y) override;
	virtual void Flush() override;
	virtual IShaderProgram* Sh_Load(const char* name, int flags = 0, uint64 nMaskGen = 0) override;
	virtual void Sh_Reload() override;
	virtual ITexture* LoadTexture(const char* nameTex, uint flags, byte eTT) override;
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;
	virtual void OnAfterVarChange(ICVar* pVar) override;
	virtual void OnVarUnregister(ICVar* pVar) override;
	virtual bool OnInputEvent(const SInputEvent& event) override;
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) override;
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
	virtual void DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) override;
	virtual void DrawFullScreenImage(int texture_id) override;
	virtual int SetPolygonMode(int mode) override;
	virtual int GetWidth() override;
	virtual int GetHeight() override;
	virtual void ScreenShot(const char* filename = nullptr) override;
	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;
	virtual void PrintLine(const char* szText, SDrawTextInfo& info) override;
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
	virtual int GetFrameID(bool bIncludeRecursiveCalls = true) override;

private:
	CCamera m_Camera;
};
