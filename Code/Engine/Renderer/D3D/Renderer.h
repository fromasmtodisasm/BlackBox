#pragma once
#include "../BaseRenderer.hpp"
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/File/CryFile.h>
#include <Cry_Color4.h>

#include "Device.hpp"
#include "SwapChain.hpp"

#include <memory>
#include <utility>

class CD3DRenderer;
extern CD3DRenderer* gD3DRender;

#define DEVICE m_Device->Get<ID3DDevice>()

struct SRenderThread;

//template <class T, class... Types>
//_smart_ptr<T> make_smart_ptr(Types&&... _Args)
//{
//    const auto _Rx = new T(std::forward<Types>(_Args)...);
//    return _smart_ptr<T>(_Rx);
//}
template<class T, class Inst>
_smart_ptr<T> make_smart_ptr(Inst* inst)
{
	return _smart_ptr<T>(inst);
}

struct Image2D
{
	float   x, y, w, h;
	int     id;
	float   s0, t0, s1, t1;
	color4f color;
	float   z;
	Image2D() = default;
	Image2D(
	    float x, float y, float w, float h,
	    int   id,
	    float s0, float t0, float s1, float t1,
	    color4f color,
	    float   z)
	    : x(x)
	    , y(y)
	    , w(w)
	    , h(h)
	    , id(id)
	    , s0(s0)
	    , t0(t0)
	    , s1(s1)
	    , t1(t1)
	    , color(color)
	    , z(z)
	{
	}
};

struct STexPic : _reference_target_t, public ITexPic, public CD3D11_TEXTURE2D_DESC
{
	STexPic() {}
	STexPic(CD3D11_TEXTURE2D_DESC& desc, int id, const char* name)
	    : CD3D11_TEXTURE2D_DESC(desc)
	    , m_Id(id)
	    , m_Name(name)
	    , m_Loaded(true)

	{
	}

	virtual void        AddRef() override { _reference_target_t::AddRef(); }
	virtual void        Release(int bForce) override { _reference_target_t::Release(); }
	void                Release() { _reference_target_t::Release(); }
	virtual const char* GetName() override { return m_Name.c_str(); }
	virtual int         GetWidth() override { return Width; }
	virtual int         GetHeight() override { return Height; }
	virtual int         GetOriginalWidth() override { return Width; }
	virtual int         GetOriginalHeight() override { return Height; }
	virtual int         GetTextureID() override { return m_Id; }
	virtual int         GetFlags() override { return 0; }
	virtual int         GetFlags2() override { return 0; }
	virtual void        SetClamp(bool bEnable) override {}
	virtual bool        IsTextureLoaded() override { return m_Loaded; }
	virtual void        PrecacheAsynchronously(float fDist, int Flags) override {}
	virtual void        Preload(int Flags) override {}
	virtual byte*       GetData32() override { return nullptr; }
	virtual bool        SetFilter(int nFilter) override { return false; }

	int                 m_Id{-1};
	string              m_Name;
	bool                m_Loaded = false;
};

struct SPerFrameConstants
{
	D3DXMATRIX View;
	D3DXMATRIX Projection;
	//D3DXVECTOR4 SunDirection;
};

ID3DDevice*        GetDevice();
ID3DDeviceContext* GetDeviceContext();

//=========================================================================================

// NOTE: ...
struct RenderTarget : _reference_target_t
{
	~RenderTarget()
	{
	}

	static std::pair<bool, _smart_ptr<RenderTarget>> Create(int textureWidth, int textureHeight)
	{
		auto                 renderTarget          = std::make_pair(false, _smart_ptr<RenderTarget>(new RenderTarget));
		auto&                m_renderTargetTexture = renderTarget.second->m_renderTargetTexture;
		auto&                m_renderTargetView    = renderTarget.second->m_renderTargetView;
		auto&                m_shaderResourceView  = renderTarget.second->m_shaderResourceView;

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width            = textureWidth;
		textureDesc.Height           = textureHeight;
		textureDesc.MipLevels        = 1;
		textureDesc.ArraySize        = 1;
		textureDesc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage            = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags        = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags   = 0;
		textureDesc.MiscFlags        = 0;

		HRESULT result               = GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_renderTargetTexture);
		if (FAILED(result))
			return renderTarget;

		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format             = textureDesc.Format;
		renderTargetViewDesc.ViewDimension      = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		result                                  = GetDevice()->CreateRenderTargetView(m_renderTargetTexture.Get(), &renderTargetViewDesc, &m_renderTargetView);
		if (FAILED(result))
			return renderTarget;

		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format                    = textureDesc.Format;
		shaderResourceViewDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels       = 1;

		result                                           = GetDevice()->CreateShaderResourceView(m_renderTargetTexture.Get(), &shaderResourceViewDesc, &m_shaderResourceView);
		if (FAILED(result))
			return renderTarget;

		renderTarget.first = true;
		return renderTarget;
	}

	ComPtr<ID3DTexture2D>          m_renderTargetTexture{};
	ComPtr<ID3DRenderTargetView>   m_renderTargetView{};
	ComPtr<ID3DShaderResourceView> m_shaderResourceView{};
};

class CD3DRenderer : public CRenderer
{
	friend class CSwapChain;

public:
	//HRESULT InitCube();
	CD3DRenderer();
	~CD3DRenderer() override;
	//////////////////////////////////////////////////////////////////////////
	// Remove pointer indirection.
	ILINE CD3DRenderer*     operator->() { return this; }
	ILINE const bool        operator!() const { return false; }
	ILINE                   operator bool() const { return true; }
	ILINE                   operator CD3DRenderer*() { return this; }
	//////////////////////////////////////////////////////////////////////////

	// Inherited via CRenderer
	virtual void            ShareResources(IRenderer* renderer) override;
	virtual void            SetRenderCallback(IRenderCallback* pCallback) override;
	virtual void            PushProfileMarker(char* label) override;
	virtual void            PopProfileMarker(char* label) override;
	virtual int             CreateRenderTarget() override;
	virtual void            DrawFullscreenQuad() override;
	virtual float           GetDepthValue(int x, int y) override;
	virtual void            Sh_Reload() override;
	virtual IWindow*        Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) override;
	virtual bool            ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) override;
	virtual void            BeginFrame(void) override;
	virtual void            Update(void) override;
	virtual void            GetViewport(int* x, int* y, int* width, int* height) override;
	virtual void            SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void            SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void            Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs) override;
	virtual bool            ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) override;
	virtual void            ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
	virtual void            DrawFullScreenImage(int texture_id) override;
	virtual int             SetPolygonMode(int mode) override;
	virtual void            ScreenShot(const char* filename = nullptr) override;
	virtual void            RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;
	virtual void            SetState(State state, bool enable) override;
	virtual void            SetCullMode(CullMode mode = CullMode::BACK) override;
	virtual bool            DeleteContext(WIN_HWND hWnd) override;
	virtual bool            CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) override;
	virtual bool            SetCurrentContext(WIN_HWND hWnd) override;
	virtual void            MakeMainContextActive() override;
	virtual WIN_HWND        GetCurrentContextHWND() override;
	virtual bool            IsCurrentContextMainVP() override;
	virtual int             GetCurrentContextViewportHeight() const override;
	virtual int             GetCurrentContextViewportWidth() const override;
	virtual void            ClearDepthBuffer() override;
	virtual void            ClearColorBuffer(const Legacy::Vec3 vColor) override;
	virtual void            SetRenderTarget(int nHandle) override;
	// Inherited via CRenderer
	virtual bool            InitOverride() override;
	ID3DDevice*             GetDevice() { return m_Device->Get<ID3DDevice>(); }
	ID3DDeviceContext*      GetDeviceContext() { return m_Device->Get<ID3DDeviceContext>(); }

	bool                    OnResizeSwapchain(int newWidth, int newHeight);

	virtual void            GetMemoryUsage(ICrySizer* Sizer) const override;

	//! Draw a 2d image on the screen (Hud etc.)
	virtual void            Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0 = 0, float t0 = 0, float s1 = 1, float t1 = 1, float angle = 0, float r = 1, float g = 1, float b = 1, float a = 1, float z = 1) override;

	virtual void*           EF_Query(int Query, int Param) override;

	virtual unsigned int    LoadTexture(const char* filename, int* tex_type = NULL, unsigned int def_tid = 0, bool compresstodisk = true, bool bWarn = true) override;
	virtual void            RemoveTexture(unsigned int TextureId) override;
	virtual void            RemoveTexture(ITexPic* pTexPic) override;

	virtual ITexPic*        EF_GetTextureByID(int Id) override;
	virtual ITexPic*        EF_LoadTexture(const char* nameTex, uint flags, uint flags2, byte eTT, float fAmount1 = -1.0f, float fAmount2 = -1.0f, int Id = -1, int BindId = 0) override;
	virtual void            SetTexture(int tnum, ETexType Type = eTT_Base) override;

	ID3DShaderResourceView* CreateTexture(std::vector<uint8_t>& blob);
	ID3DShaderResourceView* CreateTextureFromFile(CCryFile file);
	ID3DShaderResourceView* CreateTextureFromFile(const char* name);

	string                  AdjustTexturePath(string name);
	bool                    FindTexture(string filename, CCryFile& result, string& adjustet_name);

	void                    Draw2DQuad(float x, float y, float w, float h, int, color4f color, float s0, float t0, float s1, float t1) final;
	void                    Draw2DQuad(float x, float y, float w, float h, ID3D11ShaderResourceView* view, color4f color, float s0, float t0, float s1, float t1);

	int                     CreateEmptyTexture(vector2di size, color4f color, DXGI_FORMAT format, UINT bindFlags);

	bool                    CreateRenderTargets();

private:
	int          NextTextureIndex();
	int          AddTextureResource(string name, ID3DShaderResourceView* pSRView, STexPic* pic);
	unsigned int LoadTextureInternal(STexPic* pix, string filename, int* tex_type = NULL, unsigned int def_tid = 0, bool compresstodisk = true, bool bWarn = true);

	void         UpdateConstants();
	template<class Data, class T, class B>
	auto ScopedMap(B* CB, std::function<void(Data* data)> Updater);

	template<class Data, template<class B> typename T, class B>
	auto ScopedMap(T<B> CB, std::function<void(Data* data)> Updater)
	{
		ScopedMap(CB.Get(), Updater);
	}

	template<class Data, class B>
	auto ScopedMap(B* CB, std::function<void(Data* data)> Updater)
	{
		D3D11_MAPPED_SUBRESOURCE Buffer;
		::GetDeviceContext()->Map(CB, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, NULL, &Buffer);
		Updater((Data*)Buffer.pData);
		::GetDeviceContext()->Unmap(CB, D3D11CalcSubresource(0, 0, 1));
	};

	void CleanupRenderTarget()
	{
		m_pMainRenderTargetView     = nullptr;
		m_DepthStencil->m_pView     = nullptr;
		m_DepthStencil->m_pResource = nullptr;

		GetDeviceContext()->OMSetRenderTargets(0, 0, 0);
	}

	bool CreateDepthStencil(int w, int h)
	{
		auto&                pDepthStencil = m_DepthStencil;
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width              = w;
		depthStencilDesc.Height             = h;
		depthStencilDesc.MipLevels          = 1;
		depthStencilDesc.ArraySize          = 1;
		depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1; // multisampling must match
		depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
		depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags     = 0;
		depthStencilDesc.MiscFlags          = 0;

		if (FAILED(H(GetDevice()->CreateTexture2D(
		                 &depthStencilDesc, 0, &pDepthStencil->m_pResource),
		             "Error Create DS Texture")))
			return false;
		ID3D11DepthStencilView* dsv{};

		if (FAILED(H(GetDevice()->CreateDepthStencilView(pDepthStencil->m_pResource, 0, &dsv),
		             "Error Create DS Texture")))
		{
			return false;
		}
		pDepthStencil->m_pView = dsv;
		return true;
	}

private:
	std::shared_ptr<CDevice>                                          m_Device;
	std::unique_ptr<CSwapChain>                                       m_pSwapChain;

	ComPtr<ID3DRenderTargetView>                                      m_pMainRenderTargetView;
	ComPtr<ID3DRasterizerState>                                       m_pRasterizerState{};
	ComPtr<ID3DDepthStencilState>                                     m_pDepthStencilState{};

	std::shared_ptr<CTexture>                                         m_DepthStencil;

	ComPtr<ID3DBuffer>                                                m_PerFrameConstants;
	ComPtr<ID3DBuffer>                                                m_PerViewConstants;
	ComPtr<ID3DBuffer>                                                m_Lights;

	std::vector<Image2D>                                              m_DrawImages;

	std::map<string, int>                                             m_LoadedTextureNames;
	std::map<int, std::pair<ID3DTexture2D*, ID3DShaderResourceView*>> m_TexturesMap;
	std::map<int, STexPic>                                            m_TexPics;
	int                                                               m_NumLoadedTextures{};

	_smart_ptr<RenderTarget>                                          m_RenderTargetScene;
	_smart_ptr<RenderTarget>                                          m_SceneRenderTargetFinal;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
extern CD3DRenderer gcpRendD3D;
#else
extern CD3DRenderer* gcpRendD3D;
#endif
