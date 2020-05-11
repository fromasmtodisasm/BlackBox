#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/World/World.hpp>
#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/3DEngine/SceneManager.hpp>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>

float World::gravity = 9.8f;

#if 0
IScene* World::GetActiveScene()
{
  return activeScene;
}

World::World()
  :
  activeScene(nullptr)
{
  REGISTER_CVAR(gravity, gravity, VF_NULL, "gravity");
}

void World::Draw(float dt) {
  activeScene->begin();
  if (m_PreRender != nullptr)
    m_PreRender->PreRender();
  activeScene->draw(dt);
  if (m_PostRender != nullptr)
    m_PostRender->PostRender();
  activeScene->end();
  // Camera ...
}

void World::SetCamera(CCamera* camera)
{
  //activeScene->setCamera(camera);
}

void World::SetScene(IScene* scene) {
  activeScene = dynamic_cast<Scene*>(scene);
}

void World::Update(float deltatime)
{
  if (activeScene != nullptr)
  {
    PROFILER_PUSH_CPU_MARKER(__FUNCTION__, Utils::COLOR_DARK_RED);
    activeScene->update(deltatime);
    PROFILER_POP_CPU_MARKER();
  }
}

void World::SetPretRenderCallback(IPreRenderCallback* callBack)
{
  m_PreRender = callBack;
}

void World::SetPostRenderCallback(IPostRenderCallback* callBack)
{
  m_PostRender = callBack;
}
#endif