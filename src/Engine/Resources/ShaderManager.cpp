#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

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
  //TODO: !!!
  _smart_ptr<CShader> vs, fs;
  _smart_ptr<CShaderProgram> p;
#if BB_PLATFORM_WINDOWS 
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
#endif
  return p;
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

ShaderRef  ShaderManager::getShader(ShaderDesc const& desc, bool isReload)
{
  ShaderRef result = nullptr;
  auto Path = root + desc.name;
  auto shader = cache.find(Path);
  if (shader != cache.end() && !isReload)
  {
    result = shader->second;
  }
  else {
    result = addShader(desc);
  }
  return result;
}

ShaderRef ShaderManager::addShader(const ShaderDesc& desc)
{
  //result = std::make_shared<CShader>(static_cast<CShader*>(CShader::load(desc)));
  auto result = CShader::load(desc);
  auto Path = root + desc.name;
  result->m_Path = Path;
  if (result)
  {
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
#if _WIN32
  defaultProgram = ShaderManager::instance()->getProgram("vertex.glsl", "fragment.glsl");
  defaultProgram->Create("default");
#endif
  ShaderDesc::root = ShaderManager::instance()->root;
  CBaseShaderProgram::print_loc_name = GetISystem()->GetIConsole()->GetCVar("shader_print");
  return true;
}
