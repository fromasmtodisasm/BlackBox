#pragma once
#include <BlackBox/Render/CShader.hpp>

#include <map>
#include <string>

class ShaderManager
{
  static ShaderManager *manager;
  std::map<std::string, CShader*> cache;
  //ShaderManager();
public:
  static ShaderManager *instance();
  CBaseShaderProgram *getProgram(std::string vShader, std::string fShader);
  CShader *getShader(std::string name, std::string type);
  static bool init();
private:
  CShader::type str2typ(std::string type);
};
