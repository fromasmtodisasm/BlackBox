#pragma once

#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>

#include <map>
#include <string>
#include <tinyxml2.h>

extern Material* defaultMaterial;
struct ISystem;

struct IMaterialShaderSink
{
  virtual void OnShaderFound(const std::string& name) = 0;
};

class MaterialManager
{
  static MaterialManager* manager;
  std::map<std::string, Material*> cache;
  ILog* m_pLog;
  bool alpha_shakaled = false;
  bool isSkyBox = false;
  ICVar* root_path = nullptr;
  std::map<std::string, BaseShaderProgramRef> shaders_map;
  ISystem* m_pSystem;
public:
  static MaterialManager* instance();
  static bool init(ISystem* pSystem);
  BaseShaderProgramRef getProgram(std::string name);
  Material* getMaterial(std::string name);
  static bool init(std::string materialLib);
  bool reloadShaders();
  bool reloadShaders(std::vector<std::string> names);

  void reloadShader(ProgramDesc& pd);
  bool loadProgram(ProgramDesc& desc, bool isReload);
  void EnumShaders(IMaterialShaderSink* callback);

private:
  MaterialManager(ISystem* pSystem);
  bool loadLib(std::string name);
  void getShaderAttributes(tinyxml2::XMLElement* shader, ProgramDesc& pd);
  bool loadMaterial(tinyxml2::XMLElement* material);
  BaseTexture* loadTexture(tinyxml2::XMLElement* texture);
  tinyxml2::XMLElement* saveTexture(tinyxml2::XMLDocument& xmlDoc, Texture* texture);
  ShaderRef loadShader(ShaderDesc& sd, bool isReload);
  ShaderRef addShader(ShaderDesc& sd, bool isReload);
  tinyxml2::XMLElement* saveShader(tinyxml2::XMLDocument& xmlDoc, CShader* shader);
};