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
  m_Engine(engine), m_viewPort(0,0,0,0)
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
  sf::ContextSettings settings(zbpp, sbits, antialiassing, majorVersion, minorVersion, glContextType);
	if (!m_Window->create(reinterpret_cast<void*>(&settings)))
		return false;
	if (!m_Window->init(x, y, width, height, cbpp, zbpp, sbits, fullscreen))
		return false;
  if (!OpenGLLoader())
    return false;
	//=======================
	translateImageY = m_Engine->GetIConsole()->CreateVariable("ty", 0.0f, 0);
	translateImageX = m_Engine->GetIConsole()->CreateVariable("tx", 0.0f, 0);

	scaleImageX = m_Engine->GetIConsole()->CreateVariable("sx", 1.0f, 0);
	scaleImageY = m_Engine->GetIConsole()->CreateVariable("sy", 1.0f, 0);

	needTranslate = m_Engine->GetIConsole()->CreateVariable("nt", 1, 0, "Translate or not 2d background of console");
	needFlipY = m_Engine->GetIConsole()->CreateVariable("nfy", 1, 0, "Flip or not 2d background of console");

	test_proj = m_Engine->GetIConsole()->CreateVariable("test_proj", "test proj empty", 0);
	r_debug = m_Engine->GetIConsole()->GetCVar("r_debug");
	render_via_viewport = m_Engine->GetIConsole()->CreateVariable("rvv", 0, 0, "Rendering use view port, if 1 else with projection matrix");
	//=======================
	m_Engine->GetIConsole()->AddConsoleVarSink(this);
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
	if (glContextType == sf::ContextSettings::Debug || r_debug->GetIVal() == 1)
	{
		glDebug = new OpenglDebuger("out/glDebug.txt");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 0);
	glLineWidth(1);
	//glEnable(GL_FRAMEBUFFER_SRGB);
  glCheck(glEnable(GL_DEPTH_TEST));
  //glCheck(glEnable(GL_TEXTURE_2D));
  glCheck(glEnable(GL_CULL_FACE));
  glCheck(glCullFace(GL_BACK));
	// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void CRender::DrawFullScreenImage(int texture_id)
{
  float
    width = GetWidth(),
    height = GetHeight();
  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  SetViewport(0, 0, width, height);
  glCheck(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));
  m_ScreenShader->use();
  auto proj = glm::ortho(0.0f, (float)GetWidth(), 0.0f, (float)GetHeight());
  auto transform = glm::scale(proj, glm::vec3(width, height, 1));
  m_ScreenShader->setUniformValue(transform, "transform");
  glCheck(glDisable(GL_DEPTH_TEST));
  glCheck(glActiveTexture(GL_TEXTURE0));
  glCheck(glBindTexture(GL_TEXTURE_2D, texture_id));
  m_ScreenQuad->draw();
}

bool CRender::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
#if 0
	m_Engine->ShowMessage(pVar->GetName(), "Show name", ISystem::M_WARNING);
#endif
	if (!strcmp(pVar->GetName(),"r_Width"))
	{
		m_Window->changeSize(pVar->GetFVal(), 0);
	}
	else if (!strcmp(pVar->GetName(),"r_Height"))
	{
		m_Window->changeSize(0, pVar->GetFVal());
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
	glEnable(GL_BLEND);
	glCheck(glDisable(GL_CULL_FACE));
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

	glCheck(glDisable(GL_DEPTH_TEST));
	glCheck(glActiveTexture(GL_TEXTURE0));
	glCheck(glBindTexture(GL_TEXTURE_2D, texture_id));
	m_ScreenQuad->draw();;

	glCheck(glEnable(GL_CULL_FACE));

}

void CRender::PrintLine(const char* szText, SDrawTextInfo& info)
{
	Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
}

bool CRender::OnInputEvent(sf::Event& event)
{
	return false;
}


IRENDER_API IRender* CreateIRender(ISystem* pSystem)
{
	return new CRender(pSystem);
}

#pragma warning(push)
