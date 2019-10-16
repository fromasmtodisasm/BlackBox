#include <BlackBox/Render/Render.hpp>
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/Camera.hpp>
#include <BlackBox/Render/IFont.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>

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
	initConsoleVariables();
	//=======================
	if (isDebug && r_debug->GetIVal() == 1)
		glContextType = sf::ContextSettings::Debug;
	else
		glContextType = sf::ContextSettings::Attribute::Core;
  sf::ContextSettings settings(zbpp, sbits, antialiassing, majorVersion, minorVersion, glContextType);
	if (!m_Window->create(reinterpret_cast<void*>(&settings)))
		return false;
	if (!m_Window->init(x, y, width, height, cbpp, zbpp, sbits, fullscreen))
		return false;
  if (!OpenGLLoader())
    return false;
	//=======================
	m_pSystem->GetIConsole()->AddConsoleVarSink(this);
	m_pSystem->GetIInputHandler()->AddEventListener(this);
	//=======================
  glInit();
	m_ScreenQuad = new Quad();
	//=======================
	ProgramDesc pd = {
		"screen_quad",
		"screenshader.vs",
		"screenshader.frag"
	};
	MaterialManager::instance()->loadProgram(pd, false);
	m_ScreenShader = MaterialManager::instance()->getProgram(pd.name);
	//m_ScreenShader->create();
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(0,"screenTexture");
	m_ScreenShader->unuse();

	cam_width->Set(GetWidth());
	cam_height->Set(GetHeight());
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
	glCheck(glViewport(x, y, width, height));
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

void CRender::initConsoleVariables()
{
	translateImageY =			CREATE_CONSOLE_VAR("ty", 0.0f, 0);
	translateImageX =			CREATE_CONSOLE_VAR("tx", 0.0f, 0);
	scaleImageX =					CREATE_CONSOLE_VAR("sx", 1.0f, 0);
	scaleImageY =					CREATE_CONSOLE_VAR("sy", 1.0f, 0);
	needTranslate =				CREATE_CONSOLE_VAR("nt", 1, 0, "Translate or not 2d background of console");
	needFlipY =						CREATE_CONSOLE_VAR("nfy", 1, 0, "Flip or not 2d background of console");
	test_proj =						CREATE_CONSOLE_VAR("test_proj", "test proj empty", 0);
	render_via_viewport = CREATE_CONSOLE_VAR("rvv", 0, 0, "Rendering use view port, if 1 else with projection matrix");

	r_debug = m_pSystem->GetIConsole()->GetCVar("r_debug");
	cam_width		= m_pSystem->GetIConsole()->GetCVar("r_cam_w");
	cam_height	= m_pSystem->GetIConsole()->GetCVar("r_cam_h");
}

void CRender::SetCullMode(CullMode mode/* = CullMode::BACK*/)
{
	if (mode == CullMode::FRONT_AND_BACK)
		glCullFace(GL_FRONT_AND_BACK);
	else
		glCullFace(GL_FRONT + static_cast<unsigned int>(mode));
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
			glCheck(glEnable(it->second));
		else
			glCheck(glDisable(it->second));
	}
}

void CRender::DrawFullScreenImage(int texture_id)
{
	auto
		width = GetWidth(),
		height = GetHeight();
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	SetViewport(0, 0, width, height);
	m_ScreenShader->use();
	auto proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	auto transform = glm::scale(proj, glm::vec3(width, height, 1));
	m_ScreenShader->setUniformValue(transform, "transform");
	SetState(State::DEPTH_TEST, false);
	m_ScreenShader->bindTextureUnit2D(texture_id, 0);
	m_ScreenQuad->draw();
}

bool CRender::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
#if 0
	m_pSystem->ShowMessage(pVar->GetName(), "Show name", ISystem::M_WARNING);
#endif
	if (!strcmp(pVar->GetName(),"r_Width"))
	{
		m_Window->changeSize(pVar->GetFVal(), 0);
	}
	else if (!strcmp(pVar->GetName(),"r_Height"))
	{
		m_Window->changeSize(0, pVar->GetFVal());
	}
	else if (!strcmp(pVar->GetName(),"r_debug"))
	{
		OpenglDebuger::SetIgnore(!r_debug->GetIVal());
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
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	SetState(State::BLEND, true);
	SetState(IRender::State::CULL_FACE, false);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(a, "alpha");

	glm::mat4 model(1.0);
	auto uv_projection = glm::mat4(1.0);
	glm::mat4 projection = glm::ortho(0.0f, (float)GetWidth(), (float)GetHeight(), 0.0f, -1.0f, 1000.0f);

	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.f));
	//model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.f));
	model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.f));
	model = glm::scale(model, {scaleImageX->GetFVal() * w, scaleImageY->GetFVal() * h, 1.f });
	if (needTranslate->GetIVal())
		model = glm::translate(model, 
			glm::vec3(
			((2 * translateImageX->GetFVal() + xpos) + (float)GetWidth()) / (float)GetWidth(), 
			((2 * translateImageY->GetFVal() + ypos) + (float)GetHeight()) / (float)GetHeight(), 
			0.f)
		); 

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
		m_ScreenShader->setUniformValue(projection, "projection");
		m_ScreenShader->setUniformValue(uv_projection, "uv_projection");
		m_ScreenShader->setUniformValue(model, "model");

	}
	else
	{
		SetViewport(xpos, GetHeight() - h, xpos + w, GetHeight() - ypos - h);
	}

	SetState(State::DEPTH_TEST, false);
	m_ScreenShader->bindTextureUnit2D(texture_id, 0);
	m_ScreenQuad->draw();;
	SetState(State::CULL_FACE, true);
}

void CRender::PrintLine(const char* szText, SDrawTextInfo& info)
{
	Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
}

bool CRender::OnInputEvent(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Resized:
	{
		this->cam_width->Set(static_cast<int>(event.size.width));
		this->cam_height->Set(static_cast<int>(event.size.height));
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
