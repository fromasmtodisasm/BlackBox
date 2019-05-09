#include <BlackBox/MaterialManager.hpp>
#include <BlackBox/TextureManager.hpp>
#include <BlackBox/ShaderManager.hpp>

#include <tinyxml2.h>
#ifndef XMLCheckResult
  #define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

#include <iostream>
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

  std::string prefix = "res/materials/";
  bool usPrefix = true;
  if (name.find("/") != name.npos)
    usPrefix = false;
  {
    std::string fullName;
    if (usPrefix)
      fullName = prefix + name;
    else fullName = name;

    auto matItor = cache.find(fullName);
    if (matItor != cache.end())
    {
      material = matItor->second;
    }
    else {
      cout << "Error of load material" << endl;
    }
  }

  return material;
}

bool MaterialManager::init(std::string materialLib)
{
  defaultMaterial = new Material();
  defaultMaterial->diffuse = TextureManager::instance()->getTexture("check.jpg");
  defaultMaterial->program = defaultProgram;
  return MaterialManager::instance()->loadLib(materialLib);
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
      cout << "Failed load material" << endl;
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

  materialName = material->Attribute("name");
  if (materialName == nullptr)
    return false;
  //============ TEXTURES LOADING =======================//
  XMLElement *textures = material->FirstChildElement("textures");
  if (textures == nullptr) return false;
  XMLElement *image = textures->FirstChildElement("texture");
  if (image == nullptr) return false;
  while (image != nullptr)
  {
    Texture *t = loadTexture(image);
    switch(t->type)
    {
    case TextureType::DIFFUSE:
      result->diffuse = t;
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
      cout << "Error: unknown texture type" << endl;
    }
    }
    image = image->NextSiblingElement("texture");
  }
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
  return true;

}

Texture *MaterialManager::loadTexture(XMLElement *texture)
{
  Texture *result;
  TextureManager *textureManager = TextureManager::instance();
  const char *type = nullptr;
  const char *name = nullptr;

  type = texture->Attribute("type");
  name = texture->Attribute("name");

  result = textureManager->getTexture(name);
  if (result == nullptr)
    return result;
  result->setType(type);
  return result;
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
