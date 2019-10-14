#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/ILog.hpp>

ShaderManager *ShaderManager::manager = nullptr;
static std::shared_ptr<CShaderProgram> defaultProgram = nullptr;

ShaderManager *ShaderManager::instance()
{
  if (manager == nullptr)
  {
    manager = new ShaderManager();
  }
  return manager;
}

std::shared_ptr<CShaderProgram>  ShaderManager::getProgram(std::string vShader, std::string fShader)
{
  std::shared_ptr<CShader> vs, fs;
  std::shared_ptr<CShaderProgram> p;
  vs = getShader(vShader, "vertex", false);
  fs = getShader(fShader, "fragment", false);
  if (vs == nullptr || fs == nullptr)
  {
    GetISystem()->GetILog()->AddLog("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->AddLog("[OK] Shaders loaded\n");
    return p = std::make_shared<CShaderProgram>(vs, fs);
  }
}

std::shared_ptr<CShaderProgram> ShaderManager::getProgram(std::string vShader, std::string fShader, std::string gShader)
{
  std::shared_ptr<CShader> vs, fs, gs;
  std::shared_ptr<CShaderProgram> p;
  vs = getShader(vShader, "vertex", false);
  fs = getShader(fShader, "fragment", false);
  gs = getShader(fShader, "geometry", false);
  if (vs == nullptr || fs == nullptr)
  {
    GetISystem()->GetILog()->AddLog("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->AddLog("[OK] Shaders loaded\n");
		assert(0 && "Not implemented");
    //return p = std::make_shared<CShaderProgram>(vs, fs, gs);
  }

}

std::shared_ptr<CShaderProgram> ShaderManager::getProgram(std::string vShader, std::string fShader, std::string gShader, std::string cShader)
{
  std::shared_ptr<CShader> vs, fs, gs, cs;
  std::shared_ptr<CShaderProgram> p;
  vs = getShader(vShader, "vertex", false);
  fs = getShader(fShader, "fragment", false);
	if (gShader.size() > 0) gs = getShader(fShader, "geometry", false);
  cs = getShader(fShader, "compute", false);
  if (vs == nullptr || fs == nullptr)
  {
    GetISystem()->GetILog()->AddLog("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->AddLog("[OK] Shaders loaded\n");
		assert(0 && "Not implemented");
    //return p = std::make_shared<CShaderProgram>(vs, fs, gs, cs);
  }

}

std::shared_ptr<CShaderProgram> ShaderManager::getDefaultProgram()
{
	return defaultProgram;
}

std::shared_ptr<CShader> ShaderManager::getShader(std::string name, std::string type, bool isReload)
{
  std::shared_ptr<CShader> result = nullptr;
  auto Path = root + name;
  auto shader = cache.find(Path);
  if (shader != cache.end() && !isReload)
  {
    result = shader->second;
  }
  else {
    result = CShader::load(Path, str2typ(type));
    result->m_Path = Path;
    if (result == nullptr)
      return result;
    cache[Path] = result;
  }
  return result;
}

void ShaderManager::removeShader(std::string name)
{
	cache.erase(root + name);
}

bool ShaderManager::init()
{
  defaultProgram = ShaderManager::instance()->getProgram("vertex.glsl", "fragment.glsl");
  defaultProgram->create("default");
  return true;
}

CShader::type ShaderManager::str2typ(std::string type)
{
  if (type == "vertex")
    return CShader::type::E_VERTEX;
  else if (type == "fragment")
    return CShader::type::E_FRAGMENT;
  else if (type == "geometry")
    return CShader::type::E_GEOMETRY;
  else if (type == "compute")
    return CShader::type::E_COMPUTE;
  else
    return CShader::type::E_UNKNOWN;
}
