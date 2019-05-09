#include <BlackBox/ShaderManager.hpp>

#include <iostream>
using	namespace std;

ShaderManager *ShaderManager::manager = nullptr;
CShaderProgram *defaultProgram;

ShaderManager *ShaderManager::instance()
{
  if (manager == nullptr)
  {
    manager = new ShaderManager();
  }
  return manager;
}

CShaderProgram *ShaderManager::getProgram(std::string vShader, std::string fShader)
{
  CShader *vs, *fs;
  CShaderProgram *p;
  vs = getShader(vShader, "vertex");
  fs = getShader(fShader, "fragment");
  if (vs == nullptr || fs == nullptr)
  {
    cout << "Error of load shader" << endl;
    return nullptr;
  }
  else
  {
    cout << "Shaders loaded" << endl;
    return p = new CShaderProgram(vs, fs);
  }
}

CShader *ShaderManager::getShader(string name, string type)
{
  CShader *result = nullptr;
  auto Path = "res/shaders/" + name;
  auto shader = cache.find(Path);
  if (shader != cache.end())
  {
    result = shader->second;
  }
  else {
    result = CShader::load(Path, str2typ(type));
    if (result == nullptr)
      return result;
    cache[Path] = result;
  }
  return result;
}

bool ShaderManager::init()
{
  defaultProgram = ShaderManager::instance()->getProgram("vertex.glsl", "fragment.glsl");
  defaultProgram->create();
}

CShader::type ShaderManager::str2typ(string type)
{
  if (type == "vertex")
    return CShader::type::E_VERTEX;
  else if (type == "fragment")
    return CShader::type::E_FRAGMENT;
  else
    return CShader::type::E_UNKNOWN;
}
