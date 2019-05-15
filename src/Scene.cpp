#include <BlackBox/Scene.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/ObjectManager.hpp>
#include <BlackBox/MaterialManager.hpp>
#include <BlackBox/World.hpp>
#include <BlackBox/Light.hpp>

#include <tinyxml2.h>
#include <sstream>


using namespace tinyxml2;

#ifndef XMLCheckResult
  #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

void Scene::loadObject(XMLElement *object)
{
  Object *obj;
  Material *material;
  MaterialManager *materialManager = MaterialManager::instance();
  const char *objectName = nullptr;
  const char *objectType = nullptr;
  const char *meshPath = nullptr;
  const char *materialName = nullptr;
  XMLElement * mesh = nullptr;
  XMLElement * materialElement = nullptr;
  Transform transform;

  objectName = object->Attribute("name");
  if (objectName == nullptr)
    return;
  objectType = object->Attribute("type");
  if (objectType == nullptr)
    objectType = "object";
  mesh = object->FirstChildElement("mesh");
  if (mesh == nullptr)
    return;
  meshPath = mesh->Attribute("name");
  if (meshPath == nullptr)
    return;
  obj = ObjectManager::instance()->getObject(meshPath, objectType);
  if (obj == nullptr)
    return;
  materialElement = object->FirstChildElement("material");
  if (materialElement == nullptr)
    material = defaultMaterial;
  else {
    materialName = materialElement->Attribute("name");
    if (materialName == nullptr)
      material = defaultMaterial;
    else {
      material = materialManager->getMaterial(materialName);
      if (material == nullptr)
        material = defaultMaterial;
    }

  }

  transform = loadTransform(*object);
  obj->m_transform = transform;
  obj->setShaderProgram(defaultProgram);
  obj->setMaterial(material);
  m_Objects[objectName] = obj;

}

void Scene::loadMesh(XMLElement *mesh)
{

}

void Scene::loadLight(tinyxml2::XMLElement* light)
{
  Light *result;
  const char *lightName = nullptr;
  const char *lightType = nullptr;

  XMLElement * position = nullptr;
  XMLElement * ambient = nullptr;
  XMLElement * diffuse = nullptr;
  XMLElement * specular = nullptr;

  lightName = light->Attribute("name");
  if (lightName == nullptr)
    //TODO: hanlde this
    return;
  lightType = light->Attribute("type");
  if (lightType == nullptr)
    //TODO: hanlde this
    return;
  position = light->FirstChildElement("position");
  if (position == nullptr)
    //TODO: hanlde this
    return;

  result = new Light();
  result->type = lightType;
  result->position.x = position->FirstChildElement("X")->FloatText();
  result->position.y = position->FirstChildElement("Y")->FloatText();
  result->position.z = position->FirstChildElement("Z")->FloatText();

  result->ambient = loadColorAttribute(light->FirstChildElement("ambient"));
  result->diffuse = loadColorAttribute(light->FirstChildElement("diffuse"));
  result->specular = loadColorAttribute(light->FirstChildElement("specular"));
  m_Lights[lightName] = result;
}

glm::vec3 Scene::loadColorAttribute(tinyxml2::XMLElement* element)
{
  glm::vec3 color;
  if (element == nullptr)
  //TODO: handle this
    return glm::vec3();
  color.r = element->FloatAttribute("r");
  color.g = element->FloatAttribute("g");
  color.b = element->FloatAttribute("b");
  return color;
}

Scene::Scene(std::string name) : name(name)
{

}

void Scene::draw(float dt)
{
  auto sun = m_Lights["sun"];
 for (const auto &object : m_Objects) {
    //object.second->rotate(dt*0.01f, {0,1,0});
    //object.second->getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));
    CShaderProgram *program = object.second->m_Material->program;
    program->use();
    program->setUniformValue("light.position", sun->position);
    program->setUniformValue("light.ambient", sun->ambient);
    program->setUniformValue("light.diffuse", sun->diffuse);
    program->setUniformValue("light.specular", sun->specular);
    program->setUniformValue("light.shin", sun->specular);

    object.second->draw(m_Camera);
  }
  Object *lightObject = m_Objects["light"];
  CShaderProgram *program = lightObject->m_Material->program;
  for (const auto& light : m_Lights) {
    program->use();
    lightObject->moveTo(light.second->position);
    lightObject->draw(m_Camera);
  }
 
}

void Scene::addObject(std::string name, Object *object)
{
  //if (m_Objs.find(name) != m_Objs.end())

  m_Objects[name] = object;
}

Object *Scene::getObject(std::string name)
{
  return m_Objects[name];
}

void Scene::setCamera(CCamera *camera)
{
  m_Camera = camera;
}

void Scene::update(float dt)
{
  for (auto &obj : m_Objects)
  {
    obj.second->update(dt);
    obj.second->velocity.y -= World::gravity;
  }
}

bool Scene::save()
{
  std::stringstream sceneName;
  XMLDocument xmlDoc;
  XMLNode * pScene = xmlDoc.NewElement("scene");
  ObjectManager *objectManager = ObjectManager::instance();


  for (auto &obj : m_Objects)
  {
    {
      XMLElement * object = xmlDoc.NewElement("object");
      XMLElement * mesh = xmlDoc.NewElement("mesh");
      XMLElement * material = xmlDoc.NewElement("material");;
      XMLElement * transform;

      //XMLElement * texture = xmlDoc.NewElement("texture");
      std::string objectName = objectManager->getPathByPointer(obj.second);
      object->SetAttribute("name", obj.first.c_str());
      const char* objType = nullptr;
      object->SetAttribute("type", obj.second->type.c_str());
      mesh->SetAttribute("name", obj.second->m_Mesh->m_Path->c_str());
      material->SetAttribute("name", obj.second->m_Material->name->c_str());
      //transform->SetAttribute("name", obj.second->m_path->c_str());
      //position->SetText(1.23);


      transform = saveTransform(xmlDoc, obj.second);
      transform = saveTransform(xmlDoc, obj.second);
      object->InsertEndChild(mesh);
      object->InsertEndChild(transform);
      object->InsertEndChild(material);
      pScene->InsertEndChild(object);
      //object->InsertEndChild(mesh);
    }
  }

  for (auto& light : m_Lights)
  {
    {
      XMLElement * lightElement = saveLight(xmlDoc, light.second);

      //XMLElement * texture = xmlDoc.NewElement("texture");
      lightElement->SetAttribute("name", light.first.c_str());
      lightElement->SetAttribute("type", light.second->type);


      pScene->InsertEndChild(lightElement);
      //object->InsertEndChild(mesh);
    }
  }
  xmlDoc.InsertFirstChild(pScene);

  sceneName << "res/scenes/" << name <<  ".xml";
  XMLError eResult = xmlDoc.SaveFile(sceneName.str().c_str());
  XMLCheckResult(eResult);


  return true;
}

XMLElement *Scene::saveTransform(XMLDocument &xmlDoc, Object *object)
{
  XMLElement * transform = xmlDoc.NewElement("transform");

  XMLElement * position = xmlDoc.NewElement("position");
  XMLElement * rotation = xmlDoc.NewElement("rotation");
  XMLElement * scale = xmlDoc.NewElement("scale");

  //SET POSITION
  {
    XMLElement * X = xmlDoc.NewElement("X");
    XMLElement * Y = xmlDoc.NewElement("Y");
    XMLElement * Z = xmlDoc.NewElement("Z");
    X->SetText(object->m_transform.position.x);
    Y->SetText(object->m_transform.position.y);
    Z->SetText(object->m_transform.position.z);
    position->InsertEndChild(X);
    position->InsertEndChild(Y);
    position->InsertEndChild(Z);
  }
  //SET ROTATION
  {
    XMLElement * X = xmlDoc.NewElement("X");
    XMLElement * Y = xmlDoc.NewElement("Y");
    XMLElement * Z = xmlDoc.NewElement("Z");
    X->SetText(object->m_transform.rotation.x);
    Y->SetText(object->m_transform.rotation.y);
    Z->SetText(object->m_transform.rotation.z);
    rotation->InsertEndChild(X);
    rotation->InsertEndChild(Y);
    rotation->InsertEndChild(Z);
  }
  //SET SCALE
  {
    XMLElement * X = xmlDoc.NewElement("X");
    XMLElement * Y = xmlDoc.NewElement("Y");
    XMLElement * Z = xmlDoc.NewElement("Z");
    X->SetText(object->m_transform.scale.x);
    Y->SetText(object->m_transform.scale.y);
    Z->SetText(object->m_transform.scale.z);
    scale->InsertEndChild(X);
    scale->InsertEndChild(Y);
    scale->InsertEndChild(Z);
  }

  transform->InsertEndChild(position);
  transform->InsertEndChild(rotation);
  transform->InsertEndChild(scale);

  return transform;
}

tinyxml2::XMLElement* Scene::saveLight(tinyxml2::XMLDocument& xmlDoc, Light* light)
{
  XMLElement* result = xmlDoc.NewElement("light");
  XMLElement *position = xmlDoc.NewElement("position");
  {
    XMLElement* X = xmlDoc.NewElement("X");
    XMLElement* Y = xmlDoc.NewElement("Y");
    XMLElement* Z = xmlDoc.NewElement("Z");
    X->SetText(light->position.x);
    Y->SetText(light->position.y);
    Z->SetText(light->position.z);
    position->InsertEndChild(X);
    position->InsertEndChild(Y);
    position->InsertEndChild(Z);
  }
  XMLElement* ambient = saveColorAttribute(xmlDoc, light->diffuse, "ambient");
  XMLElement* diffuse = saveColorAttribute(xmlDoc, light->diffuse, "diffuse");
  XMLElement* specular = saveColorAttribute(xmlDoc, light->diffuse, "specular");

  result->InsertEndChild(position);
  result->InsertEndChild(ambient);
  result->InsertEndChild(diffuse);
  result->InsertEndChild(specular);
  return result;
}

tinyxml2::XMLElement* Scene::saveColorAttribute(tinyxml2::XMLDocument& xmlDoc, glm::vec3& color, const char* name)
{
  XMLElement* result = xmlDoc.NewElement(name);
  result->SetAttribute("r", color.r);
  result->SetAttribute("g", color.g);
  result->SetAttribute("b", color.b);
  return result;
}

XMLElement *Scene::saveMaterial(XMLDocument &xmlDoc, Object *object)
{
  XMLElement * material = xmlDoc.NewElement("material");

  //material->InsertEndChild(MaterialManager::instance()->saveShader(xmlDoc, object->m_Material->program->))
  return material;
}

Transform Scene::loadTransform(XMLElement &object)
{
  Transform result;
  XMLElement * transform = object.FirstChildElement("transform");

  //GET POSITION
  {
    XMLElement * position = transform->FirstChildElement("position");
    XMLElement * X = position->FirstChildElement("X");
    XMLElement * Y = position->FirstChildElement("Y");
    XMLElement * Z = position->FirstChildElement("Z");
    result.position.x = X->FloatText();
    result.position.y = Y->FloatText();
    result.position.z = Z->FloatText();
  }
  //GET ROTATION
  {
    XMLElement * rotation = transform->FirstChildElement("rotation");
    XMLElement * X = rotation->FirstChildElement("X");
    XMLElement * Y = rotation->FirstChildElement("Y");
    XMLElement * Z = rotation->FirstChildElement("Z");
    result.rotation.x = X->FloatText();
    result.rotation.y = Y->FloatText();
    result.rotation.z = Z->FloatText();

  }
  //GET SCALE
  {
    XMLElement * scale = transform->FirstChildElement("scale");
    XMLElement * X = scale->FirstChildElement("X");
    XMLElement * Y = scale->FirstChildElement("Y");
    XMLElement * Z = scale->FirstChildElement("Z");
    result.scale.x = X->FloatText();
    result.scale.y = Y->FloatText();
    result.scale.z = Z->FloatText();

  }

  return result;

}

bool Scene::load(std::string name = "default.xml")
{
  XMLDocument xmlDoc;

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

  XMLElement * lights = pScene->FirstChildElement("light");
  while (lights != nullptr)
  {
    loadLight(lights);
    lights = lights->NextSiblingElement("light");
  }

  return true;
}
