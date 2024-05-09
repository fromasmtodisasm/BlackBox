#include "pch.hpp"
#include "BaseRenderer.hpp"
#include "BufferManager.hpp"
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/System/IProjectManager.hpp>
#include <BlackBox/System/IWindow.hpp>
#include "FreeTypeFont.hpp"

#include "AuxRendererNull.hpp"
#include "AuxRenderer.hpp"

#include <SDL2/SDL.h>
#include <BlackBox/Core/Path.hpp>


#include <filesystem>
#pragma warning(push)
#pragma warning(disable : 4244)

//////////////////////////////////////////////////////////////////////////
// Globals.
//////////////////////////////////////////////////////////////////////////
CRenderer* gRenDev = NULL;

static int dump_shaders_on_load = false;
std::unique_ptr<FxParser>  g_FxParser;
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

	FxEffect* pEffect = nullptr;
#if 1
	if (g_FxParser->Parse(filename, &pEffect))
	{
		CryLog("Dumping shaders of effect: %s", filename.data());
		for (int i = 0; i < pEffect->GetNumTechniques(); i++)
		{
			auto tech = pEffect->GetTechnique(i);
			CryLog("Technique: %s", tech->GetName());
			for (int j = 0; j < tech->GetNumPasses(); j++)
			{
				auto pass = tech->GetPass(j);
				CryLog("[%s]", pass->Name.c_str());
				
			}
		}
	}
#endif
}

CRenderer::CRenderer()
    : m_viewPort(0, 0, 0, 0)
{
}

void CRenderer::Release()
{
//FIXME:
#if 0
	delete this;
#else
	this->~CRenderer();
#endif
}

void CRenderer::RegisterCallbackClient(IRendererCallbackClient* pClient)
{
	m_RenderCallbackClients.push_back(pClient);
}

void CRenderer::UnregisterCallbackClient(IRendererCallbackClient* pClient)
{
	if (auto cl = std::find(m_RenderCallbackClients.begin(), m_RenderCallbackClients.end(), pClient); cl != m_RenderCallbackClients.end())
	{
		m_RenderCallbackClients.erase(cl);
	}
}

bool CRenderer::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
	auto name = std::string_view(pVar->GetName());
	auto dispatcher = Env::System()->GetISystemEventDispatcher();
	if (name == "r_Width")
	{
		dispatcher->OnSystemEvent(ESYSTEM_EVENT_RESIZE, std::strtof(sNewValue, nullptr), GetHeight());
	}
	if (name == "r_Height")
	{
		dispatcher->OnSystemEvent(ESYSTEM_EVENT_RESIZE, GetWidth(), std::strtof(sNewValue, nullptr));
	}
	if (name == "r_Fullscreen")
	{
		dispatcher->OnSystemEvent(ESYSTEM_EVENT_TOGGLE_FULLSCREEN, std::strtol(sNewValue, nullptr, 10), 0);
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
	ShutDown();
}

IWindow* CRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
	if (!gRenDev)
		gRenDev = this;
	m_pSystem = Env::System();
	InitCVars();
	IWindow* result = m_Window = window;
	bInFullScreen              = fullscreen;
	if (window == nullptr)
		return nullptr;
	//=======================
	InitConsoleCommands();
	//=======================
	const char* sGameName = Env::System()->GetIProjectManager()->GetCurrentProjectName();
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
	CryLog("Init override before");
	if (!InitOverride())
		return nullptr;
	CryLog("Init override after");
#if 0
	g_FxParser = &s_FxParser;
#else
	g_FxParser      = std::make_unique<FxParser>();
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
#if 0
	CreateQuad();
#endif
	gShMan = new ShaderMan;
	//=======================
	//pd.vs.macro["STORE_TEXCOORDS"] = "1";
#if 0
	if (!(m_ScreenShader = _smart_ptr((CShader*)(Sh_Load("screen", 0)))))
	{
		m_pSystem->Log("Error of loading screen shader");
		return nullptr;
	}
#endif

#if 1
	_CrtCheckMemory();
	m_RenderAuxGeom = new CRenderAuxGeom();
#else
	m_RenderAuxGeom = new CRenderAuxGeomNull();
#endif
	Env::Get()->pAuxGeomRenderer = m_RenderAuxGeom;

#if 0
	CreateRenderTarget();
#endif

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
	///////////////////////////////////////////////////////
	using color = Legacy::Vec3;
	using pos = Legacy::Vec3;
	//m_LigthsList = {
	//    {color{}, pos{}},
	//    {color{}, pos{}},
	//    {color{}, pos{}},
	//    {color{}, pos{}},
	//};

	return result;
}

int CRenderer::EnumDisplayFormats(SDispFormat* formats)
{
	static int      displayInUse = 0; /* Only using first display */
	int             numModes     = 0;
	int             i            = 0;
	SDL_DisplayMode mode;
	Uint32          f;

	Env::Log()->Log("SDL_GetNumVideoDisplays(): %i", SDL_GetNumVideoDisplays());

	numModes = SDL_GetNumDisplayModes(displayInUse);
	if (numModes < 1)
	{
		Env::Log()->Log("SDL_GetNumDisplayModes failed: %s", SDL_GetError());
		return 1;
	}
	Env::Log()->Log("SDL_GetNumDisplayModes: %i", numModes);

	if (formats != nullptr)
	{
		for (i = 0; i < numModes; ++i)
		{
			if (SDL_GetDisplayMode(displayInUse, i, &mode) != 0)
			{
				Env::Log()->Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
				return 1;
			}
			f                   = mode.format;

			formats[i].m_BPP    = SDL_BITSPERPIXEL(f);
			formats[i].m_Width  = mode.w;
			formats[i].m_Height = mode.h;
			if (formats[i].m_BPP == 24)
				formats[i].m_BPP = 32;
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

	REGISTER_CVAR2("r_sun.r", &r_SunColor.r, 1.f, VF_DUMPTODISK, "");
	REGISTER_CVAR2("r_sun.g", &r_SunColor.g, 1.f, VF_DUMPTODISK, "");
	REGISTER_CVAR2("r_sun.b", &r_SunColor.b, 1.f, VF_DUMPTODISK, "");

	Env::Console()->Register("r_TexResolution", &CV_r_texresolution, 0, VF_DUMPTODISK,
		"Halves or doubles texture resolution.\n"
		"Usage: r_TexResolution [0/1]\n"
		"When 0 (default) texture resolution is halved.");
	Env::Console()->Register("r_Gamma", &CV_r_gamma, 1.0f, VF_DUMPTODISK,
		"Sets gamma level.\n"
		"Usage: r_Gamma 1.0\n"
		"Default is 1.0.");
	Env::Console()->Register("r_Brightness", &CV_r_brightness, 0.5f, VF_DUMPTODISK,
		"Sets the diplay brightness.\n"
		"Usage: r_Brightness 0.5\n"
		"Default is 0.5.");
	Env::Console()->Register("r_Contrast", &CV_r_contrast, 0.5f, VF_DUMPTODISK,
		"Sets the diplay contrast.\n"
		"Usage: r_Contrast 0.5\n"
		"Default is 0.5.");

	Env::Console()->Register("r_RenderMode", &CV_r_rendermode, 0, VF_DUMPTODISK,
		"Defines type of post-processing rendering mode.\n"
		"Usage: r_RenderMode #\n"
		"Where # represents:\n"
		" 0: Default rendering (default)\n"
		" 1: Improved\n"
		" 2: Paradisiacal\n"
		" 3: Cold realism\n"
		" 4: Cartoon\n"
		" 5: Cinematic\n");

	Env::Console()->Register("r_Glare", &CV_r_glare, 0, VF_DUMPTODISK,
		"Activates the glare effect.\n"
		"Usage: r_Glare #\n"
		"Where # represents:\n"
		" 0: Off\n"
		" 1: On\n");

	auto iConsole = Env::Console();

	iConsole->Register("r_HDRRendering", &CV_r_hdrrendering, 0, VF_DUMPTODISK,
		"Toggles HDR rendering.\n"
		"Usage: r_HDRRendering [0/1]\n"
		"Default is 1 (on). Set to 0 to disable HDR rendering.");
	iConsole->Register("r_HDRLevel", &CV_r_hdrlevel, 0.6f, VF_DUMPTODISK,
		"HDR rendering level.\n"
		"Usage: r_HDRLevel [Value]\n"
		"Default is 0.6f");
	iConsole->Register("r_TexSkyResolution", &CV_r_texskyresolution, 0, VF_DUMPTODISK);
	// changing default settings to reduce the insane amount of texture memory
	iConsole->Register("r_TexBumpResolution", &CV_r_texbumpresolution, 0, VF_DUMPTODISK);





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
	m_VertexBuffer = Env::Renderer()->CreateBuffer(4, VERTEX_FORMAT_P3F_T2F, "screen_quad", false);
	UpdateBuffer(m_VertexBuffer, verts, 4, false);
}

void CRenderer::Draw2dText(float posX, float posY, const char* szText, const SDrawTextInfo& info)
{
	if (info.font)
	{
		info.font->RenderText(szText, posX, posY, 1.f, (float*)info.color);
		info.font->Submit();
	}
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
		sb->Color		 = Legacy::Vec4(r, g, b, a);
		sb->UvProjection = uv_projection;
		sb->Model		 = model;
		sb->Alpha		 = a;
		//sb->CopyToDevice();
	}
	//m_ScreenShader->Use();
	glProgramUniformHandleui64ARB(m_ScreenShader->Get(), glGetUniformLocation(m_ScreenShader->Get(), "screenTexture"), texture_id);

	DrawFullscreenQuad();
	m_ScreenShader->Unuse();
#else
	Draw2dImage(xpos, ypos, w, h, texture_id, s0, t0, s1, t1, 0, r, g, b, a);
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
	auto s = glm::project(Legacy::Vec3(ptx, pty, ptz), m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix(), Legacy::Vec4(0, 0, GetWidth(), GetHeight()));

	*sx    = s.x;
	*sy    = s.y;
	*sz    = s.z;
}

int CRenderer::UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4])
{
	return 0;
}

int CRenderer::UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz)
{
#if 0
	Legacy::Vec4d vp;							// Where The Viewport Values Will Be Stored
	glGetIntegerv(GL_VIEWPORT, &vp[0]); // Retrieves The Viewport Values (X, Y, Width, Height)
#endif
	auto p = glm::unProject(
	    glm::vec3(sx, GetHeight() - sy, sz), m_Camera.GetViewMatrix(), m_Camera.GetProjectionMatrix(), glm::vec4(0, 0, GetWidth(), GetHeight()));
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
	mat = &m_Camera.GetProjectionMatrix()[0][0];
}

Legacy::Vec3 CRenderer::GetUnProject(const Legacy::Vec3& WindowCoords, const CCamera& cam)
{
	auto& c = WindowCoords;
	return glm::unProject(
	    glm::vec3(c.x, GetHeight() - c.y, 0), cam.GetViewMatrix(), cam.GetProjectionMatrix(), glm::vec4(0, 0, GetWidth(), GetHeight()));
}

int CRenderer::GetFrameID(bool bIncludeRecursiveCalls /* = true*/)
{
	return m_FrameID;
}

void CRenderer::PrintLine(const char* szText, SDrawTextInfo& info)
{
	Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
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
	CryLog("load shader: %s", name);
	return gShMan->Sh_Load(name, flags, nMaskGen);
}

void CRenderer::Set2DMode(bool enable, int ortox, int ortoy)
{
	//TODO: acount ortox and ortoy
	ortho      = Vec2(ortox, ortoy);
	m_Is2DMode = enable;
}

IFont* CreateIFont()
{
//#ifdef DEDICATED_SERVER
#if 0
	return new CNullFont();
#else
	return new FreeTypeFont();
#endif // DEDICATED_SERVER
}

IFont* CRenderer::GetIFont()
{
	_CrtCheckMemory();
	m_Fonts.push_back(CreateIFont());
	_CrtCheckMemory();
	FreeTypeFont* font = static_cast<FreeTypeFont*>(m_Fonts.back());
	auto          map  = font->Characters;
	map.insert(std::pair<char, FreeTypeFont::Character>(41, {}));
	return m_Fonts.back();
}

IRenderAuxGeom* CRenderer::GetIRenderAuxGeom()
{
	return m_RenderAuxGeom;
}

#ifdef LINUX
	#include <experimental/filesystem>
using fs = std::experimental::filesystem;
#else
	#include <filesystem>
namespace fs = std::filesystem;
#endif
#include <BlackBox/Core/Utils.hpp>
struct STestFXAutoComplete : public IConsoleArgumentAutoComplete
{
	wchar_t c_file[256];
#define FX_BASE "./Engine"
	[[nodiscard]] int GetCount() const override
	{
	#ifndef LINUX

		int cnt = std::count_if(
			fs::recursive_directory_iterator::recursive_directory_iterator(fs::path::path(FX_BASE)),
			fs::recursive_directory_iterator::recursive_directory_iterator(),
			[](const fs::path& p) {
				if (fs::is_regular_file(p))
				{
					if (p.extension() == ".fx")
						return true;
				}
				return false;
			});
			//static_cast<bool (*)(const fs::path&)>(fs::is_regular_file));
		return cnt;
	#else
		return 0;
	#endif
	};

	const char* GetValue(int nIndex) const override
	{
#ifndef LINUX
		int                i = 0;
		static std::string file;
		for (fs::recursive_directory_iterator next(fs::path(FX_BASE)), end; next != end; ++next)
		{
			if (fs::is_regular_file(next->path()) && next->path().extension() == ".fx")
			{
				if (i == nIndex)
				{
					//memset((void*)c_file, 0, 256);
					//memcpy((void*)c_file, , wcslen(next->path().c_str()));
					file = wstr_to_str(std::wstring(next->path().c_str()));
					return file.data();
				}
				++i;
			}
		}
#endif
		return "!!!";
	};
};

static STestFXAutoComplete s_TestFXAutoComplete;
void                       CRenderer::InitConsoleCommands() const
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
	Env::Console()->RegisterAutoComplete("testfx", &s_TestFXAutoComplete);
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
	D3DPERF_BeginEvent(D3DC_Blue, L"Flush");

#ifndef VK_RENDERER
	auto pvb             = perViewBuffer;
	pvb->Projection      = m_Camera.GetProjectionMatrix();
	pvb->View            = m_Camera.GetViewMatrix();
	pvb->OrthoProjection = glm::ortho(0.f, float(GetWidth()), float(GetHeight()), 0.f);
	pvb->ViewProjection  = pvb->Projection * pvb->View;
	pvb->Eye             = Env::System()->GetViewCamera().GetPos();

	pvb.CopyToDevice();
#endif

	{
		for (auto font : m_Fonts)
		{
			font->Submit();
		}
	}
	D3DPERF_EndEvent();
}

void CRenderer::Sh_Reload()
{
}

void CRenderer::ShutDown()
{
#if 0
	PEffect effect{nullptr};
	g_FxParser->Parse("123", &effect);
	Env::Console()->RemoveConsoleVarSink(this);
#endif
	for (size_t i = 0; i < m_Fonts.size(); i++)
	{
		m_Fonts[i]->Release();
	}
//FIXME:
#if 0
	ReleaseBuffer(m_VertexBuffer);
#endif
	SAFE_DELETE(m_RenderAuxGeom);
#ifndef VK_RENDERER
	SAFE_DELETE(m_BufferManager);
#endif
	g_FxParser.release();

	SAFE_DELETE(gShMan);

	m_RenderThread->Quit();
}

#pragma warning(pop)

void ShaderMan::RT_ShaderLoad(const char* name, int flags, uint64 nMaskGen, CShader* p)
{
	if (auto it = m_Shaders.find((name)); it != m_Shaders.end())
	{
		CryLog("Shader <%s> already cached", name);
		it->second->AddRef();
		*p = *it->second;
		return;
	}
	if (!Sh_LoadBinary(name, flags, nMaskGen, p))
	{
		if (Compile(name, flags, nMaskGen, p))
		{
			p->SaveBinaryShader(name, flags, nMaskGen);
		}
		else
		{
			p = nullptr;
		}
	}
}

CShader* ShaderMan::NewShader()
{
	return new CShader;
}

IShader* ShaderMan::Sh_Load(const char* name, int flags, uint64 nMaskGen)
{
	_smart_ptr<CShader> pShader = NewShader();
	gRenDev->ExecuteRenderThreadCommand([=]
		{
			CryLog("load shader: %s", name);
			RT_ShaderLoad(name, flags, nMaskGen, pShader); });
	if (pShader->GetFlags2() & EF2_FAILED)
	{
		pShader = nullptr;
	}
	return pShader;
}

bool ShaderMan::Sh_LoadBinary(const char* name, int flags, uint64 nMaskGen, CShader* p) const
{
	//return Env::Console()->GetCVar("r_SkipShaderCache")->GetIVal() ? nullptr : CShader::LoadBinaryShader(name, flags, nMaskGen);
	return false;
}

bool ShaderMan::Compile(std::string_view name, int flags, uint64 nMaskGen, CShader* p)
{
	FxEffect*         pEffect = nullptr;
	std::stringstream path;
	auto              pos = name.find_last_of('.');
	std::string_view  real_name = name;
	std::string_view  technique;
	int               pass = 0;
	if (pos != name.npos)
	{
		real_name = name.substr(0, pos);
		technique = name.substr(pos + 1);
	}
	path << real_name << ".fx";
	auto shader_path = PathUtil::Make(PathUtil::Make(PathUtil::GetEnginePath(), string("Engine/shaders/fx")), path.str());
	if (g_FxParser->Parse(shader_path.c_str(), &pEffect))
	{
		auto nTech = 0;
		if (auto tech = pEffect->GetTechnique(technique.data(), technique.length()); tech != nullptr)
			nTech = tech->GetId();
		p->m_NameShader = real_name.data();
		p->m_NameFile = path.str();
		if (CShader::LoadFromEffect(p, pEffect, nTech, pass))
		{
			p->AddRef();
			p->ReflectShader();
			p->CreateInputLayout();
			p->m_Flags2 |= EF2_LOADED;
			delete pEffect;
			m_Shaders[string(name)] = p;
			return true;
		}
		p->m_Flags2 |= EF2_FAILED;
		return false;
	}
	else
	{
		p->m_Flags2 |= EF2_FAILED;
	}
	return false;
}

void ShaderMan::ReloadAll()
{
	for (auto& s : m_Shaders)
	{
		s.second->Reload(0);
	}
}

ShaderMan::~ShaderMan()
{
}
