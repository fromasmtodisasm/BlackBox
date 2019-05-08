#include <BlackBox/Scene.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/ObjectManager.hpp>

#include <tinyxml2.h>
#include <sstream>

using namespace tinyxml2;

#ifndef XMLCheckResult
  #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

void Scene::loadObject(XMLElement *object)
{
  Object *obj;
  const char *objectName = nullptr;
  const char *meshPath = nullptr;
  XMLElement * mesh = nullptr;

  objectName = object->Attribute("name");
  if (objectName == nullptr)
    return;
  mesh = object->FirstChildElement("mesh");
  if (mesh == nullptr)
    return;
  meshPath = mesh->Attribute("name");
  if (meshPath == nullptr)
    return;
  obj = ObjectManager::instance()->getObject(meshPath);
  if (obj == nullptr)
    return;
  obj->setShaderProgram(defaultProgram);
  m_Objs[objectName] = obj;

}

Scene::Scene(std::string name) : name(name)
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

Object *Scene::getObject(std::string name)
{
  return m_Objs[name];
}

void Scene::setCamera(CCamera *camera)
{
  m_Camera = camera;
}

void Scene::update(float dt)
{
  for (auto &obj : m_Objs)
  {
    obj.second->update(dt);
  }
}

bool Scene::save()
{
  std::stringstream sceneName;
  XMLDocument xmlDoc;
  XMLNode * pScene = xmlDoc.NewElement("scene");
  ObjectManager *objectManager = ObjectManager::instance();


  for (auto &obj : m_Objs)
  {
    {
      XMLElement * object = xmlDoc.NewElement("object");
      XMLElement * mesh = xmlDoc.NewElement("mesh");
      //XMLElement * texture = xmlDoc.NewElement("texture");
      std::string objectName = objectManager->getPathByPointer(obj.second);
      object->SetAttribute("name", obj.first.c_str());
      mesh->SetAttribute("name", obj.second->m_path->c_str());
      object->InsertEndChild(mesh);
      pScene->InsertEndChild(object);
      //object->InsertEndChild(mesh);
    }

  }
  xmlDoc.InsertFirstChild(pScene);

  sceneName << "res/scenes/" << name <<  ".xml";
  XMLError eResult = xmlDoc.SaveFile(sceneName.str().c_str());
  XMLCheckResult(eResult);



}

bool Scene::load(std::string name = "default.xml")
{
  XMLDocument xmlDoc;
  ShaderManager *shaderManager = ShaderManager::instance();

  XMLError eResult = xmlDoc.LoadFile(("res/scenes/" + name).c_str());
  XMLCheckResult(eResult);

  XMLNode * pScene = xmlDoc.FirstChild();
  if (pScene == nullptr) return false;

  XMLElement * objects = pScene->FirstChildElement("object");

  if (objects == nullptr) return false;

  while (objects != nullptr)
  {
    loadObject(objects);
    objects = objects->NextSiblingElement("object");
  }

  return true;
}
