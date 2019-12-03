#include <BlackBox/Render/ShadowMapTechnique.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>

#include <BlackBox/Scene.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

ShadowMapping::ShadowMapping() : m_Scene(nullptr), m_DepthBuffer(nullptr)
{
}

ShadowMapping::~ShadowMapping()
{
}

bool ShadowMapping::Init(Scene* scene, FrameBufferObject* renderTarget)
{
  m_pRender = GetISystem()->GetIRender();
  m_Scene = scene;
  //
  m_DepthBuffer = FrameBufferObject::create(FrameBufferObject::BufferType::DEPTH_BUFFER, width, height, 1, false);

  if (renderTarget == nullptr)
  {
    m_RenderedScene = FrameBufferObject::create(
      FrameBufferObject::BufferType::SCENE_BUFFER, m_pRender->GetWidth(), m_pRender->GetHeight(), 0, false
    );
  }
  else
  {
    m_RenderedScene = renderTarget;
  }

  ProgramDesc pd = {
    "shadowpass",
    ShaderDesc("shadowpass.vs"),
    ShaderDesc("shadowpass.frag")
  };

  MaterialManager::instance()->loadProgram(pd, false);
  m_ShadowMapShader = MaterialManager::instance()->getProgram(pd.name);

  //==============
  lightPosX = CREATE_CVAR("lpx", -1.f, 0, "light pos x");
  lightPosY = CREATE_CVAR("lpy", 15.f, 0, "light pos y");
  lightPosZ = CREATE_CVAR("lpz", -1.f, 0, "light pos z");
  lighting = CREATE_CVAR("lighting", bLighting, 0, "light pos z");
  s_divider = CREATE_CVAR("sd", 10.0f, 0, "ortho divider");

  cam_width = GET_CVAR("r_cam_w");
  cam_height = GET_CVAR("r_cam_h");

  //
  return true;
}

bool ShadowMapping::OnRenderPass(int pass)
{
  DEBUG_GROUP(__FUNCTION__);
  renderPass = static_cast<Pass>(pass);
  switch (pass)
  {
  case RENDER_DEPTHMAP:
    OnDepthPass();
    return true;
  case RENDER_SHADOWS:
    OnRenderPass();
    return true;
  default:
    return false;
  }
  return false;
}

void ShadowMapping::DepthPass()
{
  DEBUG_GROUP(__FUNCTION__);
  float m = s_divider->GetFVal();
  lightPos = glm::vec3(lightPosX->GetFVal(), lightPosY->GetFVal(), lightPosZ->GetFVal());
  m_DepthBuffer->bind();
  m_DepthBuffer->clear();
  m_ShadowMapShader->Use();
  glm::mat4 proj = glm::ortho(-1366.f * m, 1366.f * m, -768.f * m, 768.f * m, -1.0f, 5000.f);

  lightSpaceMatrix = proj *
    glm::lookAt(lightPos,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));
  m_ShadowMapShader->Uniform(lightSpaceMatrix, "lightSpaceMatrix");

  renderStage = RENDER_DEPTH;
  m_Scene->ForEachObject(this);

  m_ShadowMapShader->Unuse();
  m_DepthBuffer->unbind();
}

void ShadowMapping::RenderPass()
{
  DEBUG_GROUP(__FUNCTION__);
  auto camera = m_Scene->getCurrentCamera();

  Pipeline::instance()->view = camera->getViewMatrix();
  Pipeline::instance()->projection = camera->getProjectionMatrix();
  Pipeline::instance()->view_pos = camera->getPosition();

  // Render opaque objects
  renderStage = RENDER_OPAQUE;
  m_Scene->ForEachObject(this);

  Pipeline::instance()->bindProgram("bb");
  auto obj = m_Scene->selectedObject();
  Pipeline::instance()->object = obj->second;
  for (auto& mesh : *obj->second->m_Mesh)
  {
    mesh.bb.draw();
  }

  auto points = m_Scene->getPoints();
  if (points != nullptr)
  {
    points->shader->Use();
    points->shader->Uniform(
      Pipeline::instance()->projection *
      Pipeline::instance()->view *
      Pipeline::instance()->model,
      "MVP"
    );
    if (auto ps = GetISystem()->GetIConsole()->GetCVar("point_size"))
    {
      points->shader->Uniform(ps->GetIVal(), "point_size");

    }
    points->draw();
    points->shader->Unuse();
  }

  // Render transparent objects
  renderStage = RENDER_TRANSPARENT;
  m_Scene->ForEachObject(this);
}

void ShadowMapping::RenderDepth(Object* object)
{
  DEBUG_GROUP(__FUNCTION__);
  if (object->visible())
  {
    m_ShadowMapShader->Uniform(object->getTransform(), "model");
    m_ShadowMapShader->setup();
    object->draw(nullptr);
  }
}

void ShadowMapping::RenderOpaque(Object* object)
{
  DEBUG_GROUP(__FUNCTION__);
  auto camera = m_Scene->getCurrentCamera();
  if (!object->m_transparent && (object->visible()) &&
    glm::abs(glm::distance(camera->getPosition(), object->m_transform.position)) < camera->zFar->GetFVal())
  {
    auto program = object->m_Material->program;
    program->Use();
    program->Uniform(lightSpaceMatrix, "lightSpaceMatrix");
    program->Uniform(lightPos, "lightPos");
    program->Uniform(object->m_Material->alpha, "alpha");
    //program->Uniform(bLighting, "lightOn");
    Pipeline::instance()->shader = program;
    Pipeline::instance()->model = object->getTransform();

    SetupLights(object);
    object->m_Material->apply(object);
    program->Uniform(1, "shadowMap");
    gl::ActiveTexture(GL_TEXTURE1);
    gl::BindTexture2D(m_DepthBuffer->texture[0]);

    if (object->m_path.find("terrain") != object->m_path.npos)
      program->Uniform(true, "isTerrain");
    else
      program->Uniform(false, "isTerrain");
    object->draw(camera);
  }
}

void ShadowMapping::RenderTransparent(Object* object)
{
  DEBUG_GROUP(__FUNCTION__);
  m_pRender->SetState(IRenderer::State::BLEND, true);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  if (object->m_transparent && (object->visible()))
  {
    auto program = object->m_Material->program;
    program->Use();
    program->Uniform(lightSpaceMatrix, "lightSpaceMatrix");
    program->Uniform(lightPos, "lightPos");
    program->Uniform(object->m_Material->alpha, "alpha");
    program->Uniform(GetISystem()->GetIConsole()->GetCVar("bt")->GetFVal(), "bloomThreshold");

    Pipeline::instance()->shader = program;
    Pipeline::instance()->model = object->getTransform();

    SetupLights(object);
    object->m_Material->apply(object);

    object->draw(m_Scene->getCurrentCamera());
  }
  m_pRender->SetState(IRenderer::State::BLEND, false);
}

void ShadowMapping::OnDepthPass()
{
  DEBUG_GROUP(__FUNCTION__);
  m_pRender->SetCullMode(IRenderer::CullMode::FRONT);
  DepthPass();
  m_pRender->SetCullMode(IRenderer::CullMode::BACK);
}

void ShadowMapping::OnRenderPass()
{
  DEBUG_GROUP(__FUNCTION__);
  auto& v = m_RenderedScene->viewPort;
  m_RenderedScene->bind({ 0,0, cam_width->GetIVal(), cam_height->GetIVal() });
  glm::vec4 fog = glm::vec4(
    GET_CVAR("fogR")->GetFVal(),
    GET_CVAR("fogG")->GetFVal(),
    GET_CVAR("fogB")->GetFVal(),
    1.f);
  auto pSystem = GetISystem();
  auto w = cam_width->GetIVal();
  auto h = cam_height->GetIVal();
  //pSystem->GetIRender()->SetState(IRenderer::State::SCISSOR_TEST, true);
  //pSystem->GetIRender()->SetScissor(0, 0, w, h);
  auto render = GetISystem()->GetIRender();
  //render->ClearDepthBuffer();
  m_RenderedScene->clear(gl::Color(fog));
  //pSystem->GetIRender()->SetState(IRenderer::State::SCISSOR_TEST, false);
  RenderPass();
}

void ShadowMapping::SetupLights(Object* object)
{
  DEBUG_GROUP(__FUNCTION__);
  if (!bLighting)
    return;
  currentLight = 0;
  Pipeline::instance()->object = object;
  // direction lights
  SetupDirectionLights();
  // point lights
  SetupPointLights();
  // spotLight
  SetupSpotLights();

  object->m_Material->program->Uniform(currentLight + 1, "countOfPointLights");
}

void ShadowMapping::SetupDirectionLights()
{
  m_Scene->ForEachDirectionLight(this);
}

void ShadowMapping::SetupPointLights()
{
  m_Scene->ForEachPointLight(this);
}

void ShadowMapping::SetupSpotLights()
{
  m_Scene->ForEachDirectionLight(this);
}

void ShadowMapping::InitLights()
{
  DEBUG_GROUP(__FUNCTION__);
  for (const auto& lightit : m_Scene->GetPointLights())
  {
    auto light = lightit.second;
    auto program = m_ShadowMapShader;
    //PointLightValues plv;
    //UNREFERENCED_PARAMETER(plv);

    //program->getUniformValue("pointLights[%d].position");
    program->Uniform(light->position, "pointLights[%d].position", currentLight);
    program->Uniform(light->ambient, "pointLights[%d].ambient", currentLight);
    program->Uniform(light->diffuse, "pointLights[%d].diffuse", currentLight);
    program->Uniform(light->specular, "pointLights[%d].specular", currentLight);
    program->Uniform(light->constant, "pointLights[%d].constant", currentLight);
    program->Uniform(light->linear, "pointLights[%d].linear", currentLight);
    program->Uniform(light->quadratic, "pointLights[%d].quadratic", currentLight);
  }
}

int ShadowMapping::SetRenderTarget(FrameBufferObject* renderTarget)
{
  m_RenderedScene = renderTarget;
  return 0;
}

bool ShadowMapping::OnObjectFound(Object* object)
{
  DEBUG_GROUP(__FUNCTION__);
  switch (renderStage)
  {
  case ShadowMapping::RENDER_DEPTH:
    RenderDepth(object);
    return true;
  case ShadowMapping::RENDER_OPAQUE:
    RenderOpaque(object);
    return true;
  case ShadowMapping::RENDER_TRANSPARENT:
    RenderTransparent(object);
    return true;
  default:
    return false;
  }
  return true;
}

int ShadowMapping::GetFrame()
{
  return m_RenderedScene->texture[0];
}

bool ShadowMapping::OnLightFound(DirectionLight* light)
{
  DEBUG_GROUP(__FUNCTION__);
  auto program = Pipeline::instance()->object->m_Material->program;
  program->Uniform(light->direction, "dirLight.direction");
  program->Uniform(light->ambient, "dirLight.ambient");
  program->Uniform(light->diffuse, "dirLight.diffuse");
  program->Uniform(light->specular, "dirLight.specular");
  return true;
}

bool ShadowMapping::OnLightFound(PointLight* light)
{
  DEBUG_GROUP(__FUNCTION__);
  if (light->enabled)
  {
    auto program = Pipeline::instance()->object->m_Material->program;
    program->Uniform(light->position, "pointLights[%d].position", currentLight);
    program->Uniform(light->ambient, "pointLights[%d].ambient", currentLight);
    program->Uniform(light->diffuse, "pointLights[%d].diffuse", currentLight);
    program->Uniform(light->specular, "pointLights[%d].specular", currentLight);
    program->Uniform(light->constant, "pointLights[%d].constant", currentLight);
    program->Uniform(light->linear, "pointLights[%d].linear", currentLight);
    program->Uniform(light->quadratic, "pointLights[%d].quadratic", currentLight);
    ++currentLight;

    //program->Uniform(light.second->position, "lightPos", currentLight);
    //program->Uniform(glm::vec3(lightPosX->GetFVal(), lightPosY->GetFVal(), lightPosZ->GetFVal()), "lightPos", currentLight);
  }
  return true;
}

bool ShadowMapping::OnLightFound(SpotLight* light)
{
  DEBUG_GROUP(__FUNCTION__);
  auto program = Pipeline::instance()->object->m_Material->program;
  program->Uniform(m_Scene->getCurrentCamera()->getPosition(), "spotLight.position");
  program->Uniform(m_Scene->getCurrentCamera()->Front, "spotLight.direction");
  program->Uniform(light->ambient, "spotLight.ambient");
  program->Uniform(light->diffuse, "spotLight.diffuse");
  program->Uniform(light->diffuse, "spotLight.specular");
  program->Uniform(light->constant, "spotLight.constant");
  program->Uniform(light->linear, "spotLight.linear");
  program->Uniform(light->quadratic, "spotLight.quadratic");
  program->Uniform(glm::cos(glm::radians(light->cutOff)), "spotLight.cutOff");
  program->Uniform(glm::cos(glm::radians(light->outerCutOff)), "spotLight.outerCutOff");
  return true;
}

bool ShadowMapping::PreRenderPass()
{
  bLighting = static_cast<bool>(lighting->GetIVal());
  return true;
}

void ShadowMapping::PostRenderPass()
{
}
