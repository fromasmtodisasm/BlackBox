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
  {
    auto vPath = "res/shaders/" + vShader;
    auto v = cache.find(vPath);
    if (v != cache.end())
    {
      vs = v->second;
    }
    else {
      vs = CShader::load(vPath, CShader::E_VERTEX);
      cache[vPath] = vs;
    }
  }
  {
    auto fPath = "res/shaders/" + fShader;
    auto f = cache.find(fPath);
    if (f != cache.end())
    {
      fs = f->second;
    }
    else {
      fs = CShader::load(fPath, CShader::E_FRAGMENT);
      cache[fPath] = fs;
    }
  }
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

bool ShaderManager::init()
{
  defaultProgram = ShaderManager::instance()->getProgram("vertex.glsl", "fragment.glsl");
  defaultProgram->create();
}
