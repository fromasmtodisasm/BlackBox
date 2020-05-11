#if 0
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/HdrTechnique.hpp>
#include <BlackBox/Renderer/Object.hpp>
#include <BlackBox/Renderer/ShadowMapTechnique.hpp>
#include <BlackBox/Renderer/SkyBox.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Scene/IScene.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ISystem.hpp>

#define NBLOOM
#ifdef TEST
#define LOG(fmt, ...) log->Log(fmt, __VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif
constexpr auto IMAGE = 0;
constexpr auto PREVIOS = 0;
constexpr auto CURRENT = 1;

HdrTechnique::HdrTechnique()
  :
  shadowMapping(nullptr),

  m_HdrBuffer(nullptr),
  m_Scene(nullptr),

  exposure(nullptr),
  enabled(nullptr),
  bloom(nullptr),
  bloomThreshold(nullptr),
  blurOn(nullptr),
  useBoxFilter(nullptr),
  defaultFilter(nullptr),
  bloom_exposure(nullptr),
  offset(nullptr),
  bloomTime(nullptr),
  upsampleTime(nullptr),
  downsampleTime(nullptr),
  averageBloomTime(nullptr),
  downsampleType(nullptr),
  blurOnly(nullptr),
  cam_height(nullptr),
  cam_width(nullptr),
  render(nullptr),
  log(nullptr),

  quadCorners{ 0 },
  quadCornersVBO{ 0 },
  testid(-1),
  timer_queries{ 0 }
{
}

HdrTechnique::~HdrTechnique()
{
  DeleteFrameBuffers();
  SAFE_DELETE(shadowMapping);
}

bool HdrTechnique::Init(IScene* pScene, FrameBufferObject* renderTarget)
{
  if (inited)
    return true;
  render = GetISystem()->GetIRenderer();
  log = GetISystem()->GetILog();
  m_Scene = pScene;
  InitConsoleVariables();
  initTest();
  createShader();
  CreateCommands();
  shadowMapping = new ShadowMapping();
  auto console = GetISystem()->GetIConsole();

  //auto format = m_DispFormats.get();

  //CreateFrameBuffers(format);
  CreateFrameBuffers(nullptr);
  m_HdrBuffer->clear(gl::Color(1, 0, 0, 1));

  // generate three query objects
  glGenQueries(3, timer_queries);

  inited = true;
  return shadowMapping->Init(pScene, m_HdrBuffer);
}

void HdrTechnique::CreateFrameBuffers(SDispFormat* format)
{
  glm::ivec2 resolution;
  auto w = GET_CVAR("r_backbuffer_w");
  auto h = GET_CVAR("r_backbuffer_h");
  if (format != nullptr)
  {
    resolution = glm::ivec2(format->m_Width, format->m_Height);
  }
  else if (w == nullptr || h == nullptr)
  {
    resolution = glm::ivec2(render->GetWidth(), render->GetHeight());
    GetISystem()->Log("Use window resulution");
  }
  else
    resolution = glm::ivec2(w->GetIVal(), h->GetIVal());
  //cam_width->Set(resolution.x);
  //cam_height->Set(resolution.y);

  m_HdrBuffer = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, resolution.x, resolution.y, 2, false);
  debuger::frame_buffer_label(m_HdrBuffer->id, "hdrBuffer");
  m_HdrBuffer->bind();
  m_HdrBuffer->clear(gl::Color(1, 1, 1, 1));
  m_HdrBuffer->unbind();

  auto mip_cnt = getMips(resolution);
  m_DownsampleBuffer.resize(mip_cnt);
  m_UpsampleBuffer.resize(mip_cnt);

  auto create_mip_chain = [&resolution, mip_cnt](std::vector<FrameBufferObject*>& chain) {
    for (int i = 0, width = resolution.x, height = resolution.y; i < mip_cnt; i++)
    {
      chain[i] = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, width, height, 1, false);
      chain[i]->bind();
      chain[i]->clear(gl::Color(1, 0, 0, 1));
      chain[i]->unbind();
      width >>= 1;
      if (width <= 0)
        width = 1;
      height >>= 1;
      if (height <= 0)
        height = 1;
    }
  };

  create_mip_chain(m_DownsampleBuffer);
  create_mip_chain(m_UpsampleBuffer);

  auto label_buffer = [](std::vector<FrameBufferObject*>& fb, std::string base) {
    int i = 0;
    for (auto& pass : fb)
    {
      debuger::frame_buffer_label(pass->id, base + std::to_string(i));
      i++;
    }
  };

  label_buffer(m_DownsampleBuffer, "pass0_");
  label_buffer(m_UpsampleBuffer, "pass1_");
}

void HdrTechnique::DeleteFrameBuffers()
{
  SAFE_DELETE(m_HdrBuffer);
  if (m_DownsampleBuffer.size() > 0)
  {
    for (auto& buf : m_DownsampleBuffer)
    {
      SAFE_DELETE(buf);
    }
  }
  if (m_DownsampleBuffer.size() > 0)
  {
    for (auto& buf : m_DownsampleBuffer)
    {
      SAFE_DELETE(buf);
    }
  }
  m_DownsampleBuffer.erase(m_DownsampleBuffer.begin(), m_DownsampleBuffer.end());
  m_UpsampleBuffer.erase(m_UpsampleBuffer.begin(), m_UpsampleBuffer.end());
}

bool HdrTechnique::OnRenderPass(int pass)
{
  DEBUG_GROUP(__FUNCTION__);
  if (!shadowMapping->OnRenderPass(pass))
  {
		SRenderParams renderParams;
    if (draw_sky && m_Scene->GetSkyBox() != nullptr)
      m_Scene->GetSkyBox()->draw(renderParams);
    {
      CStatObj* water;
      if ((water = m_Scene->getObject("water")) != nullptr)
      {
        water->m_transparent = true;
        static_cast<ShadowMapping*>(shadowMapping)->RenderTransparent(water);
      }
    }
    return HdrPass();
  }
  return true;
}

int HdrTechnique::GetFrame()
{
  return shadowMapping->GetFrame();
}

bool HdrTechnique::HdrPass()
{
  if (enabled->GetIVal())
  {
    m_Scene->setPostProcessor(this);
    PROFILER_PUSH_GPU_MARKER("BLOOM PASS", Utils::COLOR_DARK_GREEN);
    BloomPass();
    PROFILER_POP_GPU_MARKER();
  }
  else
    ;// m_Scene->setPostProcessor(nullptr);

  return false;
}

void HdrTechnique::BloomPass()
{
#if 0
  GLuint64 time_0, time_1, time_2;
#endif

#if 0
  {
    const char section[] = "DOWNSAMPLING";
    PROFILER_PUSH_CPU_MARKER(section, Utils::COLOR_BLACK);
    DEBUG_GROUP(section);
    downsampling();
    PROFILER_POP_CPU_MARKER();
  }
  {
    const char section[] = "UPSAMPLING";
    PROFILER_PUSH_CPU_MARKER(section, Utils::COLOR_RED);
    DEBUG_GROUP(section);
    upsampling();
    PROFILER_POP_CPU_MARKER();
  }
#endif

#if 0
  glGetQueryObjectui64v(timer_queries[0], GL_QUERY_RESULT, &time_0);
  glGetQueryObjectui64v(timer_queries[1], GL_QUERY_RESULT, &time_1);
  glGetQueryObjectui64v(timer_queries[2], GL_QUERY_RESULT, &time_2);

  upsampleTime->Set(float((time_1 - time_0) / 1e06));
  downsampleTime->Set(float((time_2 - time_1) / 1e06));
#endif
}

void HdrTechnique::createShader()
{
  ProgramDesc desc[] = {
      "hdr_shader",
      "downsampling",
      "downsampling_compute",
      "upsampling",
      "upsampling_compute",
  };

  desc[1].vs.macro["STORE_TEXCOORDS"] = 1;

  m_ScreenShader = gEnv->pRenderer->Sh_Load("HDR", 0);
  m_DownsampleShader = gEnv->pRenderer->Sh_Load("Downsampling", 0);
  m_UpsampleShader = gEnv->pRenderer->Sh_Load("Upsampling", 0);
  {
    m_ScreenShader->Use();
    m_ScreenShader->Uniform(0, "scene");
    m_ScreenShader->Uniform(1, "bloomBlur");
    m_ScreenShader->Unuse();
  }
  {
    m_DownsampleShader->Use();
    m_DownsampleShader->Uniform(0, "image");
    m_DownsampleShader->Unuse();
  }
  {
    m_DownsampleComputeShader->Use();
    m_DownsampleComputeShader->Uniform(0, "image");
    m_DownsampleComputeShader->Unuse();
  }

}

void HdrTechnique::InitConsoleVariables()
{
  exposure = CREATE_CVAR("exp", 1.0f, 0, "exposure");
  enabled = CREATE_CVAR("hdr", 1, 0, "Enable/disable HDR");
  bloom = CREATE_CVAR("bloom", 1, 0, "Enable/disable HDR");
  bloomThreshold = CREATE_CVAR("bt", 2.0f, 0, "Bloom threshold");
  blurOn = CREATE_CVAR("blur", 1, 0, "Enable/disable blur for bloom");
  blurOnly = CREATE_CVAR("blur_only", 0, 0, "Enable/disable blur for bloom");
  bloom_exposure = CREATE_CVAR("bexp", 0.007f, 0, "Enable/disable blur for bloom");
  bloomTime = CREATE_CVAR("bloomtime", 0.f, 0, "Time of bloom");
  upsampleTime = CREATE_CVAR("uptime", 0.f, 0, "Time of bloom");
  downsampleTime = CREATE_CVAR("dtime", 0.f, 0, "Time of bloom");
  downsampleType = CREATE_CVAR("dtype", 0, 0, "Type of bloom downsample [0-standard/1-compute]");
  offset = CREATE_CVAR("offset", -3.0f, 0, "Enable/disable blur for bloom");
  useBoxFilter = CREATE_CVAR("bf", 0, 0, "Enable/disable BoxFilter in bloom");
  defaultFilter = CREATE_CVAR("df", 1, 0, "Enable/disable default filtering in bloom");

  cam_width = GetISystem()->GetIConsole()->GetCVar("r_cam_w");
  cam_height = GetISystem()->GetIConsole()->GetCVar("r_cam_h");

  //GetISystem()->GetIConsole()->CreateKeyBind("ctrl_s", "#retrigger_value(\"show_all_frame_buffer\")");
  REGISTER_CVAR(show_all_fb, 0, VF_NULL, "Show all frame buffer");
  REGISTER_CVAR(draw_sky, 0, VF_NULL, "Draw skybox");
}

void HdrTechnique::initTest()
{
  auto render = GetISystem()->GetIRenderer();
  auto numFormats = render->EnumDisplayFormats(nullptr);
  if (numFormats > 0)
  {
    m_DispFormats = std::shared_ptr<SDispFormat>(new SDispFormat[numFormats]);
    render->EnumDisplayFormats(m_DispFormats.get());
  }

  testid = std::min(GetISystem()->GetIConsole()->GetCVar("testid")->GetIVal(), numFormats);
}

int HdrTechnique::getMips(Vec2 resolution)
{
  return static_cast<int>(std::log2(std::max(m_HdrBuffer->viewPort.z, m_HdrBuffer->viewPort.w)) + 1) - 5;
  //return 6;
}

void HdrTechnique::CreateCommands()
{
  struct HdrCommands : public IConsoleCommand
  {
  public:
    HdrCommands(HdrTechnique* tech) : tech(tech) {}
    virtual bool execute(CommandDesc& cd) override
    {
      GetISystem()->GetIConsole()->PrintLine("set mode!");
      if (cd.argsCount() == 1)
      {
        tech->SetMode(static_cast<int>(_wtof(cd.get(0).c_str())));
        return true;
      }
      return false;
    }
    HdrTechnique* tech;
  };

  IConsoleCommand* cmd = new HdrCommands(this);
  GetISystem()->GetIConsole()->AddCommand(
    "setmode",
    cmd,
    "Set display mode"
  );
}

int HdrTechnique::SetRenderTarget(FrameBufferObject* renderTarget)
{
  return 0;
}

void HdrTechnique::SetMode(int n)
{
  DeleteFrameBuffers();
  CreateFrameBuffers(&m_DispFormats.get()[n]);
  shadowMapping->SetRenderTarget(m_HdrBuffer);
}

bool HdrTechnique::PreRenderPass()
{
  return false;
}

void HdrTechnique::PostRenderPass()
{
}

void HdrTechnique::downsampling()
{
  LOG("Start downsampling\n");
  if (downsampleType->GetIVal() == 0)
    downsamplingStandard();
  else
    downsamplingCompute();
}

void HdrTechnique::downsamplingStandard()
{
#if 0
  // 2. blur bright fragments with two-pass Gaussian Blur
  // --------------------------------------------------
  bool first_iteration = true;
  unsigned int amount = PASSES;
  auto& ds = m_DownsampleShader;
  ds->Use();
  ds->Uniform(offset->GetFVal(), "offset");

  render->SetState(IRenderer::State::DEPTH_TEST, false);
  amount = getMips({ m_DownsampleBuffer[0]->viewPort.z, m_DownsampleBuffer[0]->viewPort.w });

  const float w = cam_width->GetIVal();
  const float h = cam_height->GetIVal();
  const auto hdr_w = m_HdrBuffer->viewPort.z;
  const auto hdr_h = m_HdrBuffer->viewPort.w;

  ds->Uniform((w / hdr_w), "vx");
  ds->Uniform((h / hdr_h), "vy");
  LOG("\tvx = %f\n", (w / hdr_w));
  LOG("\tvy = %f\n", (h / hdr_h));

  for (unsigned int i = 0; i < amount - 1; i++)
  {
    LOG("\tBegin %d iteration\n", i);
    int rx = w / (1 << (i));
    int ry = h / (1 << (i));
    auto vpx = w / (1 << (i + 1));
    auto vpy = h / (1 << (i + 1));
    ds->Uniform(rx, "rx");
    ds->Uniform(ry, "ry");
    LOG("\t\trx = %f\n", rx);
    LOG("\t\try = %f\n", ry);
    m_DownsampleBuffer[i + 1]->bind({ 0,0, vpx, vpy });

    ds->BindTextureUnit2D(first_iteration ? m_HdrBuffer->texture[0] : m_DownsampleBuffer[i]->texture[0], IMAGE);
    m_ScreenQuad.draw();
    if (first_iteration)
      first_iteration = false;
  }
  ds->Unuse();

#if 0
  glBindBufferBase(GL_ARRAY_BUFFER, 2, quadCornersVBO);
  Vec2* buf = static_cast<Vec2*>(glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY));

  for (int i = 0; i < 4; i++)
  {
    std::cout << "Corner " << i << buf[i].x << ", " << buf[i].y;
  }
#endif

  GetISystem()->GetIRenderer()->SetState(IRenderer::State::DEPTH_TEST, true);
#endif
}

void HdrTechnique::downsamplingCompute()
{
  m_DownsampleComputeShader->Use();
  {
    unsigned int image_unit = 2;
    m_DownsampleComputeShader->BindTexture2D(m_HdrBuffer->texture[1], image_unit, "inputImg1");
  }

  {
    unsigned int image_unit = 3;
    glBindImageTexture(image_unit, m_UpsampleBuffer[0]->texture[0], 0, false, 0, GL_WRITE_ONLY, GL_RGBA16F);
    m_DownsampleComputeShader->Uniform(3, "inputImg2");
  }

  auto render = GetISystem()->GetIRenderer();
  m_DownsampleComputeShader->Dispatch(render->GetWidth() / 6, render->GetHeight() / 6, 1, GL_SHADER_STORAGE_BARRIER_BIT | GL_TEXTURE_FETCH_BARRIER_BIT);
  return;
}

void HdrTechnique::upsampling()
{
  LOG("Start upsampling\n");
  auto& up = m_UpsampleShader;
  up->Use();
  up->Uniform(blurOn->GetIVal(), "blurOn");
  up->Uniform(blurOnly->GetIVal(), "blurOnly");

  uint32_t amount;
  bool first_iteration = true;
  render->SetState(IRenderer::State::DEPTH_TEST, false);

  float w = (float)cam_width->GetIVal();
  float h = (float)cam_height->GetIVal();
  auto& hdr_w = m_HdrBuffer->viewPort.z;
  auto& hdr_h = m_HdrBuffer->viewPort.w;
  up->Uniform((w / hdr_w), "vx");
  up->Uniform((h / hdr_h), "vy");

  amount = getMips({ m_DownsampleBuffer[0]->viewPort.z, m_DownsampleBuffer[0]->viewPort.w });
  for (unsigned int i = amount - 1; i > 0; i--)
  {
    LOG("\tBegin %d iteration\n", i);
    int rx = static_cast<int>(w / ((1 << i) - 1));
    int ry = static_cast<int>(h / ((1 << i) - 1));
    // Texture that blured
    auto& blured = m_UpsampleBuffer[i]->texture[0];
    // Texture on which the blur is superimposed
    auto& current_level = i == 1 ? m_HdrBuffer->texture[0] : m_DownsampleBuffer[i - 1]->texture[0];

    auto& rt = m_UpsampleBuffer[i - 1]; // Render target
    up->Uniform(rx, "rx");
    up->Uniform(ry, "ry");
    LOG("\t\trx = %d\n", rx);
    LOG("\t\try = %d\n", ry);

    rt->bind(Vec4(0, 0, rx, ry));
    up->BindTexture2D(blured, PREVIOS, "blured");
    up->BindTexture2D(current_level, CURRENT, "current");
    m_ScreenQuad.draw();
    if (first_iteration)
      first_iteration = false;
  }
  up->Unuse();
}

void HdrTechnique::Do(unsigned int texture)
{
  DEBUG_GROUP(__FUNCTION__);
  float w = (float)cam_width->GetIVal();
  float h = (float)cam_height->GetIVal();
  auto hdr_w = m_HdrBuffer->viewPort.z;
  auto hdr_h = m_HdrBuffer->viewPort.w;
  auto& ss = m_ScreenShader;

  ss->Use();
  ss->Uniform(exposure->GetFVal(), "exposure");
  ss->Uniform(bloom_exposure->GetFVal(), "bloom_exposure");
  render->SetState(IRenderer::State::DEPTH_TEST, false);

  ss->BindTexture2D(m_HdrBuffer->texture[0], 0, "scene");
  ss->BindTexture2D(m_UpsampleBuffer[0]->texture[0], 1, "bloomBlur");
  ss->Uniform(bloom->GetIVal(), "bloom");
  Vec2 scale(1.f);
  if (!show_all_fb)
    scale = Vec2(w, h) / Vec2(hdr_w, hdr_h);

  //auto uv_projection = Mat4(1.f);
  //uv_projection = glm::scale(uv_projection, glm::vec3(scale, 1.f));
  //ss->Uniform(uv_projection, "uv_projection");
  ss->Uniform(scale, "scale");
  FrameBufferObject::bindDefault({ 0,0, render->GetWidth(), render->GetHeight() });
  m_ScreenQuad.draw();
}
#endif
