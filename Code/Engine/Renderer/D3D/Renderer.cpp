#include "Renderer.h"
#include "DDSTextureLoader11.h"
#include <BlackBox/Core/Path.hpp>
#include <BlackBox/System/File/CryFile.h>

#include "RenderThread.h"

class CD3D_FEATURE_LEVEL
{
  public:
	CD3D_FEATURE_LEVEL(D3D_FEATURE_LEVEL Type)
		: m_Type(Type)
	{
	}
	operator const char*()
	{
#define CONVERT(t) \
	case t:        \
		return #t;
		switch (m_Type)
		{
			CONVERT(D3D_FEATURE_LEVEL_9_1)
			CONVERT(D3D_FEATURE_LEVEL_9_2)
			CONVERT(D3D_FEATURE_LEVEL_9_3)
			CONVERT(D3D_FEATURE_LEVEL_10_0)
			CONVERT(D3D_FEATURE_LEVEL_10_1)
			CONVERT(D3D_FEATURE_LEVEL_11_0)
		default:
			return "Unknown";
		}
#undef CONVERT
	}
					   operator D3D_FEATURE_LEVEL() { return m_Type; }
					   operator D3D_FEATURE_LEVEL*() { return &m_Type; }
	D3D_FEATURE_LEVEL* operator&() { return &m_Type; }
	D3D_FEATURE_LEVEL  m_Type;
};

// Globals
ID3DShaderResourceView* GlobalResources::FontAtlasRV{};
ID3D11SamplerState*		GlobalResources::LinearSampler{};

ID3DShaderResourceView* GlobalResources::WiteTextureRV;
ID3DShaderResourceView* GlobalResources::GreyTextureRV;

static CD3DRenderer _gcpRendD3D;
static CD3DRenderer* gcpRendD3D = _gcpRendD3D;

//ID3D10EffectTechnique* GlobalResources::BoxTechnique;
//ID3D10EffectTechnique* GlobalResources::MeshTechnique;

ID3DInputLayout* GlobalResources::VERTEX_FORMAT_P3F_C4B_T2F_Layout;

ID3D11BlendState* GlobalResources::FontBlendState;

_smart_ptr<CShader> GlobalResources::TexturedQuadShader;
_smart_ptr<CShader> GlobalResources::SpriteShader;

struct ITechniqueManager;
ID3D11Device* GetDevice()
{
	return gcpRendD3D->GetDevice();
}

ID3DDeviceContext* GetDeviceContext()
{
	return gcpRendD3D->GetDeviceContext();
}

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3D_DRIVER_TYPE	   g_driverType	  = D3D_DRIVER_TYPE_NULL;
CD3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
CD3DRenderer*	   gD3DRender;

CD3DRenderer::CD3DRenderer()
	: CRenderer()
{
	gD3DRender			 = this;
	CRenderer::m_Backend = RenderBackend::DX;
}

CD3DRenderer::~CD3DRenderer()
{
	SAFE_RELEASE(m_pMainRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pd3dDevice);

	CRenderer::~CRenderer();
}

void CD3DRenderer::ShareResources(IRenderer* renderer)
{
}

void CD3DRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
}

void CD3DRenderer::PushProfileMarker(char* label)
{
}

void CD3DRenderer::PopProfileMarker(char* label)
{
}

int CD3DRenderer::CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    GetDevice()->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
    pBackBuffer->Release();
	return 0;
}

void CD3DRenderer::DrawFullscreenQuad()
{
}

float CD3DRenderer::GetDepthValue(int x, int y)
{
	return 0.0f;
}

void CD3DRenderer::Sh_Reload()
{
}

IWindow* CD3DRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	return CRenderer::Init(x, y, width, height, cbpp, zbpp, sbits, fullscreen, window);
}

bool CD3DRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CD3DRenderer::BeginFrame(void)
{
	D3DPERF_BeginEvent(D3DC_Blue, L"BeginFrame");
	m_pImmediateContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pDepthStencilView);
	m_pImmediateContext->ClearRenderTargetView(m_pMainRenderTargetView, &m_ClearColor[0]);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);
}

void CD3DRenderer::UpdateConstants()
{
	//D3DPERF_BeginEvent(D3DC_Blue, L"UpdateConstants");
	ScopedMap<SPerFrameConstantBuffer>(m_PerFrameConstants, [&](auto pConstData)
									   {
										   pConstData->SunDirection	   = Legacy::Vec4(glm::normalize(Legacy::Vec3(2, 3, 4)), 1.f);
										   pConstData->SunColor		   = {1, 1, 1, 1};
										   pConstData->AmbientStrength = Legacy::Vec4(1, 1, 1, 1) * 0.3f;
										   pConstData->LightIntensity  = Legacy::Vec4(1, 1, 1, 1);
									   });

	ScopedMap<SPerViewConstantBuffer>(m_PerViewConstants, [&](auto pConstData)
									  {
										  auto Projection			 = m_Camera.GetProjectionMatrix();
										  auto View					 = m_Camera.GetViewMatrix();
										  pConstData->Projection	 = Projection;
										  pConstData->ViewProjection = Projection * View;
									  });

	ID3DBuffer* pBuffers[] = {
		m_PerFrameConstants,
		m_PerViewConstants};
	::GetDeviceContext()->VSSetConstantBuffers(0, 2, pBuffers);
	::GetDeviceContext()->PSSetConstantBuffers(0, 1, pBuffers);
	//D3DPERF_EndEvent();
}

void CD3DRenderer::Update(void)
{
	{
		D3DPERF_BeginEvent(D3DC_Blue, L"BeginScene");
		{
			m_FrameID++;
			UpdateConstants();

			::GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState, 0);
			Flush();
			{
				D3DPERF_BeginEvent(D3DC_Blue, L"DrawImages");
				for (auto img : m_DrawImages)
				{
					Draw2DQuad(img.x, img.y, img.w, img.h, img.id, img.color, img.s0, img.t0, img.s1, img.t1);
				}
				D3DPERF_EndEvent();
			}
			{
				D3DPERF_BeginEvent(D3DC_Blue, L"OnRenderer_BeforeEndFrame");
				for (const auto& rcl : m_RenderCallbackClients)
				{
					rcl->OnRenderer_BeforeEndFrame();
				}
				D3DPERF_EndEvent();
			}
			{
				D3DPERF_BeginEvent(D3DC_Blue, L"DrawConsole");
				if (IConsole* pConsole = gEnv->pSystem->GetIConsole())
					pConsole->Draw();
				D3DPERF_EndEvent();
			}
		}
		D3DPERF_EndEvent();
		D3DPERF_EndEvent(); //begin frame
		m_DrawImages.clear();
	}

	m_pSwapChain->Present(0, 0);
}

void CD3DRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
	uint		   n = 1;
	D3D11_VIEWPORT viewports;
	ZeroStruct(viewports);
	m_pImmediateContext->RSGetViewports(&n, &viewports);
	*x		= (int)viewports.TopLeftX;
	*y		= (int)viewports.TopLeftY;
	*width	= (int)viewports.Width;
	*height = (int)viewports.Height;
}

void CD3DRenderer::SetViewport(int x, int y, int width, int height)
{
	CD3D11_VIEWPORT viewports(float(x), float(y), float(width), float(height),0.f,1.f);
	m_pImmediateContext->RSSetViewports(1, &viewports);
}

void CD3DRenderer::SetScissor(int x, int y, int width, int height)
{
}

void CD3DRenderer::Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs)
{
	gEnv->pAuxGeomRenderer->DrawAABB(mins, maxs, UCol{});
}

bool CD3DRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CD3DRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width	= (float)width;
	vp.Height	= (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = (float)x;
	vp.TopLeftY = (float)y;
	OnResizeSwapchain(width - x, height - y);
	m_pImmediateContext->RSSetViewports(1, &vp);
}

void CD3DRenderer::DrawFullScreenImage(int texture_id)
{
}

int CD3DRenderer::SetPolygonMode(int mode)
{
	return 0;
}

void CD3DRenderer::ScreenShot(const char* filename)
{
}

void CD3DRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
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
	return m_hWnd;
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

void CD3DRenderer::ClearDepthBuffer()
{
}

void CD3DRenderer::ClearColorBuffer(const Legacy::Vec3 vColor)
{
}

void CD3DRenderer::SetRenderTarget(int nHandle)
{
}

bool CD3DRenderer::InitOverride()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_HARDWARE),
			D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_WARP),
			D3D_DRIVER_TYPE(D3D_DRIVER_TYPE_REFERENCE),
		};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	CD3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount						  = 2;
	sd.BufferDesc.Width					  = GetWidth();
	sd.BufferDesc.Height				  = GetHeight();
	sd.BufferDesc.Format				  = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator	  = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage						  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow						  = (HWND)m_Window->getNativeHandle();
	sd.SampleDesc.Count					  = 1;
	sd.SampleDesc.Quality				  = 0;
	sd.Windowed							  = TRUE;
	sd.SwapEffect						  = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

	D3D_FEATURE_LEVEL l = featureLevels[0];

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr			 = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels[0], numFeatureLevels,
											 D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &g_featureLevel, &m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
	{
		CryLog("Failed create D3D10 device");
		return false;
	}

	CryLog("Created D3D device with feature level: %s", (const char*)g_featureLevel);

	#if 0
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr							 = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pMainRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;
	#endif
	// Set up rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode				 = D3D11_CULL_NONE;
	rasterizerDesc.FillMode				 = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias			 = false;
	rasterizerDesc.DepthBiasClamp		 = 0;
	rasterizerDesc.SlopeScaledDepthBias	 = 0;
	rasterizerDesc.DepthClipEnable		 = true;
	rasterizerDesc.ScissorEnable		 = false;
	rasterizerDesc.MultisampleEnable	 = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
	m_pImmediateContext->RSSetState(m_pRasterizerState);

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroStruct(desc);
		//desc.BackFace
		desc.DepthEnable	= true;
		desc.StencilEnable	= false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc		= D3D11_COMPARISON_LESS;

		GetDevice()->CreateDepthStencilState(&desc, &m_pDepthStencilState);
		GetDeviceContext()->OMSetDepthStencilState(m_pDepthStencilState, 0);
	}

	/*if (!OnResizeSwapchain(GetWidth(), GetHeight()))
		return false;*/
	ChangeViewport(0, 0, GetWidth(), GetHeight());

	#if 0
	m_pImmediateContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pDepthStencilView);
	#endif

	Legacy::Vec3 c = Legacy::Vec3(2, 162, 246) / 255.f;
	//Legacy::Vec3 c = Legacy::Vec3(0, 0, 0) / 255.f;
	SetClearColor(c);

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth	  = Memory::AlignedSizeCB<SPerViewConstantBuffer>::value;
	cbDesc.Usage		  = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags	  = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags	  = 0;

	hr = DEVICE->CreateBuffer(&cbDesc, NULL, &this->m_PerViewConstants);
	if (FAILED(hr))
	{
		return hr;
	}
	cbDesc.ByteWidth = Memory::AlignedSizeCB<SPerFrameConstantBuffer>::value;
	hr				 = DEVICE->CreateBuffer(&cbDesc, NULL, &this->m_PerFrameConstants);
	if (FAILED(hr))
	{
		return hr;
	}

	if (!CreateRenderTargets())
	{
		CryFatalError("Cannot create render target");
	}

#ifdef MULTITHREADED_RENDER
	m_RenderThread = std::make_unique<SRenderThread>();

	m_RenderThread->Start();
#endif

	return true;
}

bool CD3DRenderer::OnResizeSwapchain(int newWidth, int newHeight)
{
	#if 1
	CleanupRenderTarget();
	#endif
	m_pSwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
	SAFE_RELEASE(m_pMainRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);

	// Create a render target view
	ID3DTexture2D* pBuffer;
	auto		   hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3DTexture2D), (LPVOID*)&pBuffer);
	if (FAILED(hr))
		return false;

	hr = m_pd3dDevice->CreateRenderTargetView(pBuffer, NULL, &m_pMainRenderTargetView);
	auto num_refs = pBuffer->Release();
	if (FAILED(hr))
		return false;

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width				= newWidth;
	depthStencilDesc.Height				= newHeight;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count	= 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags		= 0;
	depthStencilDesc.MiscFlags			= 0;

	hr = (m_pd3dDevice->CreateTexture2D(
		&depthStencilDesc, 0, &m_DepthStencilBuffer));
	if (FAILED(hr))
		return false;
	hr = (m_pd3dDevice->CreateDepthStencilView(
		m_DepthStencilBuffer, 0, &m_pDepthStencilView));
	if (FAILED(hr))
		return false;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pDepthStencilView);

	return true;
}

void CD3DRenderer::GetMemoryUsage(ICrySizer* Sizer) const
{
	Sizer->AddObject(this, sizeof(*this));
}

void CD3DRenderer::Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float angle, float r, float g, float b, float a, float z)
{
#if 1
	if (m_Is2DMode)
	{
		s0 *= ortho.x;
		s1 *= ortho.x;

		t0 *= ortho.y;
		t1 *= ortho.y;
	}
#endif
	m_DrawImages.push_back({xpos, ypos, w, h, texture_id, s0, t0, s1, t1, color4f{r, g, b, a}, z});

#if 0
	Image2D img = m_DrawImages.back();
	Draw2DQuad(img.x, img.y, img.w, img.h, img.id, img.color, img.s0, img.t0, img.s1, img.t1);
	m_DrawImages.pop_back();
#endif
}

ITexPic* CD3DRenderer::EF_GetTextureByID(int Id)
{
	ITexPic* result{};
	auto	 it = m_TexPics.find(Id);
	if (it != m_TexPics.end())
	{
		result = &it->second;
	}
	return result;
}

ITexPic* CD3DRenderer::EF_LoadTexture(const char* nameTex, uint flags, uint flags2, byte eTT, float fAmount1, float fAmount2, int Id, int BindId)
{
	//_smart_ptr<STexPic> TexPic = new STexPic;
	//m_RenderThread->ExecuteRenderThreadCommand([=]
	//										   { LoadTextureInternal(TexPic, nameTex); });
	//return TexPic;
	return EF_GetTextureByID(LoadTexture(nameTex));
}
void CD3DRenderer::SetTexture(int tnum, ETexType Type)
{
	auto t = m_TexturesMap[tnum].second;
	if (t)
	{
		::GetDeviceContext()->PSSetShaderResources(0, 1, &t);
	}
}
ID3DShaderResourceView* CD3DRenderer::CreateTextureFromFile(CCryFile file)
{
	file.Seek(0, SEEK_END);
	auto texture_size = file.GetPosition();
	file.Seek(0, SEEK_SET);
	std::vector<uint8_t> blob(texture_size);
	file.Read(&blob[0], texture_size);
	return CreateTexture(blob);
}

ID3DShaderResourceView* CD3DRenderer::CreateTextureFromFile(const char* name)
{
	ID3D11Resource*			pTexture{};
	ID3DShaderResourceView* pSRView = NULL;
	HRESULT					HResult{};

	std::string_view str(name);
	std::wstring	 wstr(str.begin(), str.end());
	//HResult = D3DX11CreateShaderResourceViewFromFile(
	HResult = DirectX::CreateDDSTextureFromFile(
		m_pd3dDevice,
		wstr.data(),
		&pTexture,
		&pSRView);
	//if (FAILED(HResult))
	//{
	//}
	return pSRView;
}

string CD3DRenderer::AdjustTexturePath(string filename)
{
	string path(filename.c_str()), fn, ext;
	int	   texture_index = -1;
	bool   is_dds		 = false;
	auto   Pack			 = gEnv->pCryPak;
	auto   Ext			 = PathUtil::GetExt(filename.c_str());
	if (Ext[0] == 0)
	{
		path   = (string(filename.c_str()) + ".dds");
		is_dds = true;
	}
	else if (!strcmp(Ext, "dds"))
	{
		is_dds = true;
	}
	if (!(strcmp(Ext, "dss")))
	{
		strcpy(path.data() + path.size() - 3, "dds");
	}
	return path;
}

bool CD3DRenderer::FindTexture(string filename, CCryFile& file, string& adjustet_name)
{
	bool result	  = false;
	adjustet_name = AdjustTexturePath(filename);
	bool loaded	  = true;

	if (file.Open(adjustet_name.data(), "r"))
	{
		result = true;
	}
	else
	{
		loaded = false;
		string _file;
		PathUtil::Split(filename.c_str(), adjustet_name, _file);
		adjustet_name = adjustet_name + _file + ".jpg";
		result		  = file.Open(adjustet_name.data(), "r");
	}
	return result;
}

ID3DShaderResourceView* CD3DRenderer::CreateTexture(std::vector<uint8_t>& blob)
{
	int						texture_index = -1;
	ID3D11Resource*			pTexture{};
	ID3DShaderResourceView* pSRView = NULL;
	HRESULT					HResult{};

	HResult = DirectX::CreateDDSTextureFromMemory(
		m_pd3dDevice,
		&blob[0],
		blob.size(),
		&pTexture,
		&pSRView);
	return pSRView;
}
unsigned int CD3DRenderer::LoadTextureInternal(STexPic* pix, string fn, int* tex_type, unsigned int def_tid, bool compresstodisk, bool bWarn)
{
	auto& filename = pix->m_Name;
	if (auto it = m_LoadedTextureNames.find(filename); it != m_LoadedTextureNames.end())
	{
		return it->second;
	}

	int		 texture_index = -1;
	CCryFile file;

	string adjustet_name;
	if (!FindTexture(filename, file, adjustet_name))
	{
		CryError("Failed open texture: %s", filename.c_str());
	}
	else
	{
		//auto srv = CreateTextureFromFile(file);
		auto srv = CreateTextureFromFile(adjustet_name.data());
		if (srv)
		{
			CryLog("$3Loaded texture %s", filename.c_str());
			texture_index = AddTextureResource(filename, srv, pix);
		}
		else
		{
			CryError("Failed load texture: %s", filename.c_str());
		}
	}

	return texture_index;
}

unsigned int CD3DRenderer::LoadTexture(const char* filename, int* tex_type, unsigned int def_tid, bool compresstodisk, bool bWarn)
{
	_smart_ptr<STexPic> TexPic = new STexPic;
	TexPic->m_Id			   = NextTextureIndex();
	TexPic->m_Name			   = filename;
	m_RenderThread->ExecuteRenderThreadCommand([=]
											   { LoadTextureInternal(TexPic, string(filename)); });
	return TexPic->GetTextureID();
}

int CD3DRenderer::AddTextureResource(string name, ID3DShaderResourceView* pSRView, STexPic* pic)
{
	//auto texture_index = NextTextureIndex();
	auto texture_index = pic->GetTextureID();
	{
		ID3DTexture2D* pTexture2D;
		pSRView->GetResource((ID3DResource**)&pTexture2D);
		//pTexture2D->GetDesc(&desc);
		m_TexturesMap[texture_index] = std::make_pair(pTexture2D, pSRView);
		m_LoadedTextureNames[name]	 = texture_index;

		D3D11_TEXTURE2D_DESC desc;
		pTexture2D->GetDesc(&desc);
		pic = new STexPic(CD3D11_TEXTURE2D_DESC(desc), texture_index, name.c_str());
		//pic->m_Desc	  = CD3D11_TEXTURE2D_DESC(desc);
		//pic->m_Id	  = (texture_index);
		//pic->m_Name	  = (name);
		//pic->m_Loaded = (true);

		m_TexPics[texture_index] = *pic;
	}
	return texture_index;
}

void CD3DRenderer::RemoveTexture(unsigned int TextureId)
{
	NOT_IMPLEMENTED;
}

void CD3DRenderer::RemoveTexture(ITexPic* pTexPic)
{
	NOT_IMPLEMENTED;
}

int CD3DRenderer::CreateEmptyTexture(vector2di size, color4f color, DXGI_FORMAT format, UINT bindFlags)
{
	// Create the render target texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width			  = size.x;
	desc.Height			  = size.y;
	desc.MipLevels		  = 1;
	desc.ArraySize		  = 1;
	desc.Format			  = format;
	desc.SampleDesc.Count = 1;
	desc.Usage			  = D3D11_USAGE_DEFAULT;
	desc.BindFlags		  = bindFlags;

	ID3DTexture2D* pTexture = NULL;
	m_pd3dDevice->CreateTexture2D(&desc, NULL, &pTexture);

	// Create the shader-resource view
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format					 = desc.Format;
	srDesc.ViewDimension			 = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels		 = 1;

	ID3DShaderResourceView* pShaderResView = NULL;
	m_pd3dDevice->CreateShaderResourceView(pTexture, &srDesc, &pShaderResView);

#if 0
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	::GetDeviceContext()->Map(pTexture, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_WRITE_DISCARD, 0, &mappedTex);

	UCHAR* pTexels = (UCHAR*)mappedTex.pData;
	for (UINT row = 0; row < desc.Height; row++)
	{
		UINT rowStart = row * mappedTex.RowPitch;
		for (UINT col = 0; col < desc.Width; col++)
		{
			UINT colStart					 = col * 4;
			pTexels[rowStart + colStart + 0] = 255; // Red
			pTexels[rowStart + colStart + 1] = 128; // Green
			pTexels[rowStart + colStart + 2] = 64;	// Blue
			pTexels[rowStart + colStart + 3] = 32;	// Alpha
		}
	}

	::GetDeviceContext()->Unmap(pTexture, D3D11CalcSubresource(0, 0, 1));
#endif

	return 0;
}

int CD3DRenderer::NextTextureIndex()
{
	return m_NumLoadedTextures++;
}

void* CD3DRenderer::EF_Query(int Query, int Param)
{
	auto query = ERenderQueryTypes(Query);
	switch (query)
	{
	case EFQ_NUMEFS:
		break;
	case EFQ_LOADEDEFS:
		break;
	case EFQ_NUMTEXTURES:
		break;
	case EFQ_LOADEDTEXTURES:
		break;
	case EFQ_NUMEFFILES0:
		break;
	case EFQ_NUMEFFILES1:
		break;
	case EFQ_EFFILENAMES0:
		break;
	case EFQ_EFFILENAMES1:
		break;
	case EFQ_VProgramms:
		break;
	case EFQ_PShaders:
		break;
	case EFQ_LightSource:
		break;
	case EFQ_RecurseLevel:
		break;
	case EFQ_Pointer2FrameID:
		break;
	case EFQ_RegisteredTemplates:
		break;
	case EFQ_NumRenderItems:
		break;
	case EFQ_DeviceLost:
		break;
	case EFQ_CubeColor:
		break;
	case EFQ_D3DDevice:
		return GetDevice();
		break;
	case EFQ_glReadPixels:
		break;
	case EFQ_Orients:
		break;
	case EFQ_NumOrients:
		break;
	case EFQ_SkyShader:
		break;
	case EFQ_SunFlares:
		break;
	case EFQ_CurSunFlare:
		break;
	case EFQ_Materials:
		break;
	case EFQ_LightMaterials:
		break;
	default:
		break;
	}
	return nullptr;
}

void CD3DRenderer::Draw2DQuad(float x, float y, float w, float h, int texture, color4f color, float s0, float t0, float s1, float t1)
{
	glm::mat4	 projection = glm::ortho(0.0f, (float)GetWidth(), (float)GetHeight(), 0.0f);
	Legacy::Vec4 cur_c		= Legacy::Vec4(texture != -1 ? Legacy::Vec4{1} : Legacy::Vec4{color.r, color.g, color.b, color.a});

	auto screen_size(Legacy::Vec2(GetWidth(), GetHeight()));
	auto xpos = x;
	auto ypos = y;
	/*
		Coordinates of quad
		A---D 
		|	|
		B---C
		*/
	// Update VBO for each character
	using P3F_T2F = SVF_P3F_C4B_T2F;
#if 0
	Legacy::Vec2 uv_pos	 = Legacy::Vec2(x, y) / screen_size;
	Legacy::Vec2 uv_size = Legacy::Vec2(w, h) / screen_size;
#else
	Legacy::Vec2 uv_pos	 = Legacy::Vec2(s0, t0);
	Legacy::Vec2 uv_size = Legacy::Vec2(s1, t1);
#endif

	Legacy::Vec4 pA, pB, pC, pD;
	pA = Legacy::Vec4(Legacy::Vec3{xpos, ypos, 0}, 1.f);
	pB = Legacy::Vec4(Legacy::Vec3{xpos, ypos + h, 0}, 1.f);
	pC = Legacy::Vec4(Legacy::Vec3{xpos + w, ypos + h, 0}, 1.f);
	pD = Legacy::Vec4(Legacy::Vec3{xpos + w, ypos, 0}, 1.f);

#if 0
	Legacy::Vec2 
		tA{uv_pos.x, uv_pos.y},
		tB{uv_pos.x, uv_pos.y + uv_size.y},
		tD{uv_pos.x + uv_size.x, uv_pos.y + uv_size.y},
		tC{uv_pos.x + uv_size.x, uv_pos.y};
#else
	Legacy::Vec2
		tA{uv_pos.x, uv_pos.y},
		tB{uv_pos.x, uv_pos.y + uv_size.y},
		tD{uv_pos.x + uv_size.x, uv_pos.y},
		tC{uv_pos.x + uv_size.x, uv_pos.y + uv_size.y};
#endif

	std::array<P3F_T2F, 6> vertices = {
		P3F_T2F{Legacy::Vec3(projection * pA), UCol((cur_c)), tA},
		P3F_T2F{Legacy::Vec3(projection * pB), UCol((cur_c)), tB},
		P3F_T2F{Legacy::Vec3(projection * pC), UCol((cur_c)), tC},

		P3F_T2F{Legacy::Vec3(projection * pC), UCol((cur_c)), tC},
		P3F_T2F{Legacy::Vec3(projection * pD), UCol((cur_c)), tD},
		P3F_T2F{Legacy::Vec3(projection * pA), UCol((cur_c)), tA},
	};

	if (!GlobalResources::TexturedQuadShader)
	{
		GlobalResources::TexturedQuadShader = (CShader*)gEnv->pRenderer->Sh_Load("sprite.TexturedQuad", 0, 0);
		//auto GrayScaleShader = (CShader*)gEnv->pRenderer->Sh_Load("PostProcess.GrayScale", 0, 0);
	}

	auto vertex_cnt = 6;
	if (!GlobalResources::TexturedQuadShader || !vertex_cnt)
	{
		return;
	}
	// Activate corresponding render state
	auto VB = CreateBuffer(vertex_cnt, VERTEX_FORMAT_P3F_C4B_T2F, "Font", false);

	// Render glyph texture over quad
	// Update content of VBO memory
	UpdateBuffer(VB, vertices.data(), vertex_cnt, false);

	ID3DShaderResourceView* currentTexture = m_TexturesMap[texture].second;

	GlobalResources::TexturedQuadShader->Bind();
	m_pImmediateContext->PSSetSamplers(0, 1, &GlobalResources::LinearSampler);
	m_pImmediateContext->PSSetShaderResources(0, 1, &currentTexture);
#if 0
	m_pImmediateContext->IASetInputLayout(GlobalResources::VERTEX_FORMAT_P3F_C4B_T2F_Layout);
#endif
	m_pImmediateContext->RSSetState(m_pRasterizerState);
	m_pImmediateContext->OMSetBlendState(GlobalResources::FontBlendState, 0, 0xffffffff);
	//m_pd3dDevice->OMSetDepthStencilState(m_pDSState, 0);

	gEnv->pRenderer->DrawBuffer(VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, vertex_cnt);
	ReleaseBuffer(VB);
}

bool CD3DRenderer::CreateRenderTargets()
{
	auto [ok, rt] = RenderTarget::Create(GetWidth(), GetHeight());
	return ok;
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return gcpRendD3D;

	//  // Initializations:  //  // Here is a set of vertex definitions to support two streams.  // Vertex format:  //     stream 0 -> position + normal + color 0 + color 1  //     stream 1 -> 4 texture coordinate pairs
	struct VTXSTREAM_0
	{
		float fPosX, fPosY, fPosZ;
		float fNormX, fNormY, fNormZ;
		DWORD dwColor0, dwColor1;
	};
	struct VTXSTREAM_1
	{
		float fTexU0, fTexV0;
		float fTexU1, fTexV1;
		float fTexU2, fTexV2;
		float fTexU3, fTexV3;
	}; // Vertex declaration
#if 0
	D3DVERTEXELEMENT9 m_VtxDcl[] = { {0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
									 // stream 0, position
									 {0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
									 // stream 0, normal
									 {0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
									 // stream 0, color 0
									 {0, 28, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
									 // stream 0, color 1
									 {1, 0, D3DDECLUSAGE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
									 // stream 1, tex coord set 0
									 {1, 8, D3DDECLUSAGE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
									 // stream 1, tex coord set 1
									 {1, 16, D3DDECLUSAGE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
									 // stream 1, tex coord set 2
									 {1, 24, D3DDECLUSAGE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3}
									 // stream 1, tex coord set 3    }
									 // Create a vertex declaration object
									 LPDIRECT3DVERTEXDECLARATION9 m_pVtxDeclObject;
	m_pd3dDevice->CreateVertexDeclaration(m_VtxDcl, &m_pVtxDclObj);
#endif
}
