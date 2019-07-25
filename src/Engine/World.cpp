#include <BlackBox/World.hpp>

float World::gravity = 1;

Scene *World::getActiveScene() const
{
    return activeScene;
}

World::World()
{
    
}

void World::draw(float dt) {
  activeScene->begin();
  if (m_PreRender != nullptr)
    m_PreRender->PreRender();
  activeScene->draw(dt);
  if (m_PostRender != nullptr)
    m_PostRender->PostRender();
  activeScene->end();
  // Camera ...
}

void World::setCamera(CCamera *camera)
{
  activeScene->setCamera(camera);
}

void World::setScene(Scene *scene) {
  activeScene = scene;
}

void World::update(float deltatime)
{
  activeScene->update(deltatime);
}

void World::setPretRenderCallback(IPreRenderCallback* callBack)
{
  m_PreRender = callBack;
}

void World::setPostRenderCallback(IPostRenderCallback* callBack)
{
  m_PostRender = callBack;
}
