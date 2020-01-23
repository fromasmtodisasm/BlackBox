#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/Renderer/IPostProcessor.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/ITechnique.hpp>
#include <BlackBox/Renderer/Object.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/World/World.hpp>

#include <algorithm>

Scene::Scene(std::string name)
  :
  lighting(true),
  name(name),
  m_RenderedScene(-1),
  m_Technique(nullptr),
  m_World(nullptr),
  quadVAO(-1),
  shadowMapMat(nullptr),
  skyBox(nullptr),
  m_Points(nullptr)
{
  float quadVertices[] = {
    // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
  };

  ProgramDesc pd = {
    "screen_shader",
    ShaderDesc("screenshader.vs"),
    ShaderDesc("screenshader.frag")
  };

  MaterialManager::instance()->loadProgram(pd, false);
  m_ScreenShader = MaterialManager::instance()->getProgram(pd.name);

  m_ScreenShader->Use();
  m_ScreenShader->Uniform(0, "screenTexture");
  m_ScreenShader->Unuse();

  texture_speed = GetISystem()->GetIConsole()->CreateVariable("tex_spd", 0.1f, 0, "Speed of texture animation");
}

// IScene
bool Scene::save(const char* as)
{
  Serializator serializator(this);
  return serializator.save(as);
}

bool Scene::load(const char* name, LoadObjectSink* callback)
{
  Serializator serializator(this);
  return serializator.load(name, callback);
}

void Scene::selectPrevObject()
{
  if (selected_object_it == m_Objects.begin())
  {
    selected_object_it = m_Objects.end();
  }
  else
  {
    selected_object_it--;
  }
}

void Scene::selectNextObject()
{
  if (selected_object_it == m_Objects.end())
  {
    selected_object_it = m_Objects.begin();
  }
  else
  {
    selected_object_it++;
  }
}

ObjectMapItr Scene::selectedObject()
{
  if (selected_object_it == m_Objects.end())
    return m_Objects.begin();
  return selected_object_it;
}

Object* Scene::getObject(std::string name)
{
  auto objectIt = m_Objects.find(name);
  if (objectIt != m_Objects.end())
  {
    return  objectIt->second;
  }
  return nullptr;
}

void Scene::addObject(std::string name, Object* object)
{
  m_Objects.insert(std::make_pair(name, object));
}

CCamera* Scene::getCurrentCamera()
{
  return m_CurrentCamera->second;
}

void Scene::present(int width, int height)
{
  DEBUG_GROUP(__FUNCTION__);
  if (postProcessor == nullptr)
  {
    GetISystem()->GetIRender()->DrawFullScreenImage(m_RenderedScene);
  }
  else
  {
    //PROFILER_PUSH_GPU_MARKER("Postprocessing", Utils::COLOR_GREEN);
    postProcessor->Do(m_RenderedScene);
  }
  //PROFILER_POP_GPU_MARKER();
}

size_t Scene::numObjects()
{
  return m_Objects.size();
}

void Scene::setTechnique(ITechnique* technique)
{
  m_Technique = technique;
}

void Scene::setPostProcessor(IPostProcessor* postProcessor)
{
  this->postProcessor = postProcessor;
}

bool Scene::selectObject(std::string name)
{
  auto it = m_Objects.find(name);
  if (it == m_Objects.end())
    return false;
  selected_object_it = it;
  return true;
}

SkyBox* Scene::GetSkyBox()
{
  return skyBox;
}

void Scene::draw(float dt)
{
  //auto tech_name = m_Technique->GetName();
  //sprintf(debug_label, "Technique: %s", tech_name);
  DEBUG_GROUP("Render Loop");
  if (m_Objects.size() > 0)
  {
    DEBUG_GROUP("PASS...");
    for (int pass = 0; m_Technique->OnRenderPass(pass); pass++);
  }
  if (m_Technique)
    m_RenderedScene = m_Technique->GetFrame();

  /*
  if (skyBox != nullptr)
    skyBox->draw(m_Camera);
  */
}

void Scene::setCamera(std::string name, CCamera* camera)
{
  m_Camera[name] = camera;
  m_CurrentCamera = m_Camera.find(name);
}

void Scene::update(float dt)
{
  for (auto& obj : m_Objects)
  {
    obj.second->update(dt);
    //obj.second->velocity.y -= World::gravity*dt;
  }
}

GLint Scene::getRenderTarget()
{
  return m_RenderedScene;
}

void Scene::begin()
{
  /*
  m_RenderedScene->bind();
  glCheck(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)); // буфер трафарета не используется
  glCheck(glEnable(GL_DEPTH_TEST));
  */
}

void Scene::end()
{
  //m_RenderedScene->unbind();
}

void Scene::ForEachObject(ForEachObjectSink* callback)
{
  for (const auto& object : m_Objects) {
    if (!callback->OnObjectFound(object.second))
      break;
  }
}

void Scene::ForEachDirectionLight(ForEachDirectionLightSink* callback)
{
  for (const auto& light : m_DirectionLight)
  {
    if (!callback->OnLightFound(light.second))
      break;
  }
}

void Scene::ForEachPointLight(ForEachPointLightSink* callback)
{
  for (const auto& light : m_PointLights)
  {
    if (!callback->OnLightFound(light.second))
      break;
  }
}

void Scene::ForEachSpotLight(ForEachSpotLightSink* callback)
{
  for (const auto& light : m_SpotLights)
  {
    if (!callback->OnLightFound(light.second))
      break;
  }
}

const PointLightList& Scene::GetPointLights()
{
  return m_PointLights;
}

Terrain* Scene::getTerrain()
{
  return &terrain;
}

PointObject* Scene::getPoints()
{
  return m_Points;
}

PointObject::PointObject()
{
  ProgramDesc pd = {
    "points",
    ShaderDesc("points.vert"),
    ShaderDesc("points.frag")
  };

  MaterialManager::instance()->loadProgram(pd, false);
  shader = MaterialManager::instance()->getProgram(pd.name);
}

PointObject::~PointObject()
{
}

void PointObject::draw()
{
  glBindVertexArray(VAO);
  glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(point_cnt));
  glBindVertexArray(0);
}
