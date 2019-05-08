#include <BlackBox/Scene.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>

Scene::Scene()
{

}

void Scene::draw(float dt)
{
 for (const auto &object : m_Objs) {
    //object.second->rotate(dt*0.01f, {0,1,0});
    object.second->getShaderProgram()->use();
    object.second->getShaderProgram()->setUniformValue("Model", object.second->getTransform());
    object.second->getShaderProgram()->setUniformValue("View", m_Camera->getViewMatrix());
    object.second->getShaderProgram()->setUniformValue("Projection", m_Camera->getProjectionMatrix());
    object.second->getShaderProgram()->setUniformValue("lightPos", m_Objs["light"]->m_transform.position);
    object.second->getShaderProgram()->setUniformValue("lightColor", glm::vec3(1,1,1.0));
    //object.second->getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));

    object.second->draw();
 }
}

void Scene::addObject(std::string name, Object *object)
{
  m_Objs[name] = object;
}
