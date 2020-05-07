#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/System.hpp>
#include <BlackBox/System/ConsoleCommands.hpp>
#include <BlackBox/Core/Utils.hpp>

class SystemCommand : public BaseCommand
{
protected:
  ISystem* m_pSystem;
  SystemCommand(ISystem *pSystem): m_pSystem(pSystem) {}
  virtual ~SystemCommand() {};

};

//*******************************************************
#if 0
class MaterialCommand : public BaseCommand
{
  IWorld* m_World;
public:
  MaterialCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    if (cd.args.size() == 1)
    {
      std::string name = wstr_to_str(cd.args[0]);
      Material* m = MaterialManager::instance()->getMaterial(name);
      if (!m)
        return false;
      game->getWorld()->GetActiveScene()->selectedObject()->second->setMaterial(m);
      return true;
    }
    return false;
  }
};

MaterialCommand::MaterialCommand(CGame* game) : BaseCommand()
{
  m_World = game->getWorld();
}
#endif
//*******************************************************
#if 0
class ShaderCommand : public SystemCommand, public IMaterialShaderSink
{
  IWorld* m_World;
public:
  ShaderCommand(ISystem* pSystem);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    if (cd.args.size() > 0)
    {
      if (cd.args[0] == L"reload")
        return reload(cd);
      else if (cd.args[0] == L"set")
        return move(cd);
      if (cd.args[0] == L"dump")
        return dump(cd);
      if (cd.args[0] == L"edit")
        return edit(cd);
      if (cd.args[0] == L"enum")
        return enumerate(cd);
    }
    else
    {
      GetISystem()->GetIConsole()->PrintLine("help");
    }
    return false;
  }
  bool reload(CommandDesc& cd);
  bool move(CommandDesc& cd);
  bool dump(CommandDesc& cd);
  bool edit(CommandDesc& cd);
  bool enumerate(CommandDesc& cd);

  // Inherited via IMaterialShaderSink
  virtual void OnShaderFound(const std::string& name) override
  {
    GetISystem()->GetIConsole()->PrintLine("Program: %s", name.c_str());
  }
};

ShaderCommand::ShaderCommand(ISystem* pSystem) : SystemCommand(pSystem)
{
  m_World = pSystem->GetIRenderer()->GetIWorld();
}

bool ShaderCommand::reload(CommandDesc& cd)
{
  switch (cd.args.size())
  {
  case 1:
  {
    if (MaterialManager::instance()->reloadShaders())
      return true;
    else
      return false;
  }
  default:
  {
    std::vector<std::string> shaders;
    for (auto arg = cd.args.begin()++; arg != cd.args.end(); arg++)
    {
      shaders.push_back(wstr_to_str(*arg));
    }
    if (MaterialManager::instance()->reloadShaders(shaders))
      return true;
    else
      return false;
  }
  }
  return false;
}

bool ShaderCommand::move(CommandDesc& cd)
{
  if (cd.args.size() == 2)
  {
    assert(0 && "Goodbye");
    auto s = MaterialManager::instance()->getProgram(wstr_to_str(cd.args[1]));
    //game->getWorld()->GetActiveScene()->selectedObject()->second->getMaterial()->program = s;
    return true;
  }
  return false;
}
bool ShaderCommand::dump(CommandDesc& cd)
{
  if (cd.args.size() == 2)
  {
    _smart_ptr<IShaderProgram> s = MaterialManager::instance()->getProgram(wstr_to_str(cd.args[1]));
    if (!s)
      return false;
    s->Dump();
    GetISystem()->GetIConsole()->ExecuteString("exec os @EDITOR dump.bin");
    return true;
  }
  return false;
}
bool ShaderCommand::edit(CommandDesc& cd)
{
  if (cd.args.size() == 3)
  {
    _smart_ptr<IShaderProgram> s = MaterialManager::instance()->getProgram(wstr_to_str(cd.args[2]));
    if (!s)
      return false;
    std::string shader_name;
    std::string type = wstr_to_str(cd.get(1));
    if (type == "vs")
      shader_name = s->GetShaderName(IShader::type::E_VERTEX);
    else if (type == "fs")
      shader_name = s->GetShaderName(IShader::type::E_FRAGMENT);
    else if (type == "gs")
      shader_name = s->GetShaderName(IShader::type::E_GEOMETRY);
    else if (type == "cs")
      shader_name = s->GetShaderName(IShader::type::E_COMPUTE);
    if (shader_name.length() > 0)
      //GetISystem()->GetIConsole()->ExecuteString((std::string("exec os @EDITOR -multiInst -lcpp ") + GetISystem()->GetIConsole()->GetCVar("shader_path")->GetString() + shader_name).c_str());
      GetISystem()->GetIConsole()->ExecuteString((std::string("exec os @EDITOR -lcpp ") + GetISystem()->GetIConsole()->GetCVar("shader_path")->GetString() + shader_name).c_str());
    else
      GetISystem()->GetIConsole()->PrintLine("Shader type[%s] not present", type);

    return true;
  }
  return false;
}
bool ShaderCommand::enumerate(CommandDesc& cd)
{
  MaterialManager::instance()->EnumShaders(this);
  return true;
}
#endif
void AddInternalCommands(ISystem *pSystem)
{
  //auto pConsole = gEnv->pConsole;
  //pConsole->AddCommand("shader", new ShaderCommand(pSystem), "Set shader to object and reload shader");

}
