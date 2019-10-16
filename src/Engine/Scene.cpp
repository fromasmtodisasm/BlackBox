#include <BlackBox/Scene.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/Camera.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/World.hpp>
#include <BlackBox/Render/Light.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/TextureCube.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Pipeline.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Render/SkyBox.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/Profiler/Profiler.h>

#include <tinyxml2.h>
#include <sstream>
#include <variant>
#include <algorithm>




using namespace tinyxml2;
#define strdup _strdup

#ifndef XMLCheckResult
  #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return false; }
#endif

void Scene::loadTerrain(tinyxml2::XMLElement* terrain)
{
	//Object* obj;
  Material *material;
  MaterialManager *materialManager = MaterialManager::instance();
  const char *terrainName = nullptr;
  const char *materialName = nullptr;
  const char *heightmapName = nullptr;
  XMLElement * heightmap = nullptr;
  XMLElement * materialElement = nullptr;
  Transform transform;


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

	//m_Objects.insert({ "terrain", this->terrain.load(terrainName)});
}

void Scene::loadObject(XMLElement *object)
{
  Object *obj;
  Material *material;
  MaterialManager *materialManager = MaterialManager::instance();
  const char *objectName = nullptr;
  const char *objectType = nullptr;
  bool objectTransparent = false;
  bool objectVisible = false;
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
	if (objectType == std::string("points"))
	{
		m_Points = createPointObject(object);
		return;
	}
	else
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
		obj = ObjectManager::instance()->getObject(meshPath, objectType);
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
  m_Objects.insert(std::pair<std::string, Object*>(objectName, obj));

}

void Scene::loadMesh(XMLElement *mesh)
{

}

void Scene::loadLight(tinyxml2::XMLElement* light)
{
  struct Light {
    union {
      DirectionLight *d;
      PointLight *p;
      SpotLight *s;
    };
    BaseLight::Type type;
 } result;
const char *lightName = nullptr;
  const char *lightType = nullptr;

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

  baseLight->ambient = loadColorAttribute(light->FirstChildElement("ambient"));
  baseLight->diffuse = loadColorAttribute(light->FirstChildElement("diffuse"));
  baseLight->specular = loadColorAttribute(light->FirstChildElement("specular"));
  baseLight->enabled = light->BoolAttribute("enabled");

  switch (baseLight->type)
  {
  case BaseLight::DIRECTIONAL:
    m_DirectionLight[lightName] = result.d;
    break;
  case BaseLight::POINT:
    m_PointLights[lightName] = result.p;
    break;
  case BaseLight::SPOT:
    m_SpotLights[lightName] = result.s;
    break;
  default:
    break;
  }
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

Scene::Scene(std::string name) 
  : 
  lighting(true),
  name(name), 
  m_RenderedScene(-1),
  m_Technique(nullptr),
  m_World(nullptr),
  quadVAO(-1),
  shadowMapMat(nullptr),
  skyBox(nullptr)
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
		"screenshader.vs",
		"screenshader.frag"
	};

	MaterialManager::instance()->loadProgram(pd, false);
	m_ScreenShader = MaterialManager::instance()->getProgram(pd.name);
	
	
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(0,"screenTexture");
	m_ScreenShader->unuse();

	texture_speed = GetISystem()->GetIConsole()->CreateVariable("tex_spd", 0.1f, 0, "Speed of texture animation");
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

std::map<std::string,Object*>::iterator Scene::selectedObject()
{
	if (selected_object_it == m_Objects.end())
		return m_Objects.begin();
	return selected_object_it;
}

bool Scene::selectObject(std::string name)
{
	auto it = m_Objects.find(name);
	if (it == m_Objects.end())
		return false;
	selected_object_it = it;
	return true;
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
  m_RenderedScene = m_Technique->GetFrame();
	
  /*
	if (skyBox != nullptr)
		skyBox->draw(m_Camera);
  */
}

void Scene::addObject(std::string name, Object *object)
{
  m_Objects.insert(std::make_pair(name, object));
}

Object *Scene::getObject(std::string name)
{
  auto objectIt = m_Objects.find(name);
  if (objectIt != m_Objects.end())
  {
    return  objectIt->second;
  }
  return nullptr;
}

size_t Scene::numObjects()
{
	return m_Objects.size();
}

void Scene::setCamera(std::string name, CCamera *camera)
{
  m_Camera[name] = camera;
  m_CurrentCamera = m_Camera.find(name);
}

CCamera* Scene::getCurrentCamera()
{
	return m_CurrentCamera->second;
}

void Scene::update(float dt)
{
  for (auto &obj : m_Objects)
  {
    obj.second->update(dt);
    obj.second->velocity.y -= World::gravity;
  }
}

void Scene::saveObject(tinyxml2::XMLDocument& xmlDoc, ObjectManager* objectManager, std::pair<const std::string, Object*>& obj, tinyxml2::XMLNode* pScene)
{
  XMLElement* object = xmlDoc.NewElement("object");
  XMLElement* mesh = xmlDoc.NewElement("mesh");
  XMLElement* material = xmlDoc.NewElement("material");;
  XMLElement* transform;

  //XMLElement * texture = xmlDoc.NewElement("texture");
  std::string objectName = objectManager->getPathByPointer(obj.second);
  object->SetAttribute("name", obj.first.c_str());
  const char* objType = nullptr;
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
}

bool Scene::save(std::string as)
{
  std::stringstream sceneName;
  tinyxml2::XMLDocument xmlDoc;
  XMLNode * pScene = xmlDoc.NewElement("scene");
  ObjectManager *objectManager = ObjectManager::instance();


  for (auto &obj : m_Objects)
  {
    saveObject(xmlDoc, objectManager, obj, pScene);
  }

  for (auto& light : m_PointLights)
  {
    {
      XMLElement * lightElement = saveLight(xmlDoc, light.second);
      lightElement->SetAttribute("name", light.first.c_str());
      lightElement->SetAttribute("type", light.second->toStr);
      lightElement->SetAttribute("enabled", light.second->enabled);
      pScene->InsertEndChild(lightElement);
    }
  }
  for (auto& light : m_DirectionLight)
  {
    {
      XMLElement * lightElement = saveLight(xmlDoc, light.second);
      lightElement->SetAttribute("name", light.first.c_str());
      lightElement->SetAttribute("type", light.second->toStr);
      lightElement->SetAttribute("enabled", light.second->enabled);
      pScene->InsertEndChild(lightElement);
    }
  }
  for (auto& light : m_SpotLights)
  {
    {
      XMLElement * lightElement = saveLight(xmlDoc, light.second);
      lightElement->SetAttribute("name", light.first.c_str());
      lightElement->SetAttribute("type", light.second->toStr);
      lightElement->SetAttribute("enabled", light.second->enabled);
      pScene->InsertEndChild(lightElement);
    }
  }
  for (auto& camera : m_Camera)
  {
    XMLElement * cameraElement = saveCamera(xmlDoc, camera.second);
    cameraElement->SetAttribute("name", camera.first.c_str());
    pScene->InsertEndChild(cameraElement);
  }
  xmlDoc.InsertFirstChild(pScene);

  if (as == "")
    sceneName << "res/scenes/" << name << ".xml";
  else
    sceneName << "res/scenes/" << as;
  XMLError eResult = xmlDoc.SaveFile(sceneName.str().c_str());
  XMLCheckResult(eResult);


  return true;
}

XMLElement *Scene::saveTransform(tinyxml2::XMLDocument &xmlDoc, Transform *transform)
{
  XMLElement * result = xmlDoc.NewElement("transform");

  XMLElement * position = xmlDoc.NewElement("position");
  XMLElement * rotation = xmlDoc.NewElement("rotation");
  XMLElement * scale = xmlDoc.NewElement("scale");

  //SET POSITION
  {
    XMLElement * X = xmlDoc.NewElement("X");
    XMLElement * Y = xmlDoc.NewElement("Y");
    XMLElement * Z = xmlDoc.NewElement("Z");
    X->SetText(transform->position.x);
    Y->SetText(transform->position.y);
    Z->SetText(transform->position.z);
    position->InsertEndChild(X);
    position->InsertEndChild(Y);
    position->InsertEndChild(Z);
  }
  //SET ROTATION
  {
    XMLElement * X = xmlDoc.NewElement("X");
    XMLElement * Y = xmlDoc.NewElement("Y");
    XMLElement * Z = xmlDoc.NewElement("Z");
    X->SetText(transform->rotation.x);
    Y->SetText(transform->rotation.y);
    Z->SetText(transform->rotation.z);
    rotation->InsertEndChild(X);
    rotation->InsertEndChild(Y);
    rotation->InsertEndChild(Z);
  }
  //SET SCALE
  {
    XMLElement * X = xmlDoc.NewElement("X");
    XMLElement * Y = xmlDoc.NewElement("Y");
    XMLElement * Z = xmlDoc.NewElement("Z");
    X->SetText(transform->scale.x);
    Y->SetText(transform->scale.y);
    Z->SetText(transform->scale.z);
    scale->InsertEndChild(X);
    scale->InsertEndChild(Y);
    scale->InsertEndChild(Z);
  }

  result->InsertEndChild(position);
  result->InsertEndChild(rotation);
  result->InsertEndChild(scale);

  return result;
}

tinyxml2::XMLElement* Scene::saveLight(tinyxml2::XMLDocument& xmlDoc, BaseLight * light)
{
  XMLElement* result = xmlDoc.NewElement("light");
  if (light->BaseLight::type == BaseLight::DIRECTIONAL)
  {
      XMLElement *direction = saveVec3(xmlDoc, reinterpret_cast<DirectionLight*>(light)->direction, "direction");
      result->InsertEndChild(direction);
  }
  else if (light->BaseLight::type == BaseLight::POINT || light->BaseLight::type == BaseLight::SPOT)
  {
    PointLight* _light = reinterpret_cast<PointLight*>(light);
    XMLElement* position = saveVec3(xmlDoc, _light->position, "position");
    XMLElement* constant = saveFloat(xmlDoc, _light->constant, "constant");
    XMLElement* linear = saveFloat(xmlDoc, _light->linear, "linear");
    XMLElement* quadratic = saveFloat(xmlDoc, _light->quadratic, "quadratic");
    if (light->BaseLight::type == BaseLight::SPOT)
    {
      SpotLight* _light = reinterpret_cast<SpotLight*>(light);
      XMLElement *direction = saveVec3(xmlDoc, _light->direction, "direction");

      XMLElement *cutOff = saveFloat(xmlDoc, _light->cutOff, "cutOff");
      XMLElement *outerCutOff = saveFloat(xmlDoc, _light->outerCutOff, "outerCutOff");

      result->InsertEndChild(direction);
      result->InsertEndChild(cutOff);
      result->InsertEndChild(outerCutOff);
    }
    result->InsertEndChild(position);
    result->InsertEndChild(constant);
    result->InsertEndChild(linear);
    result->InsertEndChild(quadratic);
  }
  XMLElement* ambient = saveVec3(xmlDoc, light->ambient, "ambient");
  XMLElement* diffuse = saveVec3(xmlDoc, light->diffuse, "diffuse");
  XMLElement* specular = saveVec3(xmlDoc, light->specular, "specular");

  
  result->InsertEndChild(ambient);
  result->InsertEndChild(diffuse);
  result->InsertEndChild(specular);
  return result;
}

tinyxml2::XMLElement* Scene::saveCamera(tinyxml2::XMLDocument& xmlDoc, CCamera* camera)
{
  XMLElement* result = xmlDoc.NewElement("camera");
  auto transform = saveTransform(xmlDoc, &camera->transform);
  auto speed = saveFloat(xmlDoc, camera->MovementSpeed->GetFVal(), "speed");
  result->InsertEndChild(transform);
  result->InsertEndChild(speed);

  return result;
}

tinyxml2::XMLElement* Scene::saveVec3(tinyxml2::XMLDocument& xmlDoc, glm::vec3& color, const char* name)
{
  XMLElement* result = xmlDoc.NewElement(name);
  result->SetAttribute("x", color.r);
  result->SetAttribute("y", color.g);
  result->SetAttribute("z", color.b);
  return result;
}

tinyxml2::XMLElement* Scene::saveFloat(tinyxml2::XMLDocument& xmlDoc, float value, const char* name)
{
  XMLElement* result = xmlDoc.NewElement(name);
  result->SetText(value);
  return result;
}

XMLElement *Scene::saveMaterial(tinyxml2::XMLDocument &xmlDoc, Object *object)
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
  result.position = loadVec3(*transform, "position");
  //GET ROTATION
  result.rotation = loadVec3(*transform, "rotation");
  //GET SCALE
  result.scale = loadVec3(*transform, "scale");

  return result;
}

glm::vec3 Scene::loadVec3(tinyxml2::XMLElement& element, const char* name)
{
  glm::vec3 result;

  XMLElement * vector = element.FirstChildElement(name);
  XMLElement * X = vector->FirstChildElement("X");
  XMLElement * Y = vector->FirstChildElement("Y");
  XMLElement * Z = vector->FirstChildElement("Z");
  result.x = X->FloatText();
  result.y = Y->FloatText();
  result.z = Z->FloatText();

  return result;
}

void Scene::loadCamera(tinyxml2::XMLElement* element)
{
  CCamera *result;
  const char* name = element->Attribute("name");
  if (name == nullptr)
  {
    //log it
    return;
  }
  XMLElement * transform = element->FirstChildElement("transform");

  //GET POSITION
  auto position = loadVec3(*transform, "position");
  //GET ROTATION
  auto rotation = loadVec3(*transform, "rotation");

  auto speed = element->FirstChildElement("speed");
  float cam_speed = 0.5;

  if (speed != nullptr)
    cam_speed = speed->FloatText();

  result = new CCamera(position, glm::vec3(0.f, 1.f, 0.f), rotation.y, rotation.x);

  result->MovementSpeed = GetISystem()->GetIConsole()->CreateVariable("cam_speed", cam_speed, 0, "Camera speed");
  
  m_Camera[name] = result;
}

void Scene::loadTagPoint(tinyxml2::XMLElement* element)
{
}

bool Scene::load(std::string name = "default.xml")
{
  tinyxml2::XMLDocument xmlDoc;

  XMLError eResult = xmlDoc.LoadFile(name.c_str());
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

  XMLElement * terrain = pScene->FirstChildElement("terrain");
	if (terrain != nullptr)
	{
		loadTerrain(terrain);
	}

  XMLElement * lights = pScene->FirstChildElement("light");
  while (lights != nullptr)
  {
    loadLight(lights);
    lights = lights->NextSiblingElement("light");
  }
	selected_object_it = m_Objects.begin();

	auto sbm = MaterialManager::instance()->getMaterial("skybox");
	if (sbm != nullptr)
	{
		skyBox = new SkyBox(reinterpret_cast<TextureCube*>(sbm->diffuse[0]));
		Pipeline::instance()->skyBox = reinterpret_cast<TextureCube*>(sbm->diffuse[0]);
	}

  //XMLElement* technique = pScene->FirstChildElement("technique");
#ifdef SAVE_LOAD_TAGPOINTS
  XMLElement * tag_points = pScene->FirstChildElement("tag_points");
  if (tag_points == nullptr) return false;
  while (tag_points != nullptr)
  {
    loadTagPoint(tag_points);
    tag_points = tag_points->NextSiblingElement("tagpoint");
  }

  //m_CurrentCamera = m_Camera.find("main");
  //assert(m_CurrentCamera != m_Camera.end());

#endif // SAVE_LOAD_TAGPOINTS

  XMLElement * cameras = pScene->FirstChildElement("camera");
  if (cameras == nullptr) return false;
  while (cameras != nullptr)
  {
    loadCamera(cameras);
    cameras = cameras->NextSiblingElement("camera");
  }

  m_CurrentCamera = m_Camera.find("main");
  assert(m_CurrentCamera != m_Camera.end());


  return true;
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

SkyBox* Scene::GetSkyBox()
{
  return skyBox;
}

void Scene::setPostProcessor(IPostProcessor* postProcessor)
{
	this->postProcessor = postProcessor;
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

void Scene::setTechnique(ITechnique* technique)
{
  m_Technique = technique;
}

const PointLightList &Scene::GetPointLights()
{
  return m_PointLights;
}

Terrain *Scene::getTerrain()
{
	return &terrain;
}

PointObject* Scene::createPointObject(XMLElement* object)
{
	struct Point
	{
		glm::vec3 pos;
	};

	auto positions = object->FirstChildElement("positions");
	if (positions)
	{
		auto filename = positions->Attribute("file");
		if (filename)
		{
			FILE* file = fopen(filename, "r");
			if (file == nullptr) return nullptr;
			std::vector<Point> points;
			char line[128];
			while (!feof(file))
			{
				Point point;
				auto m = fscanf(file, "v\t%f %f %f\n", &point.pos.x, &point.pos.y, &point.pos.z);
				if (m != 3)
				{
					fgets(line, 128, file);
					continue;
				}
				
				points.push_back(point);
			}
			if (points.size() > 0)
			{
				PointObject* po = new PointObject();
				GLuint VBO;

				po->point_cnt = points.size();
				glGenVertexArrays(1, &po->VAO);
				glBindVertexArray(po->VAO);
					glGenBuffers(1, &VBO);
					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Point), points.data(), GL_STATIC_DRAW);

					glEnableVertexAttribArray(0);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

				glBindVertexArray(0);

				debuger::vertex_array_label(po->VAO, "point objects");
				debuger::buffer_label(VBO, "point objects");
				return po;
			}
		}
	}
	return nullptr;
}

PointObject* Scene::getPoints()
{
	return m_Points;
}

PointObject::PointObject()
{
	ProgramDesc pd = {
		"points",
		"points.vert",
		"points.frag"
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
	glDrawArrays(GL_POINTS, 0, point_cnt);
	glBindVertexArray(0);
}
