#pragma once
#include <BlackBox/Render/Shader.hpp>

#include <map>
#include <string>

class ShaderManager
{
  static ShaderManager *manager;
  std::map<std::string, std::shared_ptr<CShader>> cache;
	std::string root = "res/shaders/";
  //ShaderManager();
public:
  static ShaderManager *instance();
  std::shared_ptr<CShaderProgram> getProgram(std::string vShader, std::string fShader);
  std::shared_ptr<CShaderProgram> getProgram(std::string vShader, std::string fShader, std::string gShader);
  std::shared_ptr<CShaderProgram> getProgram(std::string vShader, std::string fShader, std::string gShader, std::string cShader);
  std::shared_ptr<CShaderProgram> getDefaultProgram();
  std::shared_ptr<CShader> getShader(std::string name, std::string type, bool isReload);
  void removeShader(std::string name);
  static bool init();
private:
  CShader::type str2typ(std::string type);
};
