#include "pch.hpp"
#include "Renderer.h"
#include "DDSTextureLoader11.h"
#include <BlackBox/Core/Path.hpp>
#include <BlackBox/System/File/CryFile.h>
#include "AuxRenderer.hpp"

#include "Common/Include_HLSL_CPP_Shared.h"

#include "RenderThread.h"

#define EDITOR (Env::Get()->IsEditing())
// Globals
ID3DShaderResourceView* GlobalResources::FontAtlasRV{};
ID3D11SamplerState*     GlobalResources::LinearSampler{};

ID3DShaderResourceView* GlobalResources::WiteTextureRV;
ID3DShaderResourceView* GlobalResources::GreyTextureRV;

static CD3DRenderer     _gcpRendD3D;
CD3DRenderer*           gcpRendD3D = _gcpRendD3D;

ID3DInputLayout*        GlobalResources::VERTEX_FORMAT_P3F_C4B_T2F_Layout;

_smart_ptr<CShader>     GlobalResources::TexturedQuadShader;
_smart_ptr<CShader>     GlobalResources::SpriteShader;

_smart_ptr<ID3D11BlendState> g_pBlendState;

namespace util
{
	template<class T>
	_smart_ptr<T> make_smart_ptr()
	{
		return _smart_ptr<T>(new T);
	}
} // namespace util

struct ITechniqueManager;
CDevice* GetDevice()
{
	return gcpRendD3D->GetDevice().get();
}

ID3DDevice* GetD3DDevice()
{
	return gcpRendD3D->GetD3DDevice();
}

ID3DDeviceContext* GetDeviceContext()
{
	return gcpRendD3D->GetDeviceContext();
}

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
CD3DRenderer* gD3DRender;

namespace utils
{
} // namespace utils

CD3DRenderer::CD3DRenderer()
    : CRenderer()
{
	gD3DRender           = this;
	CRenderer::m_Backend = RenderBackend::DX;
}

CD3DRenderer::~CD3DRenderer()
{
	for (auto t : m_TexturesMap)
	{
		SAFE_RELEASE(t.second.first);
		SAFE_RELEASE(t.second.second);
	}
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
	m_pSwapChain->Get()->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	GetD3DDevice()->CreateRenderTargetView(pBackBuffer, NULL, m_pMainRenderTargetView.GetAddressOf());
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
	//CryLog("load shader: %s", name);
	//return gShMan->Sh_Load(name, flags, nMaskGen);

	return CRenderer::Sh_Reload();
}

IWindow* CD3DRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	m_LigthsList.resize(4);
	m_LigthsList[0] = {Legacy::Vec3{-10.0f, 10.0f, 10.0f},  Legacy::Vec3{300, 300, 900}};
	m_LigthsList[1] = {Legacy::Vec3{10.0f, 10.0f, 10.0f},   Legacy::Vec3{300, 300, 300}};
	m_LigthsList[2] = {Legacy::Vec3{-10.0f, -10.0f, 10.0f}, Legacy::Vec3{300, 300, 300}};
	m_LigthsList[3] = {Legacy::Vec3{-10.0f, -10.0f, 10.0f}, Legacy::Vec3{300, 300, 300}};
	if(auto result = CRenderer::Init(x, y, width, height, cbpp, zbpp, sbits, fullscreen, window); result)
	{
		g_pBlendState = GetDevice()->CreateBlendState(
			D3D11_BLEND_DESC
			{
				.AlphaToCoverageEnable = false,
				.IndependentBlendEnable = false,
				.RenderTarget = {
					{
						.BlendEnable = true,
						.SrcBlend = D3D11_BLEND_SRC_ALPHA,
						.DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
						.BlendOp = D3D11_BLEND_OP_ADD,
						.SrcBlendAlpha = D3D11_BLEND_ONE,
						.DestBlendAlpha = D3D11_BLEND_ZERO,
						.BlendOpAlpha = D3D11_BLEND_OP_ADD,
						.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
					}
				}
			}
		);

		return result;
	}
	return nullptr;
}

bool CD3DRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

static bool whiteTextureLoaded = false;

void CD3DRenderer::BeginFrame(void)
{
	if (!whiteTextureLoaded)
	{
		auto id = LoadTexture("Textures/white");

		auto& pT = m_TexturesMap[id];
		m_TexturesMap[-1] = pT;
		whiteTextureLoaded = true;
	}
	auto pDC = m_Device->Get<ID3D11DeviceContext>();

	StateManager().m_StateManager.OnBeginFrame();
	if (EDITOR)
		pDC->OMSetRenderTargets(1, m_RenderTargetScene->m_renderTargetView.GetAddressOf(), static_cast<ID3D11DepthStencilView*>(m_DepthStencil->m_pView));
	else
		StateManager().m_StateManager.SetRenderTargets({m_pMainRenderTargetView.GetAddressOf(), 1}, static_cast<ID3D11DepthStencilView*>(m_DepthStencil->m_pView));

	if (EDITOR)
		pDC->ClearRenderTargetView(m_RenderTargetScene->m_renderTargetView, &m_ClearColor[0]);
	else
		pDC->ClearRenderTargetView(m_pMainRenderTargetView, &m_ClearColor[0]);
	pDC->ClearDepthStencilView(static_cast<ID3D11DepthStencilView*>(m_DepthStencil->m_pView), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void CD3DRenderer::UpdateConstants()
{
	//D3DPERF_BeginEvent(D3DC_Blue, L"UpdateConstants");
	ScopedMap<SPerFrameConstantBuffer>(&(*m_PerFrameConstants), [&](auto pConstData)
	                                   {
		                                   pConstData->SunDirection    = Legacy::Vec4(glm::normalize(Legacy::Vec3(2, 3, 4)), 1.f);
		                                   pConstData->SunColor        = {r_SunColor, 1};
		                                   pConstData->AmbientStrength = Legacy::Vec4(1, 1, 1, 1) * 0.3f;
		                                   pConstData->NumLights       = m_LigthsList.size();
		                                   pConstData->LightIntensity  = Legacy::Vec4(1, 1, 1, 1); });

	ScopedMap<SPerViewConstantBuffer>(&(*m_PerViewConstants), [&](auto pConstData)
	                                  {
		                                  auto Projection            = m_Camera.GetProjectionMatrix();
		                                  auto View                  = m_Camera.GetViewMatrix();
		                                  pConstData->Projection     = Projection;
		                                  pConstData->ViewProjection = Projection * View;
		                                  pConstData->View           = View; 
		                                  pConstData->Eye            = m_Camera.GetPos(); });
	ScopedMap<HLSL_Light>(&(*m_Lights), [&](auto pConstData)
	                      { memcpy(pConstData, &m_LigthsList[0], m_LigthsList.size() * sizeof HLSL_Light); });

	ID3DBuffer* pBuffers[] = {
	    m_PerFrameConstants,
	    m_Lights,
	    m_PerViewConstants,
	};
	constexpr auto StartSlot = PERFRAME_SLOT;
	::GetDeviceContext()->VSSetConstantBuffers(StartSlot, 3, pBuffers);
	::GetDeviceContext()->PSSetConstantBuffers(StartSlot, 3, pBuffers);
	//D3DPERF_EndEvent();
}

void CD3DRenderer::Update(void)
{
	if (CV_r_ShadersReload)
	{
		Sh_Reload();
		CV_r_ShadersReload = 0;
	}
	{
		D3DPERF_BeginEvent(D3DC_Blue, L"BeginScene");
		{
			m_FrameID++;
			UpdateConstants();

			StateManager().SetDepthStencilState(m_pDepthStencilState);
			{
				{
					StateManager().SetBlendState(g_pBlendState);
					if (IConsole* pConsole = Env::System()->GetIConsole())
					{
						D3DPERF_BeginEvent(D3DC_Blue, L"DrawConsole");
						pConsole->Draw();
						D3DPERF_EndEvent();
					}
				}
				if (m_FrameID > 20)
				{
#if 0
					ImGui::Begin("View");
					ImGui::Image(ImTextureID(m_RenderTargetScene->m_shaderResourceView.Get()), ImGui::GetContentRegionAvail());
					ImGui::End();
#endif
				}
			}
			{
				m_RenderAuxGeom->Flush();
				D3DPERF_BeginEvent(D3DC_Blue, L"DrawImages");
				for (auto img : m_DrawImages)
				{
					Draw2DQuad(img.x, img.y, img.w, img.h, img.id, img.color, img.s0, img.t0, img.s1, img.t1);
				}
				D3DPERF_EndEvent();
				Flush();
			}
			//{
			//	D3DPERF_BeginEvent(D3DC_Blue, L"DrawConsole");
			//	if (IConsole* pConsole = Env::System()->GetIConsole())
			//		pConsole->Draw();
			//	D3DPERF_EndEvent();
			//}
		}
		D3DPERF_EndEvent();
		m_DrawImages.clear();
	}

	m_pSwapChain->Present(0, 0);
}

void CD3DRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
	uint           n = 1;
	D3D11_VIEWPORT viewports;
	ZeroStruct(viewports);
	m_Device->Get<ID3D11DeviceContext>()->RSGetViewports(&n, &viewports);
	*x      = (int)viewports.TopLeftX;
	*y      = (int)viewports.TopLeftY;
	*width  = (int)viewports.Width;
	*height = (int)viewports.Height;
}

void CD3DRenderer::SetViewport(int x, int y, int width, int height)
{
	CD3D11_VIEWPORT viewports(float(x), float(y), float(width), float(height), 0.f, 1.f);
	m_Device->Get<ID3D11DeviceContext>()->RSSetViewports(1, &viewports);
}

void CD3DRenderer::SetScissor(int x, int y, int width, int height)
{
}

void CD3DRenderer::Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs)
{
	Env::AuxGeomRenderer()->DrawAABB(mins, maxs, UCol{});
}

bool CD3DRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CD3DRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width    = (float)width;
	vp.Height   = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = (float)x;
	vp.TopLeftY = (float)y;
	OnResizeSwapchain(width - x, height - y);
	m_Device->Get<ID3D11DeviceContext>()->RSSetViewports(1, &vp);
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

	m_Device   = std::make_shared<CDevice>((HWND)m_Window->getNativeHandle());
	if (!m_Device->Create(nullptr))
	{
		return false;
	}

	CryLog("Created D3D device with feature level: %s", (const char*)m_Device->m_FeatureLevel);
	m_pSwapChain = CSwapChain::Create(m_Device, (HWND)m_Window->getNativeHandle());

	// Set up rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode              = D3D11_CULL_NONE;
	rasterizerDesc.FillMode              = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias             = false;
	rasterizerDesc.DepthBiasClamp        = 0;
	rasterizerDesc.SlopeScaledDepthBias  = 0;
	rasterizerDesc.DepthClipEnable       = true;
	rasterizerDesc.ScissorEnable         = false;
	rasterizerDesc.MultisampleEnable     = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	m_pRasterizerState = GetDevice()->CreateRasterizerState(rasterizerDesc);
	StateManager().SetRasterizerState(m_pRasterizerState);

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroStruct(desc);
		//desc.BackFace
		desc.DepthEnable    = true;
		desc.StencilEnable  = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc      = D3D11_COMPARISON_LESS;

		m_pDepthStencilState = m_Device->CreateDepthStencilState(desc);
	}

	m_DepthStencil = std::make_shared<CTexture>();
	ChangeViewport(0, 0, GetWidth(), GetHeight());

	//Legacy::Vec3 c = Legacy::Vec3(2, 162, 246) / 255.f;
	Legacy::Vec3 c = Legacy::Vec3(0, 0, 0) / 255.f;
	//Legacy::Vec3 c = Legacy::Vec3(50, 50, 50) / 255.f;
	SetClearColor(c);

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth      = Memory::AlignedSizeCB<SPerViewConstantBuffer>::value;
	cbDesc.Usage          = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags      = 0;

	hr                    = DEVICE->CreateBuffer(&cbDesc, NULL, this->m_PerViewConstants.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	cbDesc.ByteWidth = Memory::AlignedSizeCB<SPerFrameConstantBuffer>::value;
	hr               = DEVICE->CreateBuffer(&cbDesc, NULL, this->m_PerFrameConstants.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}
	static_assert((Memory::AlignedSizeCB<HLSL_Light>::value % sizeof Legacy::Vec4) == 0, "Bad alignment!!!");
	cbDesc.ByteWidth = 4 * Memory::AlignedSizeCB<HLSL_Light>::value;
	hr               = DEVICE->CreateBuffer(&cbDesc, NULL, this->m_Lights.GetAddressOf());
	if (FAILED(hr))
	{
		return hr;
	}

#ifdef MULTITHREADED_RENDER
	m_RenderThread = std::make_unique<SRenderThread>();

	m_RenderThread->Start();
#endif

	return true;
}

bool CD3DRenderer::OnResizeSwapchain(int newWidth, int newHeight)
{
	gcpRendD3D->CleanupRenderTarget();

	if (m_pSwapChain->OnResize(newWidth, newHeight, m_pMainRenderTargetView.GetAddressOf()))
	{
		if (!CreateRenderTargets())
		{
			CryFatalError("Cannot create render target");
		}
		return CreateDepthStencil(newWidth, newHeight);
	}
	return false;
}

void CD3DRenderer::GetMemoryUsage(ICrySizer* Sizer) const
{
	Sizer->AddObject(this, sizeof(*this));
}

void CD3DRenderer::Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float angle, float r, float g, float b, float a, float z)
{
#if 0
	if (m_Is2DMode)
	{
		s0 *= ortho.x;
		s1 *= ortho.x;

		t0 *= ortho.y;
		t1 *= ortho.y;
	}
#endif
	xpos = (float)ScaleCoordX(xpos); w = (float)ScaleCoordX(w);
	ypos = (float)ScaleCoordY(ypos); h = (float)ScaleCoordY(h);
	if ((s0 == 0 && t0 == 0 && s1 == 0 && t1 == 0) || (s1==1.f && t1 ==1.f))
	{
		s1 = 1;
		t1 = 1;
	}
	else
	{
		t0 = 1 - t0;
		t1 = 1 - t1;
	}
	m_DrawImages.push_back({xpos, ypos, w, h, texture_id, s0, t0, s1, t1, color4f{r, g, b, a}, z});

#if 1
	Image2D img = m_DrawImages.back();
	Draw2DQuad(img.x, img.y, img.w, img.h, img.id, img.color, img.s0, img.t0, img.s1, img.t1);
	m_DrawImages.pop_back();
#endif
}

auto static NulTex = STexPic();
ITexPic* CD3DRenderer::EF_GetTextureByID(int Id)
{
	ITexPic* result{&NulTex};
	auto     it = m_TexPics.find(Id);
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
		StateManager().m_StateManager.SetResources({ &t, 1 }, 0);
		//::GetDeviceContext()->PSSetShaderResources(0, 1, &t);
	}
}

static TextureFormat GetTextureFormat(const char* filename)
{
	auto ext = PathUtil::GetExt(filename);
	if (!strcmp(ext, "dds"))
	{
		return eTextureFormat_DDS;
	}
	else if (!strcmp(ext, "jpg"))
	{
		return eTextureFormat_JPG;
	}
	return eTextureFormat_Unknown;
}

ID3DShaderResourceView* CD3DRenderer::CreateTextureFromFile(CCryFile file)
{
	file.Seek(0, SEEK_END);
	auto texture_size = file.GetPosition();
	file.Seek(0, SEEK_SET);
	std::vector<uint8_t> blob(texture_size);
	file.Read(&blob[0], texture_size);
	return CreateTexture(blob, file.GetFilename(), GetTextureFormat(file.GetFilename()));
}

ID3DShaderResourceView* CD3DRenderer::CreateTextureFromFile(const char* name)
{
	std::string_view str(name);
	std::wstring     wstr(str.begin(), str.end());
	auto             result = m_Device->CreateDDSTextureFromFile(
	                wstr.data());
	//if (FAILED(HResult))
	//{
	//}
	return std::get<ID3DShaderResourceView*>(result);
}

string CD3DRenderer::AdjustTexturePath(string filename)
{
	string path(filename.c_str()), fn, ext;
	int    texture_index = -1;
	bool   is_dds        = false;
	auto   Pack          = Env::CryPak();
	auto   Ext           = PathUtil::GetExt(filename.c_str());
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

bool CD3DRenderer::FindTexture(string filename, CCryFile& file, string& resultName)
{
	bool result   = false;
	resultName = AdjustTexturePath(filename);
	bool loaded   = true;

	if (file.Open(resultName.data(), "r"))
	{
		result = true;
	}
	else
	{
		loaded = false;
		string _file;
		PathUtil::Split(filename.c_str(), resultName, _file);
		auto ext = PathUtil::GetExt(_file.data());
		if (ext[0] == 0)
		{
			ext = ".jpg";
		}
		resultName = resultName + _file + ext;
		result        = file.Open(resultName.data(), "r");
	}
	return result;
}

ID3DShaderResourceView* CD3DRenderer::CreateTexture(std::vector<uint8_t>& blob, const char* fileName,  TextureFormat format)
{
	int                     texture_index = -1;
	ID3D11Resource*         pTexture{};
	ID3DShaderResourceView* pSRView = NULL;
	HRESULT                 HResult{};

	if (format == eTextureFormat_DDS)
	{
		HResult = m_Device->CreateDDSTextureFromMemory(
				&blob[0],
				blob.size(),
				&pTexture,
				&pSRView);
	}
	else
	{
		HResult = m_Device->CreateTextureFromJPGFile(
			fileName, 
			&blob[0],
			blob.size(),
			&pTexture, &pSRView);
	}
	return pSRView;
}
unsigned int CD3DRenderer::LoadTextureInternal(STexPic* pix, string fn, int* tex_type, unsigned int def_tid, bool compresstodisk, bool bWarn)
{
	auto& filename = pix->m_Name;
	if (auto it = m_LoadedTextureNames.find(filename); it != m_LoadedTextureNames.end())
	{
		auto p = m_TexPics[it->second];
		pix->m_Id = it->second;
		pix->AddRef();
		pix->Width = p.Width;
		pix->Height = p.Height;
		pix->ArraySize = p.ArraySize;
		pix->BindFlags = p.BindFlags;
		pix->m_Loaded = true;
		return it->second;
	}

	int      texture_index = -1;
	CCryFile file;

	string   adjustet_name;
	if (!FindTexture(filename, file, adjustet_name))
	{
		CryError("Failed open texture: %s", filename.c_str());
	}
	else
	{
		auto srv = CreateTextureFromFile(std::move(file));
		//auto srv = CreateTextureFromFile(resultName.data());
		if (srv)
		{
			//CryLog("$3Loaded texture %s", filename.c_str());
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
	auto id										 = NextTextureIndex();
	TexPic->m_Id               = id;
	TexPic->m_Name             = filename;
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
		m_LoadedTextureNames[name]   = texture_index;

		D3D11_TEXTURE2D_DESC desc;
		pTexture2D->GetDesc(&desc);
		auto d                   = CD3D11_TEXTURE2D_DESC(desc);
		pic                      = new STexPic(d, texture_index, name.c_str());
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
	if (auto result = m_Device->CreateEmptyTexture(size, color, format, bindFlags); result == nullptr)
		return 0;
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
		return DEVICE;
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

ID3DDepthStencilState* m_pDSState;
static ID3DDepthStencilState* CreateDepthStencil()
{

	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroStruct(desc);
	//desc.BackFace
	desc.DepthEnable    = false;
	desc.StencilEnable  = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	m_pDSState = GetDevice()->CreateDepthStencilState(desc);
	return m_pDSState;
}

void CD3DRenderer::Draw2DQuad(float x, float y, float w, float h, ID3D11ShaderResourceView* view, color4f color, float s0, float t0, float s1, float t1)
{
	glm::mat4 projection = glm::ortho(0.0f, (float)GetWidth(), (float)GetHeight(), 0.0f);
	//auto      cur_c      = Legacy::Vec4(color.r, color.g, color.b, color.a);
	auto      screen_size(Legacy::Vec2(GetWidth(), GetHeight()));
	auto      xpos = x;
	auto      ypos = y;
	/*
		Coordinates of quad
		A---D 
		|	|
		B---C
		*/
	// Update VBO for each character
	using P3F_T2F  = SVF_P3F_C4B_T2F;
#if 0
	Legacy::Vec2 uv_pos	 = Legacy::Vec2(x, y) / screen_size;
	Legacy::Vec2 uv_size = Legacy::Vec2(w, h) / screen_size;
#else
	Legacy::Vec2 uv_pos  = Legacy::Vec2(s0, t0);
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

	UCol cur_c;
	cur_c.dcolor = color.pack32();
	std::array<P3F_T2F, 6> vertices = {
	    P3F_T2F{Legacy::Vec3(projection * pA), cur_c, tA},
	    P3F_T2F{Legacy::Vec3(projection * pB), cur_c, tB},
	    P3F_T2F{Legacy::Vec3(projection * pC), cur_c, tC},

	    P3F_T2F{Legacy::Vec3(projection * pC), cur_c, tC},
	    P3F_T2F{Legacy::Vec3(projection * pD), cur_c, tD},
	    P3F_T2F{Legacy::Vec3(projection * pA), cur_c, tA},
	};

	if (!GlobalResources::TexturedQuadShader)
	{
		GlobalResources::TexturedQuadShader = (CShader*)Env::Renderer()->Sh_Load("sprite.TexturedQuad", 0, 0);
		//auto GrayScaleShader = (CShader*)Env::Renderer()->Sh_Load("PostProcess.GrayScale", 0, 0);
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

	GlobalResources::TexturedQuadShader->Bind();
	//m_Device->Get<ID3D11DeviceContext>()->PSSetSamplers(0, 1, &GlobalResources::LinearSampler);
	StateManager().m_StateManager.SetSamplerState(GlobalResources::LinearSampler, 0);
	StateManager().m_StateManager.SetResources({ &view, 1 }, 0);
	//m_Device->Get<ID3D11DeviceContext>()->PSSetShaderResources(0, 1, &view);
#if 0
	m_Device->Get<ID3D11DeviceContext>()->IASetInputLayout(GlobalResources::VERTEX_FORMAT_P3F_C4B_T2F_Layout);
#endif
	static auto m_pDSState = ::CreateDepthStencil();
	StateManager().SetDepthStencilState(m_pDSState);
	StateManager().SetRasterizerState(m_pRasterizerState);
	StateManager().SetBlendState(g_pBlendState);

	Env::Renderer()->DrawBuffer(VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, vertex_cnt);
	ReleaseBuffer(VB);
}

void CD3DRenderer::Draw2DQuad(float x, float y, float w, float h, int texture, color4f color, float s0, float t0, float s1, float t1)
{
	//auto                      cur_c          = color4f(texture != -1 ? color4f{1, 1, 1, 1} : color4f{color.r, color.g, color.b, color.a});

	ID3D11ShaderResourceView* currentTexture = m_TexturesMap[texture].second;
	Draw2DQuad(x, y, w, h, currentTexture, color, s0, t0, s1, t1);
}

bool CD3DRenderer::CreateRenderTargets()
{
	auto [ok, rt] = RenderTarget::Create(GetWidth(), GetHeight());
	if (ok)
	{
		m_RenderTargetScene = rt;
	}
	return ok;
}

std::unique_ptr<CSwapChain> CSwapChain::Create(std::shared_ptr<CDevice> pDevice, HWND Hwnd)
{
	_smart_ptr<IDXGIFactory>   pFactory   = NULL;
	_smart_ptr<IDXGISwapChain> pSwapChain = NULL;
	// Create a DXGIFactory object.
	if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf())))
	{
		auto                 as               = CSwapChain::EnumerateAdapters(pFactory);
		auto                 a                = CSwapChain::SelectAdapter(as);

		DXGI_SWAP_CHAIN_DESC sd               = {0};
		sd.BufferCount                        = 2;
		sd.BufferDesc.Width                   = gcpRendD3D->GetWidth();
		sd.BufferDesc.Height                  = gcpRendD3D->GetHeight();
		sd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator   = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow                       = Hwnd;
		sd.SampleDesc.Count                   = 1;
		sd.SampleDesc.Quality                 = 0;
		sd.Windowed                           = TRUE;
		sd.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		pFactory->CreateSwapChain(pDevice->Get<ID3D11Device>(), &sd, pSwapChain.GetAddressOf());
		return std::make_unique<CSwapChain>(pDevice, pSwapChain);
	}

	return nullptr;
}

std::pair<bool, _smart_ptr<RenderTarget>> RenderTarget::Create(int textureWidth, int textureHeight)
{
	auto                 renderTarget = std::make_pair(false, _smart_ptr<RenderTarget>(new RenderTarget));
	auto& m_renderTargetTexture = renderTarget.second->m_renderTargetTexture;
	auto& m_renderTargetView = renderTarget.second->m_renderTargetView;
	auto& m_shaderResourceView = renderTarget.second->m_shaderResourceView;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = textureWidth;
	textureDesc.Height = textureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = GetD3DDevice()->CreateTexture2D(&textureDesc, NULL, m_renderTargetTexture.GetAddressOf());
	if (FAILED(result))
		return renderTarget;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = GetD3DDevice()->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, m_renderTargetView.GetAddressOf());
	if (FAILED(result))
		return renderTarget;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = GetD3DDevice()->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, m_shaderResourceView.GetAddressOf());
	if (FAILED(result))
		return renderTarget;

	renderTarget.first = true;
	return renderTarget;
}
