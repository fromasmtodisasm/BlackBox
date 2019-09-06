#include <BlackBox/Render/ShadowMapTechnique.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/ShadowMapShader.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/IGame.hpp>

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
  m_Scene = scene;
  //
  m_DepthBuffer = FrameBufferObject::create(FrameBufferObject::BufferType::DEPTH_BUFFER, width, height, 1);

  if (renderTarget == nullptr)
  {
    m_RenderedScene = FrameBufferObject::create(
      FrameBufferObject::BufferType::SCENE_BUFFER, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight(), 0
    );
  }
  else
  {
    m_RenderedScene = renderTarget;
  }

  m_ShadowMapShader = new ShadowMapShader();
  m_ShadowMapShader->create();
  
  //==============
	lightPosX = GetIEngine()->getIConsole()->CreateVariable("lpx", -1.f, 0, "light pos x");
	lightPosY = GetIEngine()->getIConsole()->CreateVariable("lpy", 15.f, 0, "light pos y");
	lightPosZ = GetIEngine()->getIConsole()->CreateVariable("lpz", -1.f, 0, "light pos z");
	lighting =  GetIEngine()->getIConsole()->CreateVariable("lighting", bLighting, 0, "light pos z");
	s_divider = GetIEngine()->getIConsole()->CreateVariable("sd", 10.0f, 0, "ortho divider");

  //
  return true;
}

bool ShadowMapping::OnRenderPass(int pass)
{
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
  float m = s_divider->GetFVal();
  lightPos = glm::vec3(lightPosX->GetFVal(), lightPosY->GetFVal(), lightPosZ->GetFVal());
  m_DepthBuffer->bind();
  glViewport(0, 0, m_DepthBuffer->width, m_DepthBuffer->height);
  glClear(GL_DEPTH_BUFFER_BIT);
  m_ShadowMapShader->use();
  glm::mat4 proj = glm::ortho(-1366.f * m, 1366.f * m, -768.f * m, 768.f * m, -1.0f, 5000.f);

  lightSpaceMatrix = proj *
    glm::lookAt(lightPos,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));
  m_ShadowMapShader->setUniformValue(lightSpaceMatrix, "lightSpaceMatrix");

  renderStage = RENDER_DEPTH;
  m_Scene->ForEachObject(this);
  
  m_ShadowMapShader->unuse();
  m_DepthBuffer->unbind();
}

void ShadowMapping::RenderPass()
{
  auto camera = m_Scene->getCurrentCamera();

  Pipeline::instance()->view = camera->getViewMatrix();
  Pipeline::instance()->projection = camera->getProjectionMatrix();
  Pipeline::instance()->view_pos = camera->getPosition();

  glViewport(0, 0, m_RenderedScene->width, m_RenderedScene->height);
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
	points->shader->use();
	points->shader->setUniformValue(
		Pipeline::instance()->projection *
		Pipeline::instance()->view *
		Pipeline::instance()->model,
		"MVP"
	);
	points->draw();
	points->shader->unuse();
  
  // Render transparent objects
  renderStage = RENDER_TRANSPARENT;
  m_Scene->ForEachObject(this);
  
}

void ShadowMapping::RenderDepth(Object* object)
{
  if (object->visible())
  {
    m_ShadowMapShader->setUniformValue(object->getTransform(), "model");
    m_ShadowMapShader->setup();
    object->draw(nullptr);
  }
}

void ShadowMapping::RenderOpaque(Object* object)
{
  auto camera = m_Scene->getCurrentCamera();
  if (!object->m_transparent && (object->visible()) && 
    glm::abs(glm::distance(camera->getPosition(), object->m_transform.position)) < camera->zFar->GetFVal())
  {
    auto program = object->m_Material->program;
    program->use();
    program->setUniformValue(lightSpaceMatrix, "lightSpaceMatrix");
    program->setUniformValue(lightPos, "lightPos");
		program->setUniformValue(GetIEngine()->getIConsole()->GetCVar("bt")->GetFVal(), "bloomThreshold");
    //program->setUniformValue(bLighting, "lightOn");
    Pipeline::instance()->shader = program;
    Pipeline::instance()->model = object->getTransform();

    SetupLights(object);
    object->m_Material->apply(object);
    program->setUniformValue(1, "shadowMap");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_DepthBuffer->texture[0]);

		if (object->m_path.find("terrain") != object->m_path.npos)
			program->setUniformValue(true, "isTerrain");
		else
			program->setUniformValue(false, "isTerrain");
    object->draw(camera);
  }

}

void ShadowMapping::RenderTransparent(Object* object)
{
  if (object->m_transparent && (object->visible()))
  {
    auto program = object->m_Material->program;
    program->use();
    program->setUniformValue(lightSpaceMatrix, "lightSpaceMatrix");
    program->setUniformValue(lightPos, "lightPos");
		program->setUniformValue(GetIEngine()->getIConsole()->GetCVar("bt")->GetFVal(), "bloomThreshold");

    SetupLights(object);
    object->m_Material->apply(object);

    object->draw(m_Scene->getCurrentCamera());
  }
}

void ShadowMapping::OnDepthPass()
{
  glCullFace(GL_FRONT);
  DepthPass();
  glCullFace(GL_BACK);
}

void ShadowMapping::OnRenderPass()
{
  m_RenderedScene->bind();
	glm::vec3 fog = glm::vec3(
		GetIEngine()->getIConsole()->GetCVar("fogR")->GetFVal(),
		GetIEngine()->getIConsole()->GetCVar("fogG")->GetFVal(),
		GetIEngine()->getIConsole()->GetCVar("fogB")->GetFVal());
  //glCheck(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
	glClearColor(fog.r, fog.g, fog.b, 1.0f);
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  glCheck(glEnable(GL_DEPTH_TEST));
  RenderPass();
}

void ShadowMapping::SetupLights(Object *object)
{
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

  object->m_Material->program->setUniformValue(currentLight + 1, "countOfPointLights");
 
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
  for (const auto& lightit : m_Scene->GetPointLights())
  {
    auto light = lightit.second;
    auto program = m_ShadowMapShader;
    PointLightValues plv;


    //program->getUniformValue("pointLights[%d].position");
    program->setUniformValue(light->position,   "pointLights[%d].position", currentLight);
    program->setUniformValue(light->ambient,    "pointLights[%d].ambient", currentLight);
    program->setUniformValue(light->diffuse,    "pointLights[%d].diffuse", currentLight);
    program->setUniformValue(light->specular,   "pointLights[%d].specular", currentLight);
    program->setUniformValue(light->constant,   "pointLights[%d].constant", currentLight);
    program->setUniformValue(light->linear,     "pointLights[%d].linear", currentLight);
    program->setUniformValue(light->quadratic,  "pointLights[%d].quadratic", currentLight);
  }
}

bool ShadowMapping::OnObjectFound(Object* object)
{
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
  auto program = Pipeline::instance()->object->m_Material->program;
  program->setUniformValue(light->direction, "dirLight.direction");
  program->setUniformValue(light->ambient, "dirLight.ambient");
  program->setUniformValue(light->diffuse, "dirLight.diffuse");
  program->setUniformValue(light->specular, "dirLight.specular");
  return true;
}

bool ShadowMapping::OnLightFound(PointLight* light)
{
  if (light->enabled)
  {
    auto program = Pipeline::instance()->object->m_Material->program;
    program->setUniformValue(light->position, "pointLights[%d].position", currentLight);
    program->setUniformValue(light->ambient, "pointLights[%d].ambient", currentLight);
    program->setUniformValue(light->diffuse, "pointLights[%d].diffuse", currentLight);
    program->setUniformValue(light->specular, "pointLights[%d].specular", currentLight);
    program->setUniformValue(light->constant, "pointLights[%d].constant", currentLight);
    program->setUniformValue(light->linear, "pointLights[%d].linear", currentLight);
    program->setUniformValue(light->quadratic, "pointLights[%d].quadratic", currentLight);
    ++currentLight;

    //program->setUniformValue(light.second->position, "lightPos", currentLight);
    //program->setUniformValue(glm::vec3(lightPosX->GetFVal(), lightPosY->GetFVal(), lightPosZ->GetFVal()), "lightPos", currentLight);
  }
  return true;
}

bool ShadowMapping::OnLightFound(SpotLight* light)
{
  
  auto program = Pipeline::instance()->object->m_Material->program;
  program->setUniformValue(m_Scene->getCurrentCamera()->getPosition(), "spotLight.position");
  program->setUniformValue(m_Scene->getCurrentCamera()->Front, "spotLight.direction");
  program->setUniformValue(light->ambient, "spotLight.ambient");
  program->setUniformValue(light->diffuse, "spotLight.diffuse");
  program->setUniformValue(light->diffuse, "spotLight.specular");
  program->setUniformValue(light->constant, "spotLight.constant");
  program->setUniformValue(light->linear, "spotLight.linear");
  program->setUniformValue(light->quadratic, "spotLight.quadratic");
  program->setUniformValue(glm::cos(glm::radians(light->cutOff)), "spotLight.cutOff");
  program->setUniformValue(glm::cos(glm::radians(light->outerCutOff)), "spotLight.outerCutOff");
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
