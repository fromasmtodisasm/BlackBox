#include <BlackBox/World.hpp>


World::World()
{

}

void World::draw(float dt) {
  World *world = this;
  activeScene->draw(dt);
  // Camera ...
}

void World::setCamera(CCamera *camera)
{
  //m_Camera = camera;
}

void World::addScene(string name, Scene *scene) {
  m_Scenes[name] = scene;
}

void World::update(float deltatime)
{
  for (const auto &object : m_Objs) {
    object.second->velocity.y -= gravity;
    object.second->update(deltatime);
  }
}
