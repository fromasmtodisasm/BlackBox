#pragma once
#include "../BaseRenderer.hpp"
#include <BlackBox/Renderer/Camera.hpp>
#include <Cry_Color4.h>
#pragma warning(push)
#pragma warning( disable : 4005 )
#include <d3dx10.h>
#pragma warning(pop)
#include <BlackBox\System\File\CryFile.h>
class CD3DRenderer;
extern CD3DRenderer* gD3DRender;

#define DEVICE m_pd3dDevice

struct Image2D
{
	float	x, y, w, h;
	int		id;
	float	s0, t0, s1, t1;
	color4f color;
	float	z;
	Image2D() = default;
	Image2D(
		float x, float y, float w, float h,
		int	  id,
		float s0, float t0, float s1, float t1,
		color4f color,
		float	z
	): x(x), y(y), w(w), h(h),
		id(id),
		s0(s0), t0(t0), s1(s1), t1(t1),
		color(color),
		z(z)
	{
	}
};

struct STexPic : 
	public ITexPic
	, public CD3D10_TEXTURE2D_DESC
{
	STexPic() {}
	STexPic(CD3D10_TEXTURE2D_DESC& desc, int id, const char* name)
		: CD3D10_TEXTURE2D_DESC(desc)
		, m_Id(id)
		, m_Name(name)
	{
	}
	virtual void		AddRef() override {}
	virtual void		Release(int bForce) override {}
	virtual const char* GetName() override { return m_Name.data(); }
	virtual int			GetWidth() override { return Width; }
	virtual int			GetHeight() override { return Height; }
	virtual int			GetOriginalWidth() override { return Width; }
	virtual int			GetOriginalHeight() override { return Height; }
	virtual int			GetTextureID() override { return m_Id; }
	virtual int			GetFlags() override { return 0; }
	virtual int			GetFlags2() override {return 0;}
	virtual void		SetClamp(bool bEnable) override {}
	virtual bool		IsTextureLoaded() override { return true; }
	virtual void		PrecacheAsynchronously(float fDist, int Flags) override {}
	virtual void		Preload(int Flags) override {}
	virtual byte*		GetData32() override { return nullptr; }
	virtual bool		SetFilter(int nFilter) override { return false; }

	int m_Id;
	string m_Name;
};

struct SPerFrameConstants
{
	D3DXMATRIX View;
	D3DXMATRIX Projection;
	//D3DXVECTOR4 SunDirection;
};

class CD3DRenderer : public CRenderer
{
public:
	//HRESULT InitCube();
	CD3DRenderer(ISystem* pSystem);
	~CD3DRenderer() override;
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
	virtual void Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs) override;
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
	virtual void ClearDepthBuffer() override;
	virtual void ClearColorBuffer(const Legacy::Vec3 vColor) override;
	virtual void SetRenderTarget(int nHandle) override;
	// Inherited via CRenderer
	virtual bool InitOverride() override;
	static auto GetDevice(IRenderer* pThis) { return static_cast<CD3DRenderer*>(pThis)->m_pd3dDevice; }

	bool OnResizeSwapchain(int newWidth, int newHeight);

	virtual void GetMemoryUsage(ICrySizer* Sizer) const override;

	//! Draw a 2d image on the screen (Hud etc.)
	virtual void Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0 = 0, float t0 = 0, float s1 = 1, float t1 = 1, float angle = 0, float r = 1, float g = 1, float b = 1, float a = 1, float z = 1) override;

	virtual void* EF_Query(int Query, int Param) override;


	virtual unsigned int LoadTexture(const char* filename, int* tex_type = NULL, unsigned int def_tid = 0, bool compresstodisk = true, bool bWarn = true) override;
	virtual void		 RemoveTexture(unsigned int TextureId) override;
	virtual void		 RemoveTexture(ITexPic* pTexPic) override;

	virtual ITexPic* EF_GetTextureByID(int Id) override;
	virtual ITexPic* EF_LoadTexture(const char* nameTex, uint flags, uint flags2, byte eTT, float fAmount1 = -1.0f, float fAmount2 = -1.0f, int Id = -1, int BindId = 0) override;
	virtual void	 SetTexture(int tnum, ETexType Type = eTT_Base) override;	

	ID3D10ShaderResourceView* CreateTexture(std::vector<uint8_t> &blob);
	ID3D10ShaderResourceView* CreateTextureFromFile(CCryFile file);

	string AdjustTexturePath(const char* name);
	bool FindTexture(const char* filename, CCryFile& result);

	void Draw2DQuad(float x, float y, float w, float h, int, color4f color, float s0, float t0, float s1, float t1) final;

	int CreateEmptyTexture(vector2di size, color4f color);

  private:
	int NextTextureIndex();
	int AddTextureResource(const char* name, ID3D10ShaderResourceView* pSRView);

  private:

	ID3D10Device* m_pd3dDevice					= NULL;
	IDXGISwapChain* m_pSwapChain				= NULL;
	ID3D10RenderTargetView* m_pRenderTargetView = NULL;

	ID3D10Texture2D* m_DepthStencilBuffer{};
	ID3D10DepthStencilView* m_pDepthStencilView{};

	ID3D10RasterizerState* m_pRasterizerState{};
	ID3D10DepthStencilState* m_pDepthStencilState{};

	ID3D10Buffer* m_PerViewConstants;

	std::vector<Image2D> m_DrawImages;

	std::map<string, int>												m_LoadedTextureNames;
	std::map<int, std::pair<ID3DTexture2D*, ID3D10ShaderResourceView*>> m_TexturesMap;
	std::map<int, STexPic>												m_TexPics;
	int																	m_NumLoadedTextures{};

};

ID3D10Device* GetDevice();

