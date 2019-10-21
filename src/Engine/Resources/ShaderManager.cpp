#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/IConsole.hpp>

ShaderManager *ShaderManager::manager = nullptr;
static std::shared_ptr<CShaderProgram> defaultProgram = nullptr;

std::string ShaderDesc::root = "res/shaders/";

ICVar* CBaseShaderProgram::print_loc_name = nullptr;
ICVar* CBaseShaderProgram::use_cache = nullptr;


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
  vs = getShader(ShaderDesc(vShader, "vertex"), false);
  fs = getShader(ShaderDesc(fShader, "fragment"), false);
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
  vs = getShader(ShaderDesc(vShader, "vertex"), false);
  fs = getShader(ShaderDesc(fShader, "fragment"), false);
  gs = getShader(ShaderDesc(fShader, "geometry"), false);
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
  vs = getShader(ShaderDesc(vShader, "vertex"), false);
  fs = getShader(ShaderDesc(fShader, "fragment"), false);
	if (gShader.size() > 0) gs = getShader(ShaderDesc(fShader, "geometry"), false);
  cs = getShader(ShaderDesc(fShader, "compute"), false);
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

std::shared_ptr<CShader> ShaderManager::getShader(ShaderDesc& desc, bool isReload)
{
  std::shared_ptr<CShader> result = nullptr;
  auto Path = root + desc.name;
  auto shader = cache.find(Path);
  if (shader != cache.end() && !isReload)
  {
    result = shader->second;
  }
  else {
    result = CShader::load(desc);
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
	ShaderDesc::root = ShaderManager::instance()->root;
	CBaseShaderProgram::print_loc_name = GetISystem()->GetIConsole()->GetCVar("shader_print");
  return true;
}
