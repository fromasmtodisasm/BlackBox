#include <BlackBox/Render/Render.hpp>
#include <BlackBox/Render/OpenGL/Core.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/Camera.hpp>
#include <BlackBox/Render/IFont.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/IWindow.hpp>


//
#include <SFML/Window.hpp>


#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#pragma warning(push)
#pragma warning(disable : 4244)

CRender::CRender(ISystem *engine) : 
  m_pSystem(engine), m_viewPort(0,0,0,0)
{

}

CRender::~CRender()
{
}

IWindow* CRender::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow *window)
{
	IWindow* result = m_Window = window;
	if (window == nullptr)
		return nullptr;
	//=======================
	InitConsoleVariables();
	InitConsoleCommands();
	//=======================
	if (isDebug && r_debug->GetIVal() == 1)
		glContextType = sf::ContextSettings::Debug;
	else
		glContextType = sf::ContextSettings::Attribute::Core;
	//TODO: FIX THIS
#if 0
  sf::ContextSettings settings(zbpp, sbits, antialiassing, majorVersion, minorVersion, glContextType);
	if (!m_Window->create(reinterpret_cast<void*>(&settings)))
		return nullptr;
#endif
	if (!m_Window->init(x, y, width, height, cbpp, zbpp, sbits, fullscreen))
		return nullptr;
  if (!OpenGLLoader())
    return nullptr;
	//=======================
	m_pSystem->GetIConsole()->AddConsoleVarSink(this);
	m_pSystem->GetIInput()->AddEventListener(this);
	//=======================
  glInit();
	m_ScreenQuad = new Quad();
	//=======================
	ProgramDesc pd = {
		"screen_quad",
		"screenshader.vs",
		"screenshader.frag"
	};
	pd.vs.macro["STORE_TEXCOORDS"] = "1";
	MaterialManager::instance()->loadProgram(pd, false);
	m_ScreenShader = MaterialManager::instance()->getProgram(pd.name);
	if (m_ScreenShader == nullptr)
	{
		m_pSystem->Log("Error of creating screen shader");
		return nullptr;
	}
	//m_ScreenShader->Create();
	m_ScreenShader->Use();
	m_ScreenShader->Uniform(0,"screenTexture");
	m_ScreenShader->Unuse();

	//cam_width->Set(GetWidth());
	//cam_height->Set(GetHeight());
	return result;
}

bool CRender::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
	return false;
}

void CRender::Release()
{
}

void CRender::BeginFrame(void)
{
}

void CRender::Update(void)
{
}

void CRender::GetViewport(int* x, int* y, int* width, int* height)
{
}

void CRender::SetViewport(int x, int y, int width, int height)
{
	gl::ViewPort(glm::vec4(x, y, width, height));
}

void CRender::SetScissor(int x, int y, int width, int height)
{
}

void CRender::SetCamera(const CCamera& cam)
{
}

const CCamera& CRender::GetCamera()
{
	// TODO: insert return statement here
	return *m_Camera;
}

bool CRender::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
	return false;
}

void CRender::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void CRender::Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info)
{
	info.font->RenderText(szText,	posX, posY, 1.0, info.color);
}

int CRender::SetPolygonMode(int mode)
{
	return 0;
}

int CRender::GetWidth()
{
	return m_Window->getWidth();
}

int CRender::GetHeight()
{
	return m_Window->getHeight();
}

void CRender::ScreenShot(const char* filename)
{
}

void CRender::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

void CRender::glInit()
{
	CBaseShaderProgram::use_cache = GetISystem()->GetIConsole()->GetCVar("sh_use_cache");
	fillSates();
	if (glContextType == sf::ContextSettings::Debug && r_debug->GetIVal() == 1)
	{
		glDebug = new OpenglDebuger("out/glDebug.txt");
		SetState(State::DEBUG_OUTPUT, true);
		SetState(State::DEBUG_OUTPUT_SYNCHRONOUS, true);
	}
	SetState(State::POLYGON_OFFSET_FILL, true);
	glPolygonOffset(1, 0);
	glLineWidth(1);
	SetState(State::DEPTH_TEST, true);
  //glCheck(glEnable(GL_TEXTURE_2D));
	SetState(State::BLEND, true);
	SetState(State::CULL_FACE, true);
	SetCullMode(CullMode::BACK);
}

void CRender::fillSates()
{
#define STATEMAP(k,v) stateMap.insert(std::make_pair(k,v))
		STATEMAP(State::DEPTH_TEST,									GL_DEPTH_TEST);
		STATEMAP(State::CULL_FACE,									GL_CULL_FACE);
		STATEMAP(State::BLEND,											GL_BLEND);
		STATEMAP(State::DEBUG_OUTPUT,								GL_DEBUG_OUTPUT);
		STATEMAP(State::DEBUG_OUTPUT_SYNCHRONOUS,		GL_DEBUG_OUTPUT_SYNCHRONOUS);
		STATEMAP(State::POLYGON_OFFSET_FILL,				GL_POLYGON_OFFSET_FILL);
		STATEMAP(State::FRAMEBUFFER_SRGB,						GL_FRAMEBUFFER_SRGB);
		STATEMAP(State::SCISSOR_TEST,								GL_SCISSOR_TEST);
		STATEMAP(State::STENCIL_TEST,								GL_STENCIL_TEST);

#undef STATEMAP
}

void CRender::InitConsoleVariables()
{
	translateImageY =			CREATE_CVAR("ty", 0.0f, 0);
	translateImageX =			CREATE_CVAR("tx", 0.0f, 0);
	scaleImageX =					CREATE_CVAR("sx", 1.0f, 0);
	scaleImageY =					CREATE_CVAR("sy", 1.0f, 0);
	needTranslate =				CREATE_CVAR("nt", 1, 0, "Translate or not 2d background of console");
	needFlipY =						CREATE_CVAR("nfy", 1, 0, "Flip or not 2d background of console");
	test_proj =						CREATE_CVAR("test_proj", "test proj empty", 0);
	render_via_viewport = CREATE_CVAR("rvv", 0, 0, "Rendering use view port, if 1 else with projection matrix");

	r_debug =			GET_CVAR("r_debug");
	cam_width	=		GET_CVAR("r_cam_w");
	cam_height =	GET_CVAR("r_cam_h");
}

void CRender::InitConsoleCommands()
{
	/*
	REGISTER_COMMAND(
		"set_ws",
		R"(set2dvec("r_cam_w", "r_cam_h", %1, %2))",
		VF_NULL,
		"Set size of camera"
	);
	*/
}

void CRender::SetRenderTarget(int nHandle)
{
	if (m_CurrentTarget != nHandle)
		gl::BindFramebuffer(nHandle);
}

void CRender::SetCullMode(CullMode mode/* = CullMode::BACK*/)
{
	if (mode == CullMode::FRONT_AND_BACK)
		gl::CullFace(GL_FRONT_AND_BACK);
	else
		gl::CullFace(GL_FRONT + static_cast<unsigned int>(mode));
}


bool CRender::DeleteContext(WIN_HWND hWnd)
{
	return false;
}

bool CRender::CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX/* = 1*/, int SSY/* = 1*/)
{
	return false;
}

bool CRender::SetCurrentContext(WIN_HWND hWnd)
{
	return false;
}

void CRender::MakeMainContextActive()
{
}

WIN_HWND CRender::GetCurrentContextHWND()
{
	return m_HWND;
}

bool CRender::IsCurrentContextMainVP()
{
	return false;
}

int CRender::GetCurrentContextViewportHeight() const
{
	return 0;
}

int CRender::GetCurrentContextViewportWidth() const
{
	return 0;
}

void CRender::SetClearColor(const Vec3& vColor)
{
	m_clearColor = vColor;
}

void CRender::ClearDepthBuffer()
{
  //glCheck(glClearBufferfv(GL_DEPTH, 0, &m_clearDepth));

	glCheck(glClearDepthf(1.f));
	gl::Clear(GL_DEPTH_BUFFER_BIT);
}

void CRender::ClearColorBuffer(const Vec3 vColor)
{
	glCheck(glClearBufferfv(GL_COLOR, 0, &glm::vec4(vColor,1)[0]));
}

int CRender::EnumDisplayFormats(SDispFormat* formats)
{
	int numModes = 0;
	auto videoModes = sf::VideoMode::getFullscreenModes();

	numModes = static_cast<int>(videoModes.size());
	int i = 0;
	if (formats != nullptr)
	{
		for (auto& mode : videoModes)
		{
			formats[i].m_BPP = mode.bitsPerPixel;
			formats[i].m_Width = mode.width;
			formats[i].m_Height = mode.height;
			i++;
		}
	}

	return numModes;
}

void CRender::SetState(State state, bool enable)
{
	auto it = stateMap.find(state);
	if (it != stateMap.end())
	{
		if (enable)
			gl::Enable(it->second);
		else
			gl::Disable(it->second);
	}
}

void CRender::DrawFullScreenImage(int texture_id)
{
	auto
		width = GetWidth(),
		height = GetHeight();
	gl::BindFramebuffer(0);
	SetViewport(0, 0, width, height);
	m_ScreenShader->Use();
	auto proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	auto transform = glm::scale(proj, glm::vec3(width, height, 1));
	m_ScreenShader->Uniform(transform, "transform");
	SetState(State::DEPTH_TEST, false);
	m_ScreenShader->BindTextureUnit2D(texture_id, 0);
	m_ScreenQuad->draw();
}

bool CRender::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
#if 0
	if (!strcmp(pVar->GetName(),"r_Width"))
	{
		m_Window->changeSize(pVar->GetFVal(), GetHeight());
	}
	else if (!strcmp(pVar->GetName(),"r_Height"))
	{
		m_Window->changeSize(GetWidth(), pVar->GetFVal());
	}
#endif
	if (!strcmp(pVar->GetName(), "r_cam_w"))
	{
		printf("");
	}
	else if (!strcmp(pVar->GetName(),"r_debug"))
	{
		//OpenglDebuger::SetIgnore(!r_debug->GetIVal());
		OpenglDebuger::SetIgnore(!(bool)std::stoi(sNewValue));
	}
	return false;

}

void CRender::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

void CRender::DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)
{
	bool flipY = true;
	float
		width = GetWidth(),
		height = GetHeight();
	//gl::BindFramebuffer(0);
	SetState(State::BLEND, true);
	SetState(IRender::State::CULL_FACE, false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_ScreenShader->Use();
	m_ScreenShader->Uniform(a, "alpha");

	glm::mat4 model(1.0);
	auto uv_projection = glm::mat4(1.0);
	glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1000.0f);

	model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.f));
	model = glm::scale(model, {w, h, 1.f });

  if (needFlipY->GetIVal() == 1)
  {
		uv_projection = glm::scale(glm::mat4(1.0), glm::vec3(1.0f, -1.0f, 1.0f));
  }
  else
  {
		uv_projection = glm::scale(glm::mat4(1.0), glm::vec3(1.f, 1.f, 1.0f));
  }
	uv_projection = glm::translate(uv_projection, glm::vec3(s0, 0, 0.f));
	if (render_via_viewport->GetIVal() == 0)
	{
		m_ScreenShader->Uniform(projection, "projection");
		m_ScreenShader->Uniform(uv_projection, "uv_projection");
		m_ScreenShader->Uniform(model, "model");

	}
	else
	{
		SetViewport(xpos, GetHeight() - h, xpos + w, GetHeight() - ypos - h);
	}

	SetState(State::DEPTH_TEST, false);
	m_ScreenShader->BindTextureUnit2D(texture_id, 0);
	m_ScreenQuad->draw();;
	SetState(State::CULL_FACE, true);
}

void CRender::PrintLine(const char* szText, SDrawTextInfo& info)
{
	Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
}

bool CRender::OnInputEvent(const SInputEvent& event)
{
	switch (event.deviceType)
	{
	case sf::Event::Resized:
	{
		//this->cam_width->Set(static_cast<int>(event.size.width));
		//this->cam_height->Set(static_cast<int>(event.size.height));
		break;
	}
	default:
		break;
	}
	return false;
}


IRENDER_API IRender* CreateIRender(ISystem* pSystem)
{
	return new CRender(pSystem);
}

#pragma warning(push)
