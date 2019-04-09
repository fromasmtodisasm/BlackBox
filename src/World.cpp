#include "World.hpp"


World::World()
{

}

void World::draw(float dt) {
  for (const auto &object : m_Objs) {
    object.second->rotate(dt*0.01f, {0,1,0});
    object.second->getShaderProgram()->use();
    object.second->getShaderProgram()->setUniformValue("Model", object.second->getTransform());
    object.second->getShaderProgram()->setUniformValue("View", m_Camera->getViewMatrix());
    object.second->getShaderProgram()->setUniformValue("Projection", m_Camera->getProjectionMatrix());
    object.second->getShaderProgram()->setUniformValue("color", { 1.0f, 1,0 });

    object.second->draw();
  }
  // Camera ...
}

void World::setCamera(HackCamera *camera)
{
  m_Camera = camera;
}

void World::add(string name, Object * o) {
  if (o->getType() == OBJType::TCAM)
    m_Cams[name] = o;
  else if (o->getType() == OBJType::TPRIMITIVE)
    m_Objs[name] = o;
}

void World::update(float deltatime)
{
  m_Camera->update(deltatime);
}
