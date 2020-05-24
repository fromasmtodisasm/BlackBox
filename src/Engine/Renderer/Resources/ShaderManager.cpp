#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/ShaderManager.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

ShaderManager* ShaderManager::manager = nullptr;
static ShaderProgramRef defaultProgram = nullptr;

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

ShaderProgramRef  ShaderManager::getProgram(const char* vShader, const char* fShader)
{
  //TODO: !!!
  ShaderRef vs, fs;
  BaseShaderProgramRef p;
#if BB_PLATFORM_WINDOWS 
  vs = getShader(std::move(ShaderDesc(vShader, IShader::E_VERTEX)), false);
  fs = getShader(ShaderDesc(fShader, IShader::E_VERTEX), false);
  if (!vs || !fs)
  {
    GetISystem()->GetILog()->Log("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->Log("[OK] Shaders loaded");
    return p = _smart_ptr<CShaderProgram>(new CShaderProgram(vs, fs));
  }
#endif
  return p;
}

ShaderProgramRef ShaderManager::getProgram(const char* vShader, const char* fShader, const char* gShader)
{
  ShaderRef vs, fs, gs;
  _smart_ptr<CShaderProgram> p;
  vs = getShader(ShaderDesc(vShader, IShader::E_VERTEX), false);
  fs = getShader(ShaderDesc(fShader, IShader::E_FRAGMENT), false);
  gs = getShader(ShaderDesc(fShader, IShader::E_GEOMETRY), false);
  if (!vs || !fs)
  {
    GetISystem()->GetILog()->Log("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->Log("[OK] Shaders loaded");
    assert(0 && "Not implemented");
    return nullptr;
    //return p = std::make_shared<CShaderProgram>(vs, fs, gs);
  }
}

ShaderProgramRef ShaderManager::getProgram(const char* vShader, const char* fShader, const char* gShader, const char* cShader)
{
  ShaderRef vs, fs, gs, cs;
  _smart_ptr<CShaderProgram> p;
  vs = getShader(ShaderDesc(vShader, IShader::E_VERTEX), false);
  fs = getShader(ShaderDesc(fShader, IShader::E_FRAGMENT), false);
  if (gShader != nullptr) gs = getShader(ShaderDesc(fShader, IShader::E_GEOMETRY), false);
  cs = getShader(ShaderDesc(fShader, IShader::E_COMPUTE), false);
  if (!vs || !fs)
  {
    GetISystem()->GetILog()->Log("Error of load shader");
    return nullptr;
  }
  else
  {
    GetISystem()->GetILog()->Log("[OK] Shaders loaded");
    assert(0 && "Not implemented");
    return nullptr;
    //return p = std::make_shared<CShaderProgram>(vs, fs, gs, cs);
  }
}

ShaderProgramRef ShaderManager::getProgram(IShaderProgram::ShaderInfo& vs, IShaderProgram::ShaderInfo& fs, IShaderProgram::ShaderInfo& gs, IShaderProgram::ShaderInfo& cs)
{
  return new CShaderProgram(vs, fs, gs, cs);
}

ShaderProgramRef ShaderManager::loadProgram(ProgramDesc& desc, bool isReload)
{
#if 0
  auto shader_it = shaders_map.find(desc.name);
  bool load_vs = desc.vs.name.length() > 0;
  bool load_fs = desc.fs.name.length() > 0;
  bool load_gs = desc.gs.name.length() > 0;
  bool load_cs = desc.cs.name.length() > 0;

  bool is_compute = load_cs && !load_vs && !load_fs & !load_gs;

  if (shader_it != shaders_map.end() && !isReload)
    return shader_it->second;
  auto vs = !is_compute ? desc.vs.type = "vertex", getShader(desc.vs, isReload) : nullptr;
  if (!vs && !is_compute) return false;

  auto fs = !is_compute ? desc.fs.type = "fragment", getShader(desc.fs, isReload) : nullptr;
  if (!fs && !is_compute) return false;

  decltype(fs) gs;
  decltype(fs) cs;

  if (load_gs)
  {
    desc.gs.type = "geometry";
    gs = getShader(desc.gs, isReload);
    if (!gs) return false;
  }
  if (load_cs)
  {
    desc.cs.type = "compute";
    cs = getShader(desc.cs, isReload);
    if (!cs) return false;
  }

  if (isReload)
  {
    shader_it->second->Reload(vs, fs, gs, cs, desc.name.c_str());
  }
  else
  {
    using ShaderInfo = IShaderProgram::ShaderInfo;
    ShaderInfo vi;
    ShaderInfo fi;
    ShaderInfo gi;
    ShaderInfo ci;

    if (!is_compute)
    {
      vi = ShaderInfo(vs, desc.vs.name);
      fi = ShaderInfo(fs, desc.fs.name);
    }
    if (load_cs && load_gs)
    {
      gi = ShaderInfo(gs, desc.gs.name);
      ci = ShaderInfo(cs, desc.cs.name);
    }
    else if (load_cs && !load_gs)
    {
      ci = ShaderInfo(cs, desc.cs.name);
    }
    if (!load_cs && load_gs)
    {
      gi = ShaderInfo(gs, desc.gs.name);
    }
    ShaderProgramRef shaderProgram(new CShaderProgram(vi, fi, gi, ci));

		if (!shaderProgram->Create(desc.name.c_str()))
		{
			gEnv->pSystem->Log("Error of creating program");
      return false;
		}
    //auto it = shaders_map.find(desc.name);
    shaders_map[desc.name] = shaderProgram;
    //debuger::program_label(shaderProgram->get(), desc.name);
  }
#endif
  return true;
}

ShaderProgramRef ShaderManager::loadProgram(const char* name, int flags)
{
  if (auto it = desc.find(name); it != desc.end())
  {
    return loadProgram(it->second, false);
  }
  return ShaderProgramRef();
}

ShaderProgramRef ShaderManager::getDefaultProgram()
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
  assert(result);
  return result;
}

void ShaderManager::removeShader(const char* name)
{
  cache.erase(root + name);
}

bool ShaderManager::init()
{
#if _WIN32
  //defaultProgram = ShaderManager::instance()->getProgram("vertex.glsl", "fragment.glsl");
  //defaultProgram->Create("default");
#endif
  ShaderDesc::root = ShaderManager::instance()->root;
  CBaseShaderProgram::print_loc_name = GetISystem()->GetIConsole()->GetCVar("shader_print");
  return true;
}
