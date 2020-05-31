#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/3DEngine/StatObject.hpp>
//#include <BlackBox/Renderer/SkyBox.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/ObjectManager.hpp>
#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/Scene/XmlSerialization.hpp>

#include <sstream>
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return false; }
#endif

using namespace tinyxml2;

Scene::Serializator::Serializator(Scene* scene)
  :
  m_Scene(scene)
{
}

bool Scene::Serializator::save(const char* as)
{
	#if 0
  std::stringstream sceneName;
  tinyxml2::XMLDocument xmlDoc;
  XMLNode* pScene = xmlDoc.NewElement("scene");
  ObjectManager* objectManager = ObjectManager::instance();

  for (auto& obj : m_Scene->m_Objects)
  {
    saveObject(xmlDoc, objectManager, obj, pScene);
  }

  saveLights(xmlDoc, pScene);

  for (auto& camera : m_Scene->m_Camera)
  {
    XMLElement* cameraElement = saveCamera(xmlDoc, camera.second);
    cameraElement->SetAttribute("name", camera.first.c_str());
    pScene->InsertEndChild(cameraElement);
  }
  xmlDoc.InsertFirstChild(pScene);

#if 0
  if (as == "")
    sceneName << "res/scenes/" << name << ".xml";
  else
#endif
    sceneName << "res/scenes/" << as;
  XMLError eResult = xmlDoc.SaveFile(sceneName.str().c_str());
  XMLCheckResult(eResult);
#endif
  return false;
}

bool Scene::Serializator::load(const char* name, LoadObjectSink* callback)
{
	#if 0
  tinyxml2::XMLDocument xmlDoc;
  XMLError eResult = xmlDoc.LoadFile(name);
  XMLCheckResult(eResult);

  XMLNode* pScene = xmlDoc.FirstChild();
  if (pScene == nullptr) return false;

  XMLElement* objects = pScene->FirstChildElement("object");

  if (objects == nullptr) return false;
  while (objects != nullptr)
  {
    loadObject(objects, callback);
    objects = objects->NextSiblingElement("object");
  }

  XMLElement* terrain = pScene->FirstChildElement("terrain");
  if (terrain != nullptr)
  {
    loadTerrain(terrain);
  }

  XMLElement* lights = pScene->FirstChildElement("light");
  while (lights != nullptr)
  {
    loadLight(lights);
    lights = lights->NextSiblingElement("light");
  }
  m_Scene->selected_object_it = m_Scene->m_Objects.begin();

  auto sbm = MaterialManager::instance()->getMaterial("skybox");
  if (sbm != nullptr)
  {
    //m_Scene->skyBox = new SkyBox(reinterpret_cast<TextureCube*>(sbm->diffuse[0]));
    //Pipeline::instance()->skyBox = reinterpret_cast<TextureCube*>(sbm->diffuse[0]);
  }

  //XMLElement* technique = pScene->FirstChildElement("technique");
#ifdef SAVE_LOAD_TAGPOINTS
  XMLElement* tag_points = pScene->FirstChildElement("tag_points");
  if (tag_points == nullptr) return false;
  while (tag_points != nullptr)
  {
    loadTagPoint(tag_points);
    tag_points = tag_points->NextSiblingElement("tagpoint");
  }

  //m_CurrentCamera = m_Camera.find("main");
  //assert(m_CurrentCamera != m_Camera.end());

#endif // SAVE_LOAD_TAGPOINTS

  XMLElement* cameras = pScene->FirstChildElement("camera");
  if (cameras == nullptr) return false;
  while (cameras != nullptr)
  {
    loadCamera(cameras);
    cameras = cameras->NextSiblingElement("camera");
  }

  m_Scene->m_CurrentCamera = m_Scene->m_Camera.find("main");
  assert(m_Scene->m_CurrentCamera != m_Scene->m_Camera.end());
#endif
  return true;
}

bool Scene::Serializator::loadDefault()
{
	return false;
}

void Scene::Serializator::loadTerrain(tinyxml2::XMLElement* terrain)
{
	#if 0
  //Object* obj;
  Material* material;
  MaterialManager* materialManager = MaterialManager::instance();
  const char* terrainName = nullptr;
  const char* materialName = nullptr;
  const char* heightmapName = nullptr;
  XMLElement* heightmap = nullptr;
  XMLElement* materialElement = nullptr;

  terrainName = terrain->Attribute("name");
  if (terrainName == nullptr)
    return;
  heightmap = terrain->FirstChildElement("heightmap");
  if (heightmap == nullptr)
    return;
  heightmapName = heightmap->Attribute("name");
  if (heightmapName == nullptr)
    return;

  materialElement = terrain->FirstChildElement("material");
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
	#endif
}

void Scene::Serializator::loadObject(XMLElement* object, LoadObjectSink* callback)
{
	#if 0
  CStatObj* obj;
  Material* material;
  MaterialManager* materialManager = MaterialManager::instance();
  const char* objectName = nullptr;
  const char* objectType = nullptr;
  bool objectTransparent = false;
  bool objectVisible = false;
  const char* meshPath = nullptr;
  const char* materialName = nullptr;
  XMLElement* mesh = nullptr;
  XMLElement* materialElement = nullptr;
  Transform transform;

  objectName = object->Attribute("name");
  if (objectName == nullptr)
    return;
  objectType = object->Attribute("type");
  if (objectType == nullptr)
    objectType = "object";
  {
    if (object->BoolAttribute("transparent"))
      objectTransparent = true;
    if (object->BoolAttribute("visible"))
      objectVisible = true;
    mesh = object->FirstChildElement("mesh");
    if (mesh == nullptr)
      return;
    meshPath = mesh->Attribute("name");
    if (meshPath == nullptr)
      return;
    //obj = ObjectManager::instance()->getObject(meshPath, objectType, callback);
    obj = static_cast<CStatObj*>(gEnv->p3DEngine->MakeObject(meshPath));
  }
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

  auto uv = object->FirstChildElement("uv");
  if (uv != nullptr)
  {
    auto scaleX = uv->FloatAttribute("scaleX", 1.0);
    auto scaleY = uv->FloatAttribute("scaleY", 1.0);
    obj->uvMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaleX, scaleY, 1.0f));
  }
  else
  {
    obj->uvMatrix = glm::mat4(1.0f);
  }

  transform = loadTransform(*object);
  obj->m_transform = transform;
  obj->m_transparent = objectTransparent;
  obj->m_visible = objectVisible;
  obj->setMaterial(material);
  m_Scene->m_Objects.insert(std::pair<std::string, CStatObj*>(objectName, obj));
	#endif
}

void Scene::Serializator::loadMesh(XMLElement* mesh)
{
}

void Scene::Serializator::loadLight(tinyxml2::XMLElement* light)
{
  struct Light {
    union {
      DirectionLight* d;
      PointLight* p;
      SpotLight* s;
    };
    BaseLight::Type type;
  } result;
  const char* lightName = nullptr;
  const char* lightType = nullptr;

#if 0
  XMLElement* ambient = nullptr;
  XMLElement* diffuse = nullptr;
  XMLElement* specular = nullptr;
#endif

  lightName = light->Attribute("name");
  if (lightName == nullptr)
    //TODO: hanlde this
    return;
  lightType = light->Attribute("type");
  if (lightType == nullptr)
    //TODO: hanlde this
    return;
  /*
  position = light->FirstChildElement("position");
  if (position == nullptr)
    //TODO: hanlde this
    return;
  */

  BaseLight* baseLight;
  {
    std::string tmp(lightType);
    if (tmp == "direction")
    {
      result.d = new DirectionLight;
      baseLight = result.d;
      baseLight->type = BaseLight::DIRECTIONAL;
      XMLElement* direction = light->FirstChildElement("direction");
      if (!direction)
        return;

      result.d->direction.x = direction->FloatAttribute("x");
      result.d->direction.y = direction->FloatAttribute("y");
      result.d->direction.z = direction->FloatAttribute("z");
    }
    else if (tmp == "point")
    {
      result.p = new PointLight;
      baseLight = result.p;
      baseLight->type = BaseLight::POINT;
      XMLElement* position = light->FirstChildElement("position");

      result.s->constant = light->FirstChildElement("constant")->FloatText();
      result.s->linear = light->FirstChildElement("linear")->FloatText();
      result.s->quadratic = light->FirstChildElement("quadratic")->FloatText();

      result.p->position.x = position->FloatAttribute("x");
      result.p->position.y = position->FloatAttribute("y");
      result.p->position.z = position->FloatAttribute("z");
    }
    else if (tmp == "spot")
    {
      result.s = new SpotLight;
      baseLight = result.s;
      baseLight->type = BaseLight::SPOT;
      XMLElement* direction = light->FirstChildElement("direction");
      XMLElement* position = light->FirstChildElement("position");

      result.s->constant = light->FirstChildElement("constant")->FloatText();
      result.s->linear = light->FirstChildElement("linear")->FloatText();
      result.s->quadratic = light->FirstChildElement("quadratic")->FloatText();
      result.s->cutOff = light->FirstChildElement("cutOff")->FloatText();
      result.s->outerCutOff = light->FirstChildElement("outerCutOff")->FloatText();

      result.p->position.x = position->FloatAttribute("x");
      result.p->position.y = position->FloatAttribute("y");
      result.p->position.z = position->FloatAttribute("z");

      if (!direction)
        return;

      result.d->direction.x = direction->FloatAttribute("x");
      result.d->direction.y = direction->FloatAttribute("y");
      result.d->direction.z = direction->FloatAttribute("z");
    }
    else
    {
      return;
    }
    baseLight->toStr = strdup(tmp.c_str());
  }

  baseLight->ambient = serialization::loadColorAttribute(light->FirstChildElement("ambient"));
  baseLight->diffuse = serialization::loadColorAttribute(light->FirstChildElement("diffuse"));
  baseLight->specular = serialization::loadColorAttribute(light->FirstChildElement("specular"));
  baseLight->enabled = light->BoolAttribute("enabled");

  switch (baseLight->type)
  {
  case BaseLight::DIRECTIONAL:
    m_Scene->m_DirectionLight[lightName] = result.d;
    break;
  case BaseLight::POINT:
    m_Scene->m_PointLights[lightName] = result.p;
    break;
  case BaseLight::SPOT:
    m_Scene->m_SpotLights[lightName] = result.s;
    break;
  default:
    break;
  }
}

void Scene::Serializator::saveObject(tinyxml2::XMLDocument& xmlDoc, ObjectManager* objectManager, std::pair<const std::string, CStatObj*>& obj, tinyxml2::XMLNode* pScene)
{
	#if 0
  XMLElement* object = xmlDoc.NewElement("object");
  XMLElement* mesh = xmlDoc.NewElement("mesh");
  XMLElement* material = xmlDoc.NewElement("material");;
  XMLElement* transform;

  //XMLElement * texture = xmlDoc.NewElement("texture");
  std::string objectName = objectManager->getPathByPointer(obj.second);
  object->SetAttribute("name", obj.first.c_str());
  object->SetAttribute("type", obj.second->type.c_str());
  object->SetAttribute("visible", obj.second->visible());
  object->SetAttribute("transparent", obj.second->m_transparent);
  mesh->SetAttribute("name", obj.second->m_path.c_str());
  material->SetAttribute("name", obj.second->m_Material->name->c_str());
  //transform->SetAttribute("name", obj.second->m_path->c_str());
  //position->SetText(1.23);

  transform = saveTransform(xmlDoc, &obj.second->m_transform);
  object->InsertEndChild(mesh);
  object->InsertEndChild(transform);
  object->InsertEndChild(material);
  pScene->InsertEndChild(object);
  //object->InsertEndChild(mesh);
	#endif
}

XMLElement* Scene::Serializator::saveTransform(tinyxml2::XMLDocument& xmlDoc, Transform* transform)
{
  XMLElement* result = xmlDoc.NewElement("transform");

  result->InsertEndChild(serialization::writeElement(xmlDoc, transform->position, "position"));
  result->InsertEndChild(serialization::writeElement(xmlDoc, transform->position, "rotation"));
  result->InsertEndChild(serialization::writeElement(xmlDoc, transform->position, "scale"));

  return result;
}

tinyxml2::XMLElement* Scene::Serializator::saveLight(tinyxml2::XMLDocument& xmlDoc, BaseLight* light)
{
  XMLElement* result = xmlDoc.NewElement("light");
  if (light->BaseLight::type == BaseLight::DIRECTIONAL)
  {
    XMLElement* direction = serialization::writeAttribute(xmlDoc, reinterpret_cast<DirectionLight*>(light)->direction, "direction");
    result->InsertEndChild(direction);
  }
  else if (light->BaseLight::type == BaseLight::POINT || light->BaseLight::type == BaseLight::SPOT)
  {
    PointLight* _light = reinterpret_cast<PointLight*>(light);
    XMLElement* position = serialization::writeAttribute(xmlDoc, _light->position, "position");
    XMLElement* constant = serialization::writeElement(xmlDoc, _light->constant, "constant");
    XMLElement* linear = serialization::writeElement(xmlDoc, _light->linear, "linear");
    XMLElement* quadratic = serialization::writeElement(xmlDoc, _light->quadratic, "quadratic");
    if (light->BaseLight::type == BaseLight::SPOT)
    {
      SpotLight* _light = reinterpret_cast<SpotLight*>(light);
      XMLElement* direction = serialization::writeAttribute(xmlDoc, _light->direction, "direction");

      XMLElement* cutOff = serialization::writeElement(xmlDoc, _light->cutOff, "cutOff");
      XMLElement* outerCutOff = serialization::writeElement(xmlDoc, _light->outerCutOff, "outerCutOff");

      result->InsertEndChild(direction);
      result->InsertEndChild(cutOff);
      result->InsertEndChild(outerCutOff);
    }
    result->InsertEndChild(position);
    result->InsertEndChild(constant);
    result->InsertEndChild(linear);
    result->InsertEndChild(quadratic);
  }
  XMLElement* ambient = serialization::writeAttribute(xmlDoc, light->ambient, "ambient");
  XMLElement* diffuse = serialization::writeAttribute(xmlDoc, light->diffuse, "diffuse");
  XMLElement* specular = serialization::writeAttribute(xmlDoc, light->specular, "specular");

  result->InsertEndChild(ambient);
  result->InsertEndChild(diffuse);
  result->InsertEndChild(specular);
  return result;
}

void Scene::Serializator::saveLights(XMLDocument& xmlDoc, XMLNode* pScene)
{
  for (auto& light : m_Scene->m_PointLights)
  {
    {
      XMLElement* lightElement = saveLight(xmlDoc, light.second);
      lightElement->SetAttribute("name", light.first.c_str());
      lightElement->SetAttribute("type", light.second->toStr);
      lightElement->SetAttribute("enabled", light.second->enabled);
      pScene->InsertEndChild(lightElement);
    }
  }
  for (auto& light : m_Scene->m_DirectionLight)
  {
    {
      XMLElement* lightElement = saveLight(xmlDoc, light.second);
      lightElement->SetAttribute("name", light.first.c_str());
      lightElement->SetAttribute("type", light.second->toStr);
      lightElement->SetAttribute("enabled", light.second->enabled);
      pScene->InsertEndChild(lightElement);
    }
  }
  for (auto& light : m_Scene->m_SpotLights)
  {
    {
      XMLElement* lightElement = saveLight(xmlDoc, light.second);
      lightElement->SetAttribute("name", light.first.c_str());
      lightElement->SetAttribute("type", light.second->toStr);
      lightElement->SetAttribute("enabled", light.second->enabled);
      pScene->InsertEndChild(lightElement);
    }
  }
}

tinyxml2::XMLElement* Scene::Serializator::saveCamera(tinyxml2::XMLDocument& xmlDoc, CCamera* camera)
{
  XMLElement* result = xmlDoc.NewElement("camera");
  auto transform = saveTransform(xmlDoc, &camera->transform);
  auto speed = serialization::writeElement(xmlDoc, camera->MovementSpeed, "speed");
  result->InsertEndChild(transform);
  result->InsertEndChild(speed);

  return result;
}

XMLElement* Scene::Serializator::saveMaterial(tinyxml2::XMLDocument& xmlDoc, CStatObj* object)
{
  XMLElement* material = xmlDoc.NewElement("material");

  //material->InsertEndChild(MaterialManager::instance()->saveShader(xmlDoc, object->m_Material->program->))
  return material;
}

Transform Scene::Serializator::loadTransform(XMLElement& object)
{
  Transform result;
  XMLElement* transform = object.FirstChildElement("transform");

  //GET POSITION
  result.position = serialization::loadElement(*transform, "position");
  //GET ROTATION
  result.rotation = serialization::loadElement(*transform, "rotation");
  //GET SCALE
  result.scale = serialization::loadElement(*transform, "scale");

  return result;
}

void Scene::Serializator::loadCamera(tinyxml2::XMLElement* element)
{
  CCamera* result;
  const char* name = element->Attribute("name");
  if (name == nullptr)
  {
    //log it
    return;
  }
  XMLElement* transform = element->FirstChildElement("transform");

  //GET POSITION
  auto position = serialization::loadElement(*transform, "position");
  //GET ROTATION
  auto rotation = serialization::loadElement(*transform, "rotation");

  auto speed = element->FirstChildElement("speed");
  float cam_speed = 0.5;

  if (speed != nullptr)
    cam_speed = speed->FloatText();

  result = new CCamera(position, glm::vec3(0.f, 1.f, 0.f), rotation.y, rotation.x);

  REGISTER_CVAR2("cam_speed", &result->MovementSpeed, cam_speed, 0, "Speed of camera");

  m_Scene->m_Camera[name] = result;
}

void Scene::Serializator::loadTagPoint(tinyxml2::XMLElement* element)
{
}
