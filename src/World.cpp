#include "World.hpp"


World::World()
{

}

void World::draw(float dt) {
  for (const auto &object : m_Objs) {
    //object.second->rotate(dt*0.01f, {0,1,0});
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS);
    object.second->getShaderProgram()->use();
    object.second->getShaderProgram()->setUniformValue("Model", object.second->getTransform());
    object.second->getShaderProgram()->setUniformValue("View", m_Camera->getViewMatrix());
    object.second->getShaderProgram()->setUniformValue("Projection", m_Camera->getProjectionMatrix());
    object.second->getShaderProgram()->setUniformValue("lightPos", glm::vec3(4,4,-4));
    object.second->getShaderProgram()->setUniformValue("lightColor", glm::vec3(1,1,1.0));
    //object.second->getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));

    object.second->draw();
  }
  // Camera ...
}

void World::setCamera(CCamera *camera)
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
  for (const auto &object : m_Objs) {
    object.second->update(deltatime);
  }
}
