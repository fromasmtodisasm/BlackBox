#include <BlackBox/Render/Render.hpp>
#include <BlackBox/Render/BaseShader.hpp>
#include <BlackBox/Render/OpenGL/Core.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Camera.hpp>
#include <BlackBox/Render/IFont.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/System/IWindow.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <SDL2/SDL.h>

#pragma warning(push)
#pragma warning(disable : 4244)

GLRenderer::GLRenderer(ISystem* engine) :
  m_pSystem(engine), m_viewPort(0, 0, 0, 0)
{
}

GLRenderer::~GLRenderer()
{
}

IWindow* GLRenderer::Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window)
{
  IWindow* result = m_Window = window;
  if (window == nullptr)
    return nullptr;
  //=======================
  InitConsoleVariables();
  InitConsoleCommands();
  //=======================
  if (isDebug && r_debug->GetIVal() == 1)
    glContextType = AttributeType::DEBUG;
  else
    glContextType = AttributeType::CORE;
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
  context = SDL_GL_GetCurrentContext();
  m_HWND = (HWND)window->getHandle();
  // now you can make GL calls.
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  m_Window->swap();
  //=======================
  m_pSystem->GetIConsole()->AddConsoleVarSink(this);
  m_pSystem->GetIInput()->AddEventListener(this);
  //=======================
  glInit();
  m_ScreenQuad = new Quad();
  //=======================
  ProgramDesc pd = {
    "screen_quad",
    ShaderDesc("screenshader.vs"),
    ShaderDesc("screenshader.frag")
  };
  pd.vs.macro["STORE_TEXCOORDS"] = "1";
  MaterialManager::instance()->loadProgram(pd, false);
  m_ScreenShader = MaterialManager::instance()->getProgram(pd.name);
  if (!m_ScreenShader)
  {
    m_pSystem->Log("Error of creating screen shader");
    return nullptr;
  }
  //m_ScreenShader->Create();
  m_ScreenShader->Use();
  m_ScreenShader->Uniform(0, "screenTexture");
  m_ScreenShader->Unuse();

  //cam_width->Set(GetWidth());
  //cam_height->Set(GetHeight());
  return result;
}

bool GLRenderer::ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)
{
  return false;
}

void GLRenderer::Release()
{
  delete this;
}

void GLRenderer::BeginFrame(void)
{
  gl::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::Update(void)
{
}

void GLRenderer::GetViewport(int* x, int* y, int* width, int* height)
{
}

void GLRenderer::SetViewport(int x, int y, int width, int height)
{
  gl::ViewPort(glm::vec4(x, y, width, height));
}

void GLRenderer::SetScissor(int x, int y, int width, int height)
{
}

void GLRenderer::SetCamera(const CCamera& cam)
{
}

const CCamera& GLRenderer::GetCamera()
{
  return *m_Camera;
}

bool GLRenderer::ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)
{
  return false;
}

void GLRenderer::ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
}

void GLRenderer::Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info)
{
  info.font->RenderText(szText, posX, posY, 1.0, info.color);
}

int GLRenderer::SetPolygonMode(int mode)
{
  return 0;
}

int GLRenderer::GetWidth()
{
  return m_Window->getWidth();
}

int GLRenderer::GetHeight()
{
  return m_Window->getHeight();
}

void GLRenderer::ScreenShot(const char* filename)
{
  GLint drawFboId = 0;// , readFboId = 0;
  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);

  gl::BindFramebuffer(0);
  GLint dims[4] = { 0 };
  glGetIntegerv(GL_VIEWPORT, dims);
  GLint width = dims[2];
  GLint height = dims[3];

  uint8_t* pixels = new uint8_t[3 * width * height];
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

  stbi_flip_vertically_on_write(1);
  stbi_write_png(filename, width, height, 3, pixels, width * 3);

  //stbi_image_free(img);
  gl::BindFramebuffer(drawFboId);
}

void GLRenderer::RenderToViewport(const CCamera& cam, float x, float y, float width, float height)
{
}

void GLRenderer::glInit()
{
  CBaseShaderProgram::use_cache = GetISystem()->GetIConsole()->GetCVar("sh_use_cache");
  fillSates();
  if (glContextType == AttributeType::DEBUG && r_debug->GetIVal() == 1)
  {
    glDebug = new OpenglDebuger("out/glDebug.txt");
    SetState(State::DEBUG_OUTPUT, true);
    SetState(State::DEBUG_OUTPUT_SYNCHRONOUS, true);
  }
  SetState(State::POLYGON_OFFSET_FILL, true);
  SetState(State::PROGRAM_POINT_SIZE , true);
  glPolygonOffset(1, 0);
  glLineWidth(1);
  SetState(State::DEPTH_TEST, true);
  //glCheck(glEnable(GL_TEXTURE_2D));
  SetState(State::BLEND, true);
  SetState(State::CULL_FACE, true);
  SetCullMode(CullMode::BACK);
}

void GLRenderer::fillSates()
{
#define STATEMAP(k,v) stateMap.insert(std::make_pair(k,v))
  STATEMAP(State::DEPTH_TEST, GL_DEPTH_TEST);
  STATEMAP(State::CULL_FACE, GL_CULL_FACE);
  STATEMAP(State::BLEND, GL_BLEND);
  STATEMAP(State::DEBUG_OUTPUT, GL_DEBUG_OUTPUT);
  STATEMAP(State::DEBUG_OUTPUT_SYNCHRONOUS, GL_DEBUG_OUTPUT_SYNCHRONOUS);
  STATEMAP(State::POLYGON_OFFSET_FILL, GL_POLYGON_OFFSET_FILL);
  STATEMAP(State::FRAMEBUFFER_SRGB, GL_FRAMEBUFFER_SRGB);
  STATEMAP(State::SCISSOR_TEST, GL_SCISSOR_TEST);
  STATEMAP(State::STENCIL_TEST, GL_STENCIL_TEST);
  STATEMAP(State::PROGRAM_POINT_SIZE, GL_PROGRAM_POINT_SIZE);

#undef STATEMAP
}

void GLRenderer::InitConsoleVariables()
{
  translateImageY = CREATE_CVAR("ty", 0.0f, 0);
  translateImageX = CREATE_CVAR("tx", 0.0f, 0);
  scaleImageX = CREATE_CVAR("sx", 1.0f, 0);
  scaleImageY = CREATE_CVAR("sy", 1.0f, 0);
  needTranslate = CREATE_CVAR("nt", 1, 0, "Translate or not 2d background of console");
  needFlipY = CREATE_CVAR("nfy", 1, 0, "Flip or not 2d background of console");
  test_proj = CREATE_CVAR("test_proj", "test proj empty", 0);
  render_via_viewport = CREATE_CVAR("rvv", 0, 0, "Rendering use view port, if 1 else with projection matrix");

  r_debug = GET_CVAR("r_debug");
  cam_width = GET_CVAR("r_cam_w");
  cam_height = GET_CVAR("r_cam_h");
}

void GLRenderer::InitConsoleCommands()
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

void GLRenderer::SetRenderTarget(int nHandle)
{
  if (m_CurrentTarget != nHandle)
    gl::BindFramebuffer(nHandle);
}

IShader* GLRenderer::Sh_Load(ShaderDesc const& desc)
{
  return dynamic_cast<IShader*>(CShader::load(desc));
}

void GLRenderer::SetCullMode(CullMode mode/* = CullMode::BACK*/)
{
  if (mode == CullMode::FRONT_AND_BACK)
    gl::CullFace(GL_FRONT_AND_BACK);
  else
    gl::CullFace(GL_FRONT + static_cast<unsigned int>(mode));
}

bool GLRenderer::DeleteContext(WIN_HWND hWnd)
{
  return false;
}

bool GLRenderer::CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX/* = 1*/, int SSY/* = 1*/)
{
  return false;
}

bool GLRenderer::SetCurrentContext(WIN_HWND hWnd)
{
  return false;
}

void GLRenderer::MakeMainContextActive()
{
}

WIN_HWND GLRenderer::GetCurrentContextHWND()
{
  return m_HWND;
}

bool GLRenderer::IsCurrentContextMainVP()
{
  return false;
}

int GLRenderer::GetCurrentContextViewportHeight() const
{
  return 0;
}

int GLRenderer::GetCurrentContextViewportWidth() const
{
  return 0;
}

void GLRenderer::SetClearColor(const Vec3& vColor)
{
  m_clearColor = vColor;
  gl::ClearColor(Vec4(vColor, 1));
}

void GLRenderer::ClearDepthBuffer()
{
  //glCheck(glClearBufferfv(GL_DEPTH, 0, &m_clearDepth));

  glCheck(glClearDepthf(1.f));
  gl::Clear(GL_DEPTH_BUFFER_BIT);
}

void GLRenderer::ClearColorBuffer(const Vec3 vColor)
{
  glCheck(glClearBufferfv(GL_COLOR, 0, &glm::vec4(vColor, 1)[0]));
}

void GLRenderer::SetState(State state, bool enable)
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

void GLRenderer::DrawFullScreenImage(int texture_id)
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

bool GLRenderer::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
#if 0
  if (!strcmp(pVar->GetName(), "r_Width"))
  {
    m_Window->changeSize(pVar->GetFVal(), GetHeight());
  }
  else if (!strcmp(pVar->GetName(), "r_Height"))
  {
    m_Window->changeSize(GetWidth(), pVar->GetFVal());
  }
#endif
  if (!strcmp(pVar->GetName(), "r_cam_w"))
  {
    printf("");
  }
  else if (!strcmp(pVar->GetName(), "r_debug"))
  {
    //OpenglDebuger::SetIgnore(!r_debug->GetIVal());
    OpenglDebuger::SetIgnore(!(bool)std::stoi(sNewValue));
  }
  return false;
}

void GLRenderer::Draw3dBBox(const Vec3& mins, const Vec3& maxs)
{
}

void GLRenderer::DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)
{
  bool flipY = true;
  float
    width = GetWidth(),
    height = GetHeight();
  //gl::BindFramebuffer(0);
  SetState(State::BLEND, true);
  SetState(IRenderer::State::CULL_FACE, false);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  m_ScreenShader->Use();
  m_ScreenShader->Uniform(a, "alpha");

  glm::mat4 model(1.0);
  auto uv_projection = glm::mat4(1.0);
  glm::mat4 projection = glm::ortho(0.0f, width, height, 0.0f, -1.0f, 1000.0f);

  model = glm::translate(model, glm::vec3(1.0f, 1.0f, 0.f));
  model = glm::scale(model, { w, h, 1.f });

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

void GLRenderer::PrintLine(const char* szText, SDrawTextInfo& info)
{
  Draw2dText(info.font->GetXPos(), info.font->GetYPos(), szText, info);
}

bool GLRenderer::OnInputEvent(const SInputEvent& event)
{
  switch (event.deviceType)
  {
    break;
  default:
    break;
  }
  return false;
}

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
  return new GLRenderer(pSystem);
}

#pragma warning(push)