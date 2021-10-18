#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/ShaderManager.hpp>
#include <BlackBox/Renderer/TextureManager.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ISystem.hpp>

#if 0
#include <tinyxml2.h>
#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

using	namespace std;
using namespace tinyxml2;
using ShaderInfo = IShaderProgram::ShaderInfo;

MaterialManager* MaterialManager::manager = nullptr;
Material* defaultMaterial;

MaterialManager* MaterialManager::instance()
{
  if (manager == nullptr)
  {
    manager = new MaterialManager(GetISystem());
  }
  return manager;
}

bool MaterialManager::init(ISystem* pSystem)
{
  if (manager == nullptr)
    manager = new MaterialManager(pSystem);
  return manager != nullptr;
}

Material* MaterialManager::getMaterial(std::string name)
{
  Material* material = nullptr;

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
      m_pLog->Log("[ERROR] Load material\n");
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

MaterialManager::MaterialManager(ISystem* pSystem) : m_pSystem(pSystem), m_pLog(pSystem->GetILog())
{
  root_path = m_pSystem->GetIConsole()->GetCVar("materials_path");
  m_pSystem->GetIConsole()->CreateVariable("ef", 40.f, 0, "emissive factor");
}

bool MaterialManager::loadLib(std::string name)
{
  tinyxml2::XMLDocument xmlDoc;

  XMLError eResult = xmlDoc.LoadFile((root_path->GetString() + name).c_str());
  XMLCheckResult(eResult);

  XMLNode* materials = xmlDoc.FirstChild();
  if (materials == nullptr) return false;

  XMLElement* material = materials->FirstChildElement("material");
  XMLElement* shaders = materials->FirstChildElement("shaders");

  if (material == nullptr || shaders == nullptr) return false;

  XMLNode* program = shaders->FirstChild();

  while (material != nullptr)
  {
    if (!loadMaterial(material))
    {
      //TODO: handle this case
      m_pLog->Log("[ERROR] Failed load material\n");
    }
    material = material->NextSiblingElement("material");
  }
  if (cache.size() != 0) return true;
  else return false;
}

void MaterialManager::getShaderAttributes(tinyxml2::XMLElement* shader, ProgramDesc& pd)
{
  std::string type;
  std::string name;

  type = shader->Attribute("type");
  name = shader->Attribute("name");

  if (type == "vertex")
    pd.vs = name;
  else if (type == "geometry")
    pd.gs = name;
  else if (type == "fragment")
    pd.fs = name;
}

bool MaterialManager::loadMaterial(XMLElement* material)
{
  Material* result = new Material();

  const char* materialName = nullptr;
  isSkyBox = false;

  materialName = material->Attribute("name");
  if (materialName == nullptr)
    return false;
  result->alpha = material->FloatAttribute("alpha", 1.f);
  if (material->Attribute("type", "skybox"))
    isSkyBox = true;
  if (material->Attribute("shakaled"))
    alpha_shakaled = true;
  else
    alpha_shakaled = false;

  result->emissive_factor = material->FloatAttribute("emmisive_factor", 1.0f);
  result->name = std::make_shared<std::string>(materialName);
  //============ TEXTURES LOADING =======================//
  XMLElement* textures = material->FirstChildElement("textures");
  XMLElement* colorElement = material->FirstChildElement("color");
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
    XMLElement* image = textures->FirstChildElement("texture");
    if (image == nullptr) return false;
    while (image != nullptr)
    {
      BaseTexture* t = loadTexture(image);
      switch (t->type)
      {
      case TextureType::DIFFUSE:
        t->setUnit(0);
        result->diffuse.push_back(t);
        break;
      case TextureType::SPECULAR:
        t->setUnit(1);
        result->specular = t;
        break;
      case TextureType::BUMP:
        t->setUnit(2);
        result->bump = t;
        break;
      case TextureType::NORMAL:
        t->setUnit(3);
        result->normal = t;
        break;
      case TextureType::MASK:
        t->setUnit(4);
        result->mask = t;
        break;
      case TextureType::EMISSIVE:
        t->setUnit(5);
        result->emissive = t;
        break;
      default:
      {
        m_pLog->Log("[ERROR] Unknown texture type\n");
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
  XMLElement* shader = material->FirstChildElement("shader");
  if (shader == nullptr) return false;
  const char* shader_name = nullptr;
  shader_name = shader->Attribute("name");
  if (shader_name == nullptr)
    return false;

  if (!(result->program = gEnv->pRenderer->Sh_Load(shader_name, 0)))
    return false;
  result->program_name = shader_name;
  cache[materialName] = result;
  m_pLog->Log("[INFO] Created material: %s\n", materialName);
  return true;
}

BaseTexture* MaterialManager::loadTexture(XMLElement* texture)
{
  BaseTexture* result;
  TextureManager* textureManager = TextureManager::instance();
  const char* type = nullptr;
  const char* name = nullptr;

  type = texture->Attribute("type");
  name = texture->Attribute("name");

  result = textureManager->getTexture(name, isSkyBox);
  if (result == nullptr)
    return result;
  result->setType(type);
  return result;
}

XMLElement* MaterialManager::saveTexture(tinyxml2::XMLDocument& xmlDoc, Texture* texture)
{
  XMLElement* textureElement = xmlDoc.NewElement("shader");

  textureElement->SetAttribute("type", texture->typeToStr());
  textureElement->Attribute("name", texture->path->c_str());

  return textureElement;
}

#if 0
XMLElement* MaterialManager::saveShader(tinyxml2::XMLDocument& xmlDoc, ShaderRef shader)
{
  XMLElement* shaderElement = xmlDoc.NewElement("shader");

  shaderElement->SetAttribute("type", shader->typeToStr());
  shaderElement->Attribute("name", shader->m_Path.c_str());

  return shaderElement;
}
#endif
#endif
