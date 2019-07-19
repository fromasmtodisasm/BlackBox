#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Resources/TextureManager.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <glm/glm.hpp>

#include <tinyxml2.h>
#ifndef XMLCheckResult
  #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

using	namespace std;
using namespace tinyxml2;

MaterialManager *MaterialManager::manager = nullptr;
Material *defaultMaterial;

MaterialManager *MaterialManager::instance()
{
  if (manager == nullptr)
  {
    manager = new MaterialManager();
  }
  return manager;
}

Material *MaterialManager::getMaterial(std::string name)
{
  Material *material = nullptr;

  /*
  std::string prefix = "res/materials/";
  bool usPrefix = true;
  if (name.find("/") != name.npos)
    usPrefix = false;
  */
  {
    std::string fullName = name;
    /*
    if (usPrefix)
      fullName = prefix + name;
    else fullName = name;
    */

    auto matItor = cache.find(fullName);
    if (matItor != cache.end())
    {
      material = matItor->second;
    }
    else {
      m_pLog->AddLog("[ERROR] Load material\n");
    }
  }

  return material;
}

bool MaterialManager::init(std::string materialLib)
{
  bool status = false;
  status = MaterialManager::instance()->loadLib(materialLib);
  if (status)
  {
    /*
    defaultMaterial = new Material();
    defaultMaterial->diffuse = TextureManager::instance()->getTexture("check.jpg");
    defaultMaterial->program = defaultProgram;
    */
    defaultMaterial = MaterialManager::instance()->getMaterial("default");
    //defaultMaterial->name = std::make_shared<std::string>("default");
  }
  return status;
}

MaterialManager::MaterialManager() : m_pLog(GetIEngine()->getILog())
{
}

bool MaterialManager::loadLib(std::string name)
{
  XMLDocument xmlDoc;

  XMLError eResult = xmlDoc.LoadFile(("res/materials/" + name).c_str());
  XMLCheckResult(eResult);

  XMLNode * materials = xmlDoc.FirstChild();
  if (materials == nullptr) return false;

  XMLElement * material = materials->FirstChildElement("material");

  if (material == nullptr) return false;

  while (material != nullptr)
  {
    if (!loadMaterial(material))
    {
      //TODO: handle this case
      m_pLog->AddLog("[ERROR] Failed load material\n");
    }
    material = material->NextSiblingElement("material");
  }
  if (cache.size() != 0) return true;
  else return false;
}

bool MaterialManager::loadMaterial(XMLElement *material)
{
  Material *result = new Material();

  const char *materialName = nullptr;
	isSkyBox = false;

  materialName = material->Attribute("name");
  if (materialName == nullptr)
    return false;
	if (material->Attribute("type", "skybox"))
		isSkyBox = true;
	if (material->Attribute("shakaled"))
		alpha_shakaled = true;
	else
		alpha_shakaled = false;

  result->name = std::make_shared<std::string>(materialName);
  //============ TEXTURES LOADING =======================//
  XMLElement *textures = material->FirstChildElement("textures");
  XMLElement *colorElement = material->FirstChildElement("color");
  result->diffuseColor = glm::vec3(1.0, 0.5, 0.31);
  if (colorElement != nullptr)
  {
    result->diffuseColor.r = colorElement->FloatAttribute("red");
    result->diffuseColor.g = colorElement->FloatAttribute("green");
    result->diffuseColor.b = colorElement->FloatAttribute("blue");
  }
  if (textures != nullptr)
  {
    result->hasTexture = true;
    XMLElement *image = textures->FirstChildElement("texture");
    if (image == nullptr) return false;
    while (image != nullptr)
    {
      BaseTexture *t = loadTexture(image);
      switch(t->type)
      {
      case TextureType::DIFFUSE:
        result->diffuse.push_back(t);
        break;
      case TextureType::SPECULAR:
        result->specular = t;
        break;
      case TextureType::BUMP:
        result->bump = t;
        break;
      case TextureType::NORMAL:
        result->normal = t;
        break;
      case TextureType::MASK:
        result->mask = t;
        break;
      default:
      {
        m_pLog->AddLog("[ERROR] Unknown texture type\n");
      }
      }
      image = image->NextSiblingElement("texture");
    }
  }
  else {
    result->hasTexture = false;
  }
	result->current_diffuse = 0;
  //============ SHADERS LOADING =======================//
  XMLElement *shaders = material->FirstChildElement("shaders");
  if (shaders == nullptr) return false;
  XMLElement *shader = shaders->FirstChildElement("shader");
  if (shader == nullptr) return false;
  CShaderProgram *program = new CShaderProgram();
  while (shader != nullptr)
  {
    CShader *s = loadShader(shader);
    if (s == nullptr)
      //TODO: log it!!!
      return false;
    program->attach(s);
    shader = shader->NextSiblingElement("shader");
  }
  if (!program->create())
    return false;
  result->program = program;
  cache[materialName] = result;
  m_pLog->AddLog("[INFO] Created material: %s\n", materialName);
  return true;
}

BaseTexture *MaterialManager::loadTexture(XMLElement *texture)
{
  BaseTexture *result;
  TextureManager *textureManager = TextureManager::instance();
  const char *type = nullptr;
  const char *name = nullptr;

  type = texture->Attribute("type");
  name = texture->Attribute("name");

  result = textureManager->getTexture(name, isSkyBox);
  if (result == nullptr)
    return result;
  result->setType(type);
  return result;
}

XMLElement *MaterialManager::saveTexture(XMLDocument &xmlDoc, Texture *texture)
{
  XMLElement *textureElement = xmlDoc.NewElement("shader");

  textureElement->SetAttribute("type", texture->typeToStr().c_str());
  textureElement->Attribute("name", texture->path->c_str());

  return textureElement;

}

CShader *MaterialManager::loadShader(XMLElement *shader)
{
  ShaderManager *shaderManager = ShaderManager::instance();
  const char *type = nullptr;
  const char *name = nullptr;

  type = shader->Attribute("type");
  name = shader->Attribute("name");

  return shaderManager->getShader(name, type);
}

XMLElement *MaterialManager::saveShader(XMLDocument &xmlDoc, CShader *shader)
{
  XMLElement *shaderElement = xmlDoc.NewElement("shader");

  shaderElement->SetAttribute("type", shader->typeToStr().c_str());
  shaderElement->Attribute("name", shader->m_path->c_str());

  return shaderElement;
}
