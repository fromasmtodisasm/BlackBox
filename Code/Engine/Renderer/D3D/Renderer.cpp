#include "Renderer.h"
#include "Renderer.h"
#include <BlackBox/Core/Path.hpp>
#include "DDSTextureLoader.h"
#include <BlackBox\System\File\CryFile.h>

// Globals
ID3D10ShaderResourceView* GlobalResources::FontAtlasRV{};
ID3D10SamplerState*		  GlobalResources::LinearSampler{};

ID3D10ShaderResourceView* GlobalResources::WiteTextureRV;
ID3D10ShaderResourceView* GlobalResources::GreyTextureRV;

ID3D10EffectTechnique* GlobalResources::BoxTechnique;
ID3D10EffectTechnique* GlobalResources::MeshTechnique;

ID3D10InputLayout* GlobalResources::VERTEX_FORMAT_P3F_C4B_T2F_Layout;

ID3D10BlendState* GlobalResources::FontBlendState;

_smart_ptr<CShader> GlobalResources::TexturedQuadShader;
_smart_ptr<CShader> GlobalResources::SpriteShader;

struct ITechniqueManager;
ID3D10Device* GetDevice()
{
	return CD3DRenderer::GetDevice(gEnv->pRenderer);
}

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
D3D10_DRIVER_TYPE g_driverType = D3D10_DRIVER_TYPE_NULL;
CD3DRenderer* gD3DRender;

CD3DRenderer::CD3DRenderer(ISystem* pSystem) : CRenderer(pSystem)
{
	gD3DRender			 = this;
	CRenderer::m_Backend = RenderBackend::DX;
}

CD3DRenderer::~CD3DRenderer()
{
	SAFE_RELEASE(m_pRenderTargetView);
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
	return CRenderer::Init(x,y,width, height, cbpp, zbpp, sbits, fullscreen, window);
}

bool CD3DRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CD3DRenderer::BeginFrame(void)
{
	m_pd3dDevice->ClearRenderTargetView(m_pRenderTargetView, &m_ClearColor[0]);
	m_pd3dDevice->ClearDepthStencilView(m_pDepthStencilView, D3D10_CLEAR_DEPTH, 1.f, 0);
}

void CD3DRenderer::Update(void)
{
	m_FrameID++;	

	::GetDevice()->OMSetDepthStencilState(m_pDepthStencilState, 0);
    Flush();
	for (auto img : m_DrawImages)
	{
		Draw2DQuad(img.x, img.y, img.w, img.h, img.id, img.color, img.s0, img.t0, img.s1, img.t1);
	}
	for (const auto& rcl : m_RenderCallbackClients)
	{
		rcl->OnRenderer_BeforeEndFrame();	
	}
	if (IConsole* pConsole = gEnv->pSystem->GetIConsole())
		pConsole->Draw();

	m_DrawImages.clear();

    m_pSwapChain->Present( 0, 0 );
}

void CD3DRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
	uint n = 1;
	D3D10_VIEWPORT viewports;
	m_pd3dDevice->RSGetViewports(&n, &viewports);
	*x = viewports.TopLeftX;
	*y = viewports.TopLeftY;
	*width = viewports.Width;
	*height = viewports.Height;
}

void CD3DRenderer::SetViewport(int x, int y, int width, int height)
{
}

void CD3DRenderer::SetScissor(int x, int y, int width, int height)
{
}

void CD3DRenderer::Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs)
{
}

bool CD3DRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CD3DRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
    // Setup the viewport
    D3D10_VIEWPORT vp;
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = x;
    vp.TopLeftY = y;
    m_pd3dDevice->RSSetViewports( 1, &vp );
	OnResizeSwapchain(width - x, height - y);	
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
    //createDeviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif

    D3D10_DRIVER_TYPE driverTypes[] =
    {
        D3D10_DRIVER_TYPE_HARDWARE,
        D3D10_DRIVER_TYPE_REFERENCE,
    };
    UINT numDriverTypes = sizeof( driverTypes ) / sizeof( driverTypes[0] );

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
    sd.BufferCount = 2;
    sd.BufferDesc.Width = GetWidth();
    sd.BufferDesc.Height = GetHeight();
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = (HWND)m_Window->getNativeHandle();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D10CreateDeviceAndSwapChain( NULL, g_driverType, NULL, createDeviceFlags,
                                            D3D10_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice );
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
	{
		CryLog("Failed create D3D10 device");
        return false;
	}


    // Set up rasterizer
	D3D10_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode				 = D3D10_CULL_NONE;
	rasterizerDesc.FillMode				 = D3D10_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias			 = false;
	rasterizerDesc.DepthBiasClamp		 = 0;
	rasterizerDesc.SlopeScaledDepthBias	 = 0;
	rasterizerDesc.DepthClipEnable		 = true;
	rasterizerDesc.ScissorEnable		 = false;
	rasterizerDesc.MultisampleEnable	 = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
	m_pd3dDevice->RSSetState(m_pRasterizerState);

    {
		D3D10_DEPTH_STENCIL_DESC desc;
		//desc.BackFace
		desc.DepthEnable	= true;
		desc.StencilEnable	= false;
		desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc		= D3D10_COMPARISON_LESS ;

		::GetDevice()->CreateDepthStencilState(&desc, &m_pDepthStencilState);
		::GetDevice()->OMSetDepthStencilState(m_pDepthStencilState, 0);
	
    }

	/*if (!OnResizeSwapchain(GetWidth(), GetHeight()))
		return false;*/
	ChangeViewport(0, 0, GetWidth(), GetHeight());

    m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
    //m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);

	Legacy::Vec3 c = Legacy::Vec3(2, 162, 246) / 255.f;
	//Legacy::Vec3 c = Legacy::Vec3(0, 0, 0) / 255.f;
	SetClearColor(c);

    return true;
}

bool CD3DRenderer::OnResizeSwapchain(int newWidth, int newHeight)
{
	m_pSwapChain->ResizeBuffers(0, newWidth, newHeight, DXGI_FORMAT_UNKNOWN, 0);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);

    // Create a render target view
    ID3D10Texture2D* pBuffer;
    auto hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D10Texture2D ), ( LPVOID* )&pBuffer );
    if( FAILED( hr ) )
        return false;

    hr = m_pd3dDevice->CreateRenderTargetView( pBuffer, NULL, &m_pRenderTargetView );
    pBuffer->Release();
    if( FAILED( hr ) )
        return false;

    D3D10_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width				= newWidth;
	depthStencilDesc.Height				= newHeight;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count	= 1; // multisampling must match
	depthStencilDesc.SampleDesc.Quality = 0; // swap chain values.
	depthStencilDesc.Usage				= D3D10_USAGE_DEFAULT;
	depthStencilDesc.BindFlags			= D3D10_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags		= 0;
	depthStencilDesc.MiscFlags			= 0;

	hr = (m_pd3dDevice->CreateTexture2D(
		&depthStencilDesc, 0, &m_DepthStencilBuffer));
    if( FAILED( hr ) )
        return false;
	hr = (m_pd3dDevice->CreateDepthStencilView(
		m_DepthStencilBuffer, 0, &m_pDepthStencilView));
    if( FAILED( hr ) )
        return false;

    m_pd3dDevice->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

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

	Image2D img = m_DrawImages.back();
	Draw2DQuad(img.x, img.y, img.w, img.h, img.id, img.color, img.s0, img.t0, img.s1, img.t1);
	m_DrawImages.pop_back();
}

ITexPic* CD3DRenderer::EF_GetTextureByID(int Id)
{
	ITexPic* result{};
	auto it = m_TexPics.find(Id);
	if (it != m_TexPics.end())
	{
		result = &it->second;
	}
	return result;
}

ITexPic* CD3DRenderer::EF_LoadTexture(const char* nameTex, uint flags, uint flags2, byte eTT, float fAmount1, float fAmount2, int Id, int BindId)
{
	return EF_GetTextureByID(LoadTexture(nameTex));
}
void CD3DRenderer::SetTexture(int tnum, ETexType Type)
{
	auto t = m_TexturesMap[tnum].second;
	if (t)
	{
		::GetDevice()->PSSetShaderResources(0, 1, &t);	
	}
}
ID3D10ShaderResourceView* CD3DRenderer::CreateTextureFromFile(CCryFile file)
{
	file.Seek(0, SEEK_END);
	auto texture_size = file.GetPosition();
	file.Seek(0, SEEK_SET);
	std::vector<uint8_t> blob(texture_size);
	file.Read(&blob[0], texture_size);
	return CreateTexture(blob);

}

string CD3DRenderer::AdjustTexturePath(const char* filename)
{
	string path(filename), fn, ext;
	int	   texture_index = -1;
	bool   is_dds = false;
	auto   Pack	  = gEnv->pCryPak;
	auto Ext = PathUtil::GetExt(filename);
	if (Ext[0] == 0)
	{
		path = (string(filename) +".dds");
		is_dds = true;
	}
	else if (!strcmp(Ext, "dds") )
	{
		is_dds = true;	
	}
	if (!(strcmp(Ext,"dss")))
	{
		strcpy(path.data() + path.size() - 3, "dds");
	}
	return path;
}

bool CD3DRenderer::FindTexture(const char* filename, CCryFile& file)
{
	bool	 result		   = false;
	auto	 path		   = AdjustTexturePath(filename);
	bool	 loaded		   = true;

	if (file.Open(path.data(), "r"))
	{
		result = true;
	}
	else
	{
		loaded = false;
		string _file;
		PathUtil::Split(filename, path, _file);
		path = path + _file + ".jpg";
		result = file.Open(path.data(), "r");
	}
	return result;
}



ID3D10ShaderResourceView* CD3DRenderer::CreateTexture(std::vector<uint8_t> &blob)
{
	int					   texture_index = -1;
	D3DX10_IMAGE_LOAD_INFO loadInfo;
	ZeroMemory(&loadInfo, sizeof(D3DX10_IMAGE_LOAD_INFO));
	loadInfo.BindFlags = D3D10_BIND_SHADER_RESOURCE;
	ID3D10Resource*			  pTexture{};
	ID3D10ShaderResourceView* pSRView = NULL;
	HRESULT					  HResult{};

	//HResult = D3DX10CreateShaderResourceViewFromFile(
	HResult = D3DX10CreateShaderResourceViewFromMemory(
		m_pd3dDevice,
		&blob[0], 
		blob.size(),
		nullptr,
		nullptr,
		&pSRView,
		&HResult);
	return pSRView;

}

unsigned int CD3DRenderer::LoadTexture(const char* filename, int* tex_type, unsigned int def_tid, bool compresstodisk, bool bWarn)
{
	if (auto it = m_LoadedTextureNames.find(filename); it != m_LoadedTextureNames.end())
	{
		return it->second;
	}

	
	int		 texture_index = -1;
	CCryFile file;

	if (!FindTexture(filename, file))
	{
		CryError("Failed open texture: %s", filename);
	}
	else
	{
		auto srv = CreateTextureFromFile(file);
		if (srv)
		{
			CryLog("$3Loaded texture %s", filename);
			texture_index = AddTextureResource(filename, srv);
		}
		else
		{
			CryError("Failed load texture: %s", filename);
		}
	}

	return texture_index;
}

int CD3DRenderer::AddTextureResource(const char* name, ID3D10ShaderResourceView* pSRView)
{
	auto texture_index = NextTextureIndex();
	{
		ID3D10Texture2D* pTexture2D;
		pSRView->GetResource((ID3D10Resource**)&pTexture2D);
		//pTexture2D->GetDesc(&desc);
		m_TexturesMap[texture_index] = std::make_pair(pTexture2D, pSRView);
		m_LoadedTextureNames[name]	 = texture_index;

		D3D10_TEXTURE2D_DESC desc;
		pTexture2D->GetDesc(&desc);
		m_TexPics[texture_index] = STexPic(CD3D10_TEXTURE2D_DESC(desc), texture_index, name);

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

int CD3DRenderer::CreateEmptyTexture(vector2di size, color4f color)
{
	// Create the render target texture
	D3D10_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width			  = size.x;
	desc.Height			  = size.y;
	desc.MipLevels		  = 1;
	desc.ArraySize		  = 1;
	desc.Format			  = DXGI_FORMAT_R32G32B32A32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.Usage			  = D3D10_USAGE_DEFAULT;
	desc.BindFlags		  = /* D3D10_BIND_RENDER_TARGET |*/ D3D10_BIND_SHADER_RESOURCE;

	ID3D10Texture2D* pTexture = NULL;
	m_pd3dDevice->CreateTexture2D(&desc, NULL, &pTexture);

	// Create the shader-resource view
	D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
	srDesc.Format					 = desc.Format;
	srDesc.ViewDimension			 = D3D10_SRV_DIMENSION_TEXTURE2D;
	srDesc.Texture2D.MostDetailedMip = 0;
	srDesc.Texture2D.MipLevels		 = 1;

	ID3D10ShaderResourceView* pShaderResView = NULL;
	m_pd3dDevice->CreateShaderResourceView(pTexture, &srDesc, &pShaderResView);

	D3D10_MAPPED_TEXTURE2D mappedTex;
	pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);

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

	pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));

	return 0;
}

int CD3DRenderer::NextTextureIndex()
{
	return m_NumLoadedTextures++;
}

void *CD3DRenderer::EF_Query(int Query, int Param)
{
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
	using P3F_T2F		 = SVF_P3F_C4B_T2F;
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
		GlobalResources::TexturedQuadShader = (CShader*)gEnv->pRenderer->Sh_Load("sprite", 1, 0);
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

	ID3D10ShaderResourceView* currentTexture = m_TexturesMap[texture].second;
		

	GlobalResources::TexturedQuadShader->Bind();
	m_pd3dDevice->PSSetSamplers(0, 1, &GlobalResources::LinearSampler);
	m_pd3dDevice->PSSetShaderResources(0, 1, &currentTexture);
	m_pd3dDevice->IASetInputLayout(GlobalResources::VERTEX_FORMAT_P3F_C4B_T2F_Layout);
	m_pd3dDevice->RSSetState(m_pRasterizerState);
	m_pd3dDevice->OMSetBlendState(GlobalResources::FontBlendState, 0, 0xffffffff);
	//m_pd3dDevice->OMSetDepthStencilState(m_pDSState, 0);

	gEnv->pRenderer->DrawBuffer(VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, vertex_cnt);
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	pSystem->Log("Loading...");
	return new CD3DRenderer(pSystem);
}
