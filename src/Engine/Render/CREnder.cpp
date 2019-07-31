#include <BlackBox/Render/CRender.hpp>
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/Render/IFont.hpp>

CRender::CRender(IEngine *engine) : m_Engine(engine), m_viewPort(0,0,0,0)
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
  sf::ContextSettings settings(zbpp, sbits, 0, majorVersion, minorVersion, glContextType);
	if (!m_Window->create(reinterpret_cast<void*>(&settings)))
		return false;
	if (!m_Window->init(x, y, width, height, cbpp, zbpp, sbits, fullscreen))
		return false;
  if (!OpenGLLoader())
    return false;
	//=======================
	translateImageY = m_Engine->getIConsole()->CreateVariable("timY", 3.0f, 0);
	r_debug = m_Engine->getIConsole()->GetCVar("r_debug");
	//=======================
  glInit();
	m_ScreenQuad = new Quad();
	//=======================
	m_ScreenShader = new CShaderProgram(
		CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), 
		CShader::load("res/shaders/screenshader.frag", CShader::E_FRAGMENT))
		;
	m_ScreenShader->create();
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

void CRender::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
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
		glDebug = new OpenglDebug("out/glDebug.txt");
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	}
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1, 0);
	glLineWidth(2);
	//glEnable(GL_FRAMEBUFFER_SRGB);
  glCheck(glEnable(GL_DEPTH_TEST));
  glCheck(glEnable(GL_TEXTURE_2D));
  glCheck(glEnable(GL_CULL_FACE));
  glCheck(glCullFace(GL_BACK));
	// Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void CRender::DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)
{
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	m_ScreenShader->use();
	auto projection = glm::mat4(1.0);
	auto model = glm::mat4(1.0);
	auto uv_transform = glm::mat4(1.0);
	uv_transform = glm::scale(uv_transform, glm::vec3(1.f, -1.f, 1.f));
	model = glm::translate(model, glm::vec3(1.f, 1.f, 0.f));
	//model = glm::translate(model, glm::vec3(xpos, ypos, 0));
	model = glm::scale(model, glm::vec3(w, 0.5*h, 1.f));
	model = glm::translate(model, glm::vec3(xpos, translateImageY->GetFVal() * (ypos / GetHeight()), 0.f));
	projection = glm::ortho(0.f, (float)GetWidth(), 0.f, (float)GetHeight());
	m_ScreenShader->setUniformValue(projection, "projection");
	m_ScreenShader->setUniformValue(model, "model");
	m_ScreenShader->setUniformValue(glm::mat3(uv_transform), "uv_transform");
	glCheck(glDisable(GL_DEPTH_TEST));
	glCheck(glActiveTexture(GL_TEXTURE0));
	glCheck(glBindTexture(GL_TEXTURE_2D, texture_id));
	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	m_ScreenQuad->draw();;
	glCheck(glDisable(GL_BLEND));
	m_ScreenShader->unuse();
}

void CRender::PrintLine(const char* szText, SDrawTextInfo& info)
{
	Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
}


IRENDER_API IRender* CreateIRender(IEngine* pSystem)
{
	return new CRender(pSystem);
}
