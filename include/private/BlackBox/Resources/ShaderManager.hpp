#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <map>
#include <string>

class IRENDER_API ShaderManager
{
  static ShaderManager* manager;
  std::map<std::string, _smart_ptr<CShader>> cache;
  std::string root = "res/shaders/";
  //ShaderManager();
public:
  static ShaderManager* instance();
  ShaderProgramRef getProgram(std::string vShader, std::string fShader);
  ShaderProgramRef getProgram(std::string vShader, std::string fShader, std::string gShader);
  ShaderProgramRef getProgram(std::string vShader, std::string fShader, std::string gShader, std::string cShader);
  ShaderProgramRef getDefaultProgram();
  ShaderRef getShader(ShaderDesc const& desc, bool isReload);
  ShaderRef addShader(const ShaderDesc& desc);
  void removeShader(std::string name);
  static bool init();
private:
};