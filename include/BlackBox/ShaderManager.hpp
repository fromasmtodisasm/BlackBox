#pragma once
#include <BlackBox/CShader.hpp>

#include <map>
#include <string>

class ShaderManager
{
  static ShaderManager *manager;
  std::map<std::string, CShader*> cache;
  //ShaderManager();
public:
  static ShaderManager *instance();
  CShaderProgram *getProgram(std::string vShader, std::string fShader);
};
