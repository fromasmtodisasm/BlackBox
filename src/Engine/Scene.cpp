#include <BlackBox/Scene.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/CCamera.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/World.hpp>
#include <BlackBox/Render/Light.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/TextureCube.hpp>

#include <tinyxml2.h>
#include <sstream>
#include <variant>


using namespace tinyxml2;

#ifndef XMLCheckResult
  #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return false; }
#endif

class SkyBox : public IDrawable
{
	TextureCube* texture;
	VertexArrayObject* vao;
};

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
	if (object->Attribute("transparent"))
		objectTransparent = true;
	if (object->Attribute("visible"))
		objectVisible = true;
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
	obj->m_transparent = objectTransparent;
	obj->m_visible = objectVisible;
  obj->setShaderProgram(defaultProgram);
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
  color.r = element->FloatAttribute("x");
  color.g = element->FloatAttribute("y");
  color.b = element->FloatAttribute("z");
  return color;
}

void Scene::setupLights(Object* object)
{
  CShaderProgram* program = object->m_Material->program;
  int currentLight = 0;
  int nr_point_lights = 0;
  auto sun = m_DirectionLight.find("sun");
  if (sun != m_DirectionLight.end())
  {
    program->setUniformValue(sun->second->direction, "dirLight.direction");
    program->setUniformValue(sun->second->ambient, "dirLight.ambient");
    program->setUniformValue(sun->second->diffuse, "dirLight.diffuse");
    program->setUniformValue(sun->second->specular, "dirLight.specular");
  }
  // point lights
  for (const auto& light : m_PointLights)
  {
    if (light.second->enabled)
    {
      program->setUniformValue(light.second->position, "pointLights[%d].position", currentLight);
      program->setUniformValue(light.second->ambient, "pointLights[%d].ambient", currentLight);
      program->setUniformValue(light.second->diffuse, "pointLights[%d].diffuse", currentLight);
      program->setUniformValue(light.second->specular, "pointLights[%d].specular", currentLight);
      program->setUniformValue(light.second->constant, "pointLights[%d].constant", currentLight);
      program->setUniformValue(light.second->linear, "pointLights[%d].linear", currentLight);
      program->setUniformValue(light.second->quadratic, "pointLights[%d].quadratic", currentLight);
      nr_point_lights++;
      ++currentLight;

      program->setUniformValue(light.second->position, "lightPos", currentLight);
      break;
    }
  }
  program->setUniformValue(nr_point_lights, "countOfPointLights");
  // spotLight
  auto flashLight = m_SpotLights.find("flashLight");
  if (flashLight != m_SpotLights.end())
  {
    program->setUniformValue(m_Camera->Position, "spotLight.position");
    program->setUniformValue(m_Camera->Front, "spotLight.direction");
    program->setUniformValue(flashLight->second->ambient, "spotLight.ambient");
    program->setUniformValue(flashLight->second->diffuse, "spotLight.diffuse");
    program->setUniformValue(flashLight->second->diffuse, "spotLight.specular");
    program->setUniformValue(flashLight->second->constant, "spotLight.constant");
    program->setUniformValue(flashLight->second->linear, "spotLight.linear");
    program->setUniformValue(flashLight->second->quadratic, "spotLight.quadratic");
    program->setUniformValue(glm::cos(glm::radians(flashLight->second->cutOff)), "spotLight.cutOff");
    program->setUniformValue(glm::cos(glm::radians(flashLight->second->outerCutOff)), "spotLight.outerCutOff");
  }
}

Scene::Scene(std::string name) : name(name), m_ScreenShader(new CShaderProgram(CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), CShader::load("res/shaders/screenshader.frag", CShader::E_FRAGMENT)))
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
	// screen quad VAO
	/*
	unsigned int quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	*/

	m_ScreenShader->create();
	m_ScreenShader->setUniformValue(0,"screenTexture");
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
	/*
	for (decltype(selected_object_it) current_it = ++selected_object_it; current_it != m_Objects.end(); current_it++)
	{
		if (current_it->second->visible())
		{
			selected_object_it = current_it;
			return;

		}
	}
	*/
}

Object* Scene::selectedObject()
{
	if (selected_object_it == m_Objects.end())
		return m_Objects.begin()->second;
	return selected_object_it->second;
}

void Scene::draw(float dt)
{ 
	
  if (m_Objects.size() > 0)
  {
    for (const auto& object : m_Objects) {
      //object.second->rotate(dt*0.01f, {0,1,0});
      //object.second->getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));
			if (!object.second->m_transparent && (object.second->visible()) && 
				glm::abs(glm::distance(m_Camera->Position, object.second->m_transform.position)) < m_Camera->zFar - 500.0f)
			{
				CShaderProgram* program = object.second->m_Material->program;
				program->use();
				setupLights(object.second);

				program->setUniformValue(m_Camera->Position, "viewPos");
				object.second->draw(m_Camera);
			}
    }
    for (const auto& object : m_Objects) {
      //object.second->rotate(dt*0.01f, {0,1,0});
      //object.second->getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));
			if (object.second->m_transparent && (object.second->visible()))
			{
				CShaderProgram* program = object.second->m_Material->program;
				program->use();
				setupLights(object.second);

				program->setUniformValue(m_Camera->Position, "viewPos");
				object.second->draw(m_Camera);
			}
    }
    Object* lightObject = m_Objects.find("light")->second;
    CShaderProgram* program = lightObject->m_Material->program;
    for (const auto& light : m_PointLights) {
      program->use();
      lightObject->moveTo(light.second->position);
      lightObject->draw(m_Camera);
    }
  }
}

void Scene::addObject(std::string name, Object *object)
{
  //if (m_Objs.find(name) != m_Objs.end())

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

bool Scene::save(std::string as)
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
  xmlDoc.InsertFirstChild(pScene);

  if (as == "")
    sceneName << "res/scenes/" << name << ".xml";
  else
    sceneName << "res/scenes/" << as;
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

  XMLElement * lights = pScene->FirstChildElement("light");
  while (lights != nullptr)
  {
    loadLight(lights);
    lights = lights->NextSiblingElement("light");
  }
	selected_object_it = m_Objects.begin();
  return true;
}

void Scene::setRenderTarget(FrameBufferObject *renderedScene)
{
  m_RenderedScene = renderedScene;
}

FrameBufferObject* Scene::getRenderTarget()
{
	return m_RenderedScene;
}

void Scene::begin()
{
  m_RenderedScene->bind();
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // буфер трафарета не используется
	glEnable(GL_DEPTH_TEST);
}

void Scene::end()
{
  m_RenderedScene->unbind();
}

void Scene::present()
{
	if (postProcessor == nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		m_ScreenShader->use();
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RenderedScene->texture);
		m_ScreenQuad.draw();;
	}
	else
	{
		postProcessor->Do(m_RenderedScene->texture);
	}
}

void Scene::setPostProcessor(IPostProcessor* postProcessor)
{
	this->postProcessor = postProcessor;
}

