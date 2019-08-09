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

bool ShadowMapping::Init(Scene* scene)
{
  m_Scene = scene;
  //
	m_DepthBuffer = new FrameBufferObject(FrameBufferObject::buffer_type::DEPTH_BUFFER, width, height);
  m_DepthBuffer->create();

	m_RenderedScene = new FrameBufferObject(
    FrameBufferObject::buffer_type::SCENE_BUFFER, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight()
  );
  m_RenderedScene->create();

  m_ShadowMapShader = new ShadowMapShader();
  m_ShadowMapShader->create();
  
  //==============
	lightPosX = GetIEngine()->getIConsole()->CreateVariable("lpx", -1.f, 0, "light pos x");
	lightPosY = GetIEngine()->getIConsole()->CreateVariable("lpy", 15.f, 0, "light pos y");
	lightPosZ = GetIEngine()->getIConsole()->CreateVariable("lpz", -1.f, 0, "light pos z");
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
  float divider = s_divider->GetFVal();
  lightPos = glm::vec3(lightPosX->GetFVal(), lightPosY->GetFVal(), lightPosZ->GetFVal());
  m_DepthBuffer->bind();
  glViewport(0, 0, m_DepthBuffer->width, m_DepthBuffer->height);
  glClear(GL_DEPTH_BUFFER_BIT);
  m_ShadowMapShader->use();
  glm::mat4 proj = glm::ortho(-1366 / divider, 1366 / divider, -768 / divider, 768 / divider, -1.0f, 5000.f);

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
  auto camera = m_Scene->getCamera();

  Pipeline::instance()->view = camera->getViewMatrix();
  Pipeline::instance()->projection = camera->getProjectionMatrix();
  Pipeline::instance()->view_pos = camera->Position;

  glViewport(0, 0, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight());
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
  auto camera = m_Scene->getCamera();
  if (!object->m_transparent && (object->visible()) && 
    glm::abs(glm::distance(camera->Position, object->m_transform.position)) < camera->zFar->GetFVal())
  {
    auto program = object->m_Material->program;
    program->use();
    program->setUniformValue(lightSpaceMatrix, "lightSpaceMatrix");
    program->setUniformValue(lightPos, "lightPos");
    Pipeline::instance()->shader = program;
    Pipeline::instance()->model = object->getTransform();

    m_Scene->setupLights(object);
    object->m_Material->apply(object);
    program->setUniformValue(1, "shadowMap");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_DepthBuffer->texture);

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
    m_Scene->setupLights(object);

    object->draw(m_Scene->getCamera());
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
  glCheck(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
  glCheck(glEnable(GL_DEPTH_TEST));
  RenderPass();
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
  return m_RenderedScene->texture;
}
