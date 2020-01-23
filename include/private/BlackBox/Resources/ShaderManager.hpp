#pragma once
#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <map>
#include <string>

class ShaderManager
{
  static ShaderManager* manager;
  std::map<std::string, _smart_ptr<CShader>> cache;
  std::string root = "res/shaders/";
  //ShaderManager();
public:
  static ShaderManager* instance();
  _smart_ptr<CShaderProgram> getProgram(std::string vShader, std::string fShader);
  _smart_ptr<CShaderProgram> getProgram(std::string vShader, std::string fShader, std::string gShader);
  _smart_ptr<CShaderProgram> getProgram(std::string vShader, std::string fShader, std::string gShader, std::string cShader);
  _smart_ptr<CShaderProgram> getDefaultProgram();
  _smart_ptr<CShader> getShader(ShaderDesc const& desc, bool isReload);
  void removeShader(std::string name);
  static bool init();
private:
};