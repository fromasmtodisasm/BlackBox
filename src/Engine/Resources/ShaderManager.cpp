#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/System/IConsole.hpp>

ShaderManager* ShaderManager::manager = nullptr;
static _smart_ptr<CShaderProgram> defaultProgram = nullptr;

std::string ShaderDesc::root = "res/shaders/";

ICVar* CBaseShaderProgram::print_loc_name = nullptr;
ICVar* CBaseShaderProgram::use_cache = nullptr;

ShaderManager* ShaderManager::instance()
{
  if (manager == nullptr)
  {
    manager = new ShaderManager();
  }
  return manager;
}

_smart_ptr<CShaderProgram>  ShaderManager::getProgram(std::string vShader, std::string fShader)
{
  _smart_ptr<CShader> vs, fs;
  _smart_ptr<CShaderProgram> p;
  vs = getShader(std::move(ShaderDesc(vShader, "vertex")), false);
  fs = getShader(ShaderDesc(fShader, "fragment"), false);
  if (!vs || !fs)
  {
    GetISystem()->GetILog()->Log("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->Log("[OK] Shaders loaded\n");
    return p = _smart_ptr<CShaderProgram>(new CShaderProgram(vs, fs));
  }
}

_smart_ptr<CShaderProgram> ShaderManager::getProgram(std::string vShader, std::string fShader, std::string gShader)
{
  _smart_ptr<CShader> vs, fs, gs;
  _smart_ptr<CShaderProgram> p;
  vs = getShader(ShaderDesc(vShader, "vertex"), false);
  fs = getShader(ShaderDesc(fShader, "fragment"), false);
  gs = getShader(ShaderDesc(fShader, "geometry"), false);
  if (!vs || !fs)
  {
    GetISystem()->GetILog()->Log("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->Log("[OK] Shaders loaded\n");
    assert(0 && "Not implemented");
    return nullptr;
    //return p = std::make_shared<CShaderProgram>(vs, fs, gs);
  }
}

_smart_ptr<CShaderProgram> ShaderManager::getProgram(std::string vShader, std::string fShader, std::string gShader, std::string cShader)
{
  _smart_ptr<CShader> vs, fs, gs, cs;
  _smart_ptr<CShaderProgram> p;
  vs = getShader(ShaderDesc(vShader, "vertex"), false);
  fs = getShader(ShaderDesc(fShader, "fragment"), false);
  if (gShader.size() > 0) gs = getShader(ShaderDesc(fShader, "geometry"), false);
  cs = getShader(ShaderDesc(fShader, "compute"), false);
  if (!vs || !fs)
  {
    GetISystem()->GetILog()->Log("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->Log("[OK] Shaders loaded\n");
    assert(0 && "Not implemented");
    return nullptr;
    //return p = std::make_shared<CShaderProgram>(vs, fs, gs, cs);
  }
}

_smart_ptr<CShaderProgram> ShaderManager::getDefaultProgram()
{
  return defaultProgram;
}

_smart_ptr<CShader> ShaderManager::getShader(ShaderDesc const& desc, bool isReload)
{
  _smart_ptr<CShader> result = nullptr;
  auto Path = root + desc.name;
  auto shader = cache.find(Path);
  if (shader != cache.end() && !isReload)
  {
    result = shader->second;
  }
  else {
    //result = std::make_shared<CShader>(static_cast<CShader*>(CShader::load(desc)));
    result = CShader::load(desc);
    result->m_Path = Path;
    if (!result)
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
  defaultProgram->Create("default");
  ShaderDesc::root = ShaderManager::instance()->root;
  CBaseShaderProgram::print_loc_name = GetISystem()->GetIConsole()->GetCVar("shader_print");
  return true;
}