#include "BaseRenderer.hpp"
#include "BufferManager.hpp"
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/System/IProjectManager.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/Renderer/FreeTypeFont.hpp>

#include "AuxRendererNull.hpp"

#include <BlackBox/Renderer/TextureManager.hpp>
#include <SDL2/SDL.h>

#include <BlackBox/Core/Platform/platform_impl.inl>

#include <filesystem>
#pragma warning(push)
#pragma warning(disable : 4244)

int		   RenderCVars::CV_r_GetScreenShot;
static int dump_shaders_on_load = false;
FxParser*  g_FxParser;
ShaderMan* gShMan;
//FxParser s_FxParser;

class CNullFont : public IFont
{
  public:
	bool  Init(const char* font, unsigned int w, unsigned int h) override { return true; };
	void  RenderText(const std::string_view text, float x, float y, float scale, float color[4]) override{};
	float GetXPos() override { return 0.f; };
	float GetYPos() override { return 0.f; };
	void  SetXPos(float x) override{};
	void  SetYPos(float y) override{};

	float TextWidth(const std::string_view text) override { return 0.f; };
	float CharWidth(char ch) override { return 0.f; };
	void  Submit() override {}
	void  Release() override{};
};

void TestFx(IConsoleCmdArgs* args)
{
	string filename;
	if (args->GetArgCount() < 2)
		filename = "tmp/test.fx";
	else
		filename = args->GetArg(1);

	PEffect pEffect = nullptr;
#if 0
	if (g_FxParser->Parse(filename, &pEffect))
	{
		CryLog("Dumping shaders of effect: %s", filename.data());
		for (int i = 0; i < pEffect->GetNumShaders(); i++)
		{
			CryLog("[%s]", pEffect->GetShader(i).name.c_str());
		}
	}
#endif
}

CRenderer::CRenderer(ISystem* engine)
	: m_pSystem(engine)
	, m_viewPort(0, 0, 0, 0)
{
}

void CRenderer::Release()
{
	delete this;
}

bool CRenderer::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
	if (!strcmp(pVar->GetName(), "r_Width"))
	{
		gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_RESIZE, std::strtof(sNewValue, nullptr), GetHeight());
	}
	else if (!strcmp(pVar->GetName(), "r_Height"))
	{
		gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_RESIZE, GetWidth(), std::strtof(sNewValue, nullptr));
	}
	else if (!strcmp(pVar->GetName(), "r_Fullscreen"))
	{
		gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_TOGGLE_FULLSCREEN, std::strtol(sNewValue, nullptr, 10), 0);
	}
	return true;
}

void CRenderer::OnAfterVarChange(ICVar* pVar)
{
}

void CRenderer::OnVarUnregister(ICVar* pVar)
{
}

void CRenderer::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_CHANGE_FOCUS:
		break;
	case ESYSTEM_EVENT_MOVE:
		break;
	case ESYSTEM_EVENT_RESIZE:
		if (!transit_to_FS)
		{
			m_Window->changeSize(wparam, lparam);
			ChangeViewport(0, 0, wparam, lparam);
		}
		transit_to_FS = false;
		break;
	case ESYSTEM_EVENT_ACTIVATE:
		bIsActive = bool(wparam);
		break;
	case ESYSTEM_EVENT_LEVEL_LOAD_START:
		break;
	case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:
		break;
	case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:
		break;
	case ESYSTEM_EVENT_LANGUAGE_CHANGE:
		break;
	case ESYSTEM_EVENT_TOGGLE_FULLSCREEN:
		bInFullScreen = bool(wparam);
		m_Window->EnterFullscreen(bInFullScreen);
		break;
	case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:
		bIsActive = bool(wparam);
		break;
	default:
		break;
	}
}

CRenderer::~CRenderer()
{
	gEnv->pConsole->RemoveConsoleVarSink(this);
	for (size_t i = 0; i < m_Fonts.size(); i++)
	{
		m_Fonts[i]->Release();
	}
	ReleaseBuffer(m_VertexBuffer);
	SAFE_DELETE(m_RenderAuxGeom);
#ifndef VK_RENDERER
	SAFE_DELETE(m_BufferManager);
#endif
	SAFE_DELETE(m_VertexBuffer);

	delete gShMan;
}

IWindow* CRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	InitCVars();
	IWindow* result = m_Window = window;
	bInFullScreen			   = fullscreen;
	if (window == nullptr)
		return nullptr;
	//=======================
	InitConsoleCommands();
	//=======================
	const char* sGameName = gEnv->pSystem->GetIProjectManager()->GetCurrentProjectName();
	strcpy(m_WinTitle, sGameName);

	CryLog("Creating window called '%s' (%dx%d)", m_WinTitle, width, height);
	IWindow::SInitParams wp{m_WinTitle, x, y, width, height, cbpp, zbpp, sbits, fullscreen, m_Backend};
	if (!m_Window->init(&wp))
		return nullptr;
	CryLog("window inited");
#ifdef GL_RENDERER
	if (!OpenGLLoader())
		return nullptr;
	context = SDL_GL_GetCurrentContext();
	// now you can make GL calls.
	m_Window->swap();
#endif
	m_hWnd = (HWND)window->getHandle();
	if (!InitOverride())
		return false;
#if 0
	g_FxParser = &s_FxParser;
#else
	g_FxParser		= nullptr;
#endif
#if 0
	if (!InitResourceManagers())
		return nullptr;
	PrintHardware();
#endif

	m_pSystem->GetIConsole()->AddConsoleVarSink(this);
	m_pSystem->GetISystemEventDispatcher()->RegisterListener(this, "Renderer");
#ifndef VK_RENDERER
	m_BufferManager = new CBufferManager();
#endif
	CreateQuad();
	gShMan = new ShaderMan;
	//=======================
	//pd.vs.macro["STORE_TEXCOORDS"] = "1";
	if (!(m_ScreenShader = _smart_ptr((CShader*)(Sh_Load("screen", int(ShaderBinaryFormat::SPIRV))))))
	{
		m_pSystem->Log("Error of loading screen shader");
		return nullptr;
	}

#if 1
	m_RenderAuxGeom = new CRenderAuxGeom();
#else
	m_RenderAuxGeom = new CRenderAuxGeomNull();
#endif

	CreateRenderTarget();

	auto* dm = static_cast<SDL_DisplayMode*>(window->GetDesktopMode());
#if 0
	m_MainMSAAFrameBuffer = FrameBufferObject::create(dm->w, dm->h, m_RenderTargets.back(), false);
#endif

	{
#if 0
		char buffer[32];
		snprintf(buffer, 32, "rt_%zd", m_RenderTargets.size());
		dm = static_cast<SDL_DisplayMode*>(m_Window->GetDesktopMode());
		m_RenderTargets.push_back(Texture::create(
			Image(dm->w, dm->h, 3, std::vector<uint8_t>(), false),
			TextureType::LDR_RENDER_TARGET, buffer));
#endif
	}

#if 0
	m_MainReslovedFrameBuffer = FrameBufferObject::create(dm->w, dm->h, m_RenderTargets.back(), false);

	perViewBuffer = PerViewBuffer::Create(2);
	screenBuffer  = ScreenBuffer::Create(11);
#endif
	auto path = string("bin/shadercache");
	if (!std::filesystem::exists(path))
	{
		std::filesystem::create_directories(path);
	}

	return result;
}

int CRenderer::EnumDisplayFormats(SDispFormat* formats)
{
	static int		displayInUse = 0; /* Only using first display */
	int				numModes	 = 0;
	int				i			 = 0;
	SDL_DisplayMode mode;
	Uint32			f;

	gEnv->pLog->Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

	numModes = SDL_GetNumDisplayModes(displayInUse);
	if (numModes < 1)
	{
		gEnv->pLog->Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
		return 1;
	}
	gEnv->pLog->Log("SDL_GetNumDisplayModes: %i", numModes);

	if (formats != nullptr)
	{
		for (i = 0; i < numModes; ++i)
		{
			if (SDL_GetDisplayMode(displayInUse, i, &mode) != 0)
			{
				gEnv->pLog->Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
				return 1;
			}
			f = mode.format;

			formats[i].m_BPP	= SDL_BITSPERPIXEL(f);
			formats[i].m_Width	= mode.w;
			formats[i].m_Height = mode.h;
		}
	}

	return numModes;
}

void RenderCVars::InitCVars()
{
#if 0
	REGISTER_CVAR(r_Width, 800, VF_NULL, "Window Width");
	REGISTER_CVAR(r_Height, 600, VF_NULL, "Window Height");
	REGISTER_CVAR(r_Fullscreen, 600, VF_NULL);
	REGISTER_CVAR(r_Fullscreen, 600, VF_NULL);
#endif
	REGISTER_CVAR(r_MSAA, r_MSAA, VF_DUMPTODISK | VF_READONLY, "");
	REGISTER_CVAR(r_MSAA_samples, r_MSAA_samples, VF_DUMPTODISK, "");

	REGISTER_CVAR(r_Vsync, r_Vsync, VF_DUMPTODISK, "");
	REGISTER_CVAR(r_DisplayIndex, r_DisplayIndex, VF_DUMPTODISK, "");
	//REGISTER_CVAR3("r_GetScreenShot", CV_r_GetScreenShot, 0, VF_NULL,
	REGISTER_CVAR3("r_getscreenshot", CV_r_GetScreenShot, 0, VF_NULL,
				   "To capture one screenshot (variable is set to 0 after capturing)\n"
				   "0=do not take a screenshot (default), 1=save a screenshot (together with .HDR if enabled), 2=save a screenshot");
	REGISTER_CVAR(r_GraphicsDeviceId, r_GraphicsDeviceId, VF_DUMPTODISK, "");
}

RenderCVars::~RenderCVars()
{
}

CVertexBuffer* CRenderer::CreateBuffer(int vertexCount, int vertexFormat, const char* szSource, bool bDynamic /* = false*/)
{
#ifndef VK_RENDERER
	return m_BufferManager->Create(vertexCount, vertexFormat, szSource, bDynamic);
#else
	return nullptr;
#endif
}

void CRenderer::ReleaseBuffer(CVertexBuffer* bufptr)
{
#ifndef VK_RENDERER
	m_BufferManager->Release(bufptr);
#endif
}

void CRenderer::DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start /* = 0*/, int vert_stop /* = 0*/, CMatInfo* mi /* = NULL*/)
{
#ifndef VK_RENDERER
	m_BufferManager->Draw(src, indicies, numindices, offsindex, prmode, vert_start, vert_stop, mi);
#endif
}

void CRenderer::UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs /* = 0*/, int Type /* = 0*/)
{
#ifndef VK_RENDERER
	m_BufferManager->Update(dest, src, vertexcount, bUnLock, nOffs, Type);
#endif
}

void CRenderer::CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount)
{
#ifndef VK_RENDERER
	m_BufferManager->Create(dest, src, indexcount);
#endif
}

void CRenderer::UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock /* = true*/)
{
#ifndef VK_RENDERER
	m_BufferManager->Update(dest, src, indexcount, bUnLock);
#endif
}

void CRenderer::ReleaseIndexBuffer(SVertexStream* dest)
{
}

void CRenderer::CreateQuad()
{
	SVF_P3F_T2F verts[] = {
		{{0, 1, 0}, {0, 1}},
		{{0, 0, 0}, {0, 0}},
		{{1, 1, 0}, {1, 1}},
		{{1, 0, 0}, {1, 0}}};
	m_VertexBuffer = gEnv->pRenderer->CreateBuffer(4, VERTEX_FORMAT_P3F_T2F, "screen_quad", false);
	UpdateBuffer(m_VertexBuffer, verts, 4, false);
}

void CRenderer::Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info)
{
	info.font->RenderText(szText, posX, posY, 1.0, info.color);
	info.font->Submit();
}

void CRenderer::DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)
{
//TODO: implement draw image via aux render. Need extend it, to draw arbitary colored or textured primitves. In this terms, drawimage - simple drawing of textured quad. Aux Render must collect all draw request and batch it.
#if 0
	float
		width  = GetWidth(),
		height = GetHeight();
	//gl::BindFramebuffer(0);
	RSS(this, BLEND, true);
	RSS(this, CULL_FACE, false);
	RSS(this, DEPTH_TEST, false);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 model(1.0);
	auto uv_projection = glm::mat4(1.0);

	model = glm::translate(model, glm::vec3(xpos, ypos, 0.f));
	model = glm::scale(model, {w, h, 1.f});

	uv_projection = glm::scale(glm::mat4(1.0), glm::vec3(s1 - s0, t1 - t0, 1.0f));
	uv_projection = glm::translate(uv_projection, glm::vec3(s0, t0, 0.f));

	{
		auto sb			 = CRenderer::screenBuffer;
		sb->Color		 = Vec4(r, g, b, a);
		sb->UvProjection = uv_projection;
		sb->Model		 = model;
		sb->Alpha		 = a;
		//sb->CopyToDevice();
	}
	//m_ScreenShader->Use();
	glProgramUniformHandleui64ARB(m_ScreenShader->Get(), glGetUniformLocation(m_ScreenShader->Get(), "screenTexture"), texture_id);

	DrawFullscreenQuad();
	m_ScreenShader->Unuse();
#endif
}

int CRenderer::GetWidth()
{
	return m_Window->getWidth();
}

int CRenderer::GetHeight()
{
	return m_Window->getHeight();
}

IGraphicsDeviceConstantBuffer* CRenderer::CreateConstantBuffer(int size)
{
#ifndef VK_RENDERER
	return m_BufferManager->CreateConstantBuffer(size);
#else
	return nullptr;
#endif
}

void CRenderer::ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz)
{
	auto s = glm::project(Vec3(ptx, pty, ptz), m_Camera.GetViewMatrix(), m_Camera.getProjectionMatrix(), Vec4(0, 0, GetWidth(), GetHeight()));

	*sx = s.x;
	*sy = s.y;
	*sz = s.z;
}

int CRenderer::UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4])
{
	return 0;
}

int CRenderer::UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz)
{
#if 0
	Vec4d vp;							// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, &vp[0]); // Retrieves The Viewport Values (X, Y, Width, Height)
#endif
	auto p = glm::unProject(
		glm::vec3(sx, GetHeight() - sy, sz), m_Camera.GetViewMatrix(), m_Camera.getProjectionMatrix(), glm::vec4(0, 0, GetWidth(), GetHeight()));
	*px = p.x;
	*py = p.y;
	*pz = p.z;

	return 0;
}

void CRenderer::GetModelViewMatrix(float* mat)
{
}

void CRenderer::GetModelViewMatrix(double* mat)
{
}

void CRenderer::GetProjectionMatrix(double* mat)
{
}

void CRenderer::GetProjectionMatrix(float* mat)
{
	mat = &m_Camera.getProjectionMatrix()[0][0];
}

Vec3 CRenderer::GetUnProject(const Vec3& WindowCoords, const CCamera& cam)
{
	auto& c = WindowCoords;
	return glm::unProject(
		glm::vec3(c.x, GetHeight() - c.y, 0), cam.GetViewMatrix(), cam.getProjectionMatrix(), glm::vec4(0, 0, GetWidth(), GetHeight()));
}

int CRenderer::GetFrameID(bool bIncludeRecursiveCalls /* = true*/)
{
	return m_FrameID;
}

void CRenderer::PrintLine(const char* szText, SDrawTextInfo& info)
{
	Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
}

ITexture* CRenderer::LoadTexture(const char* nameTex, uint flags, byte eTT)
{
#if 0
	return TextureManager::instance()->getTexture(nameTex, eTT);
#else
	return new CTextureNull();
//return nullptr;
#endif
}

void CRenderer::SetCamera(const CCamera& cam)
{
	m_Camera = cam;
}

const CCamera& CRenderer::GetCamera()
{
	return m_Camera;
}

IShader* CRenderer::Sh_Load(const char* name, int flags, uint64 nMaskGen)
{
#if 0
	flags = int(ShaderBinaryFormat::SPIRV);
#endif
	gEnv->pLog->Log("load shader: %s", name);
	return gShMan->Sh_Load(name, flags, nMaskGen);
}

void CRenderer::Set2DMode(bool enable, int ortox, int ortoy)
{
}

IFont* CreateIFont()
{
	return new FreeTypeFont();
}

IFont* CRenderer::GetIFont()
{
#ifdef DEDICATED_SERVER
	m_Fonts.push_back(new CNullFont());
#else
	m_Fonts.push_back(CreateIFont());
#endif // DEDICATED_SERVER
	return m_Fonts.back();
}

IRenderAuxGeom* CRenderer::GetIRenderAuxGeom()
{
	return m_RenderAuxGeom;
}

#ifdef LINUX
#	include <experimental/filesystem>
using fs = std::experimental::filesystem;
#else
#	include <filesystem>
namespace fs = std::filesystem;
#endif
#include <BlackBox/Core/Utils.hpp>
struct STestFXAutoComplete : public IConsoleArgumentAutoComplete
{
	wchar_t c_file[256];
#define FX_BASE "tmp"
	[[nodiscard]] int GetCount() const override
	{
#ifndef LINUX
		int cnt = std::count_if(
			fs::directory_iterator::directory_iterator(fs::path::path(FX_BASE)),
			fs::directory_iterator::directory_iterator(),
			static_cast<bool (*)(const fs::path&)>(fs::is_regular_file));
		return cnt;
#else
		return 0;
#endif
	};

	const char* GetValue(int nIndex) const override
	{
#ifndef LINUX
		int				   i = 0;
		static std::string file;
		for (fs::directory_iterator next(fs::path(FX_BASE)), end; next != end; ++next, ++i)
		{
			if (i == nIndex)
			{
				//memset((void*)c_file, 0, 256);
				//memcpy((void*)c_file, , wcslen(next->path().c_str()));
				file = wstr_to_str(std::wstring(next->path().c_str()));
				return file.data();
			}
		}
#endif
		return "!!!";
	};
};

static STestFXAutoComplete s_TestFXAutoComplete;
void					   CRenderer::InitConsoleCommands() const
{
	/*
  REGISTER_COMMAND(
    "set_ws",
    R"(set2dvec("r_cam_w", "r_cam_h", %1, %2))",
    VF_NULL,
    "Set size of camera"
  );
  */
	REGISTER_CVAR(dump_shaders_on_load, false, VF_DUMPTODISK, "");
	REGISTER_COMMAND("testfx", TestFx, 0, "Test fx parser");
	gEnv->pConsole->RegisterAutoComplete("testfx", &s_TestFXAutoComplete);
}

void CRenderer::ShareResources(IRenderer* renderer)
{
}

void CRenderer::SetRenderCallback(IRenderCallback* pCallback)
{
}

void CRenderer::PushProfileMarker(char* label)
{
	RenderDebugger::PushGroup(0, label);
}

void CRenderer::PopProfileMarker(char* label)
{
	RenderDebugger::EndGroup();
}

int CRenderer::CreateRenderTarget()
{
	return 0;
}

void CRenderer::DrawFullscreenQuad()
{
}

ITechniqueManager* CRenderer::GetITechniqueManager()
{
	return nullptr;
}

float CRenderer::GetDepthValue(int x, int y)
{
	return 0.0f;
}

void CRenderer::Flush()
{
	DEBUG_GROUP("AUX");

#ifndef VK_RENDERER
	auto pvb			 = perViewBuffer;
	pvb->Projection		 = m_Camera.getProjectionMatrix();
	pvb->View			 = m_Camera.GetViewMatrix();
	pvb->OrthoProjection = glm::ortho(0.f, float(GetWidth()), float(GetHeight()), 0.f);
	pvb->ViewProjection	 = pvb->Projection * pvb->View;
	pvb->Eye			 = gEnv->pSystem->GetViewCamera().GetPos();

	pvb.CopyToDevice();
#endif
	m_RenderAuxGeom->Flush();
}

void CRenderer::Sh_Reload()
{
}

#pragma warning(pop)
