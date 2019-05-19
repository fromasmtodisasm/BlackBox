#include <BlackBox/ShaderManager.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/ILog.hpp>

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
    GetIEngine()->getILog()->AddLog("Error of load shader");
    return nullptr;
  }
  else
  {
    GetIEngine()->getILog()->AddLog("[OK] Shaders loaded\n");
    return p = new CShaderProgram(vs, fs);
  }
}

CShader *ShaderManager::getShader(std::string name, std::string type)
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
    result->m_path = std::make_shared<std::string>(Path);
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
  return true;
}

CShader::type ShaderManager::str2typ(std::string type)
{
  if (type == "vertex")
    return CShader::type::E_VERTEX;
  else if (type == "fragment")
    return CShader::type::E_FRAGMENT;
  else
    return CShader::type::E_UNKNOWN;
}
