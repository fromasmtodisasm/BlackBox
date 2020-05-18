#include <Game.hpp>

#include <BlackBox/Core/IMarkers.hpp>
#include <BlackBox/Core/Utils.hpp>
//#include <BlackBox/Renderer/SceneManager.hpp>
#include <BlackBox/Scene/IScene.hpp>
#include <BlackBox/System/ConsoleCommands.hpp>
#include <BlackBox/World/IWorld.hpp>

#if 0
#ifdef _WIN32
#include <process.h>
#endif

bool MyExec(char* FileName, LPSTR cmd, HANDLE* handle)
{
#ifdef _WIN32
  //DWORD res;
  STARTUPINFO         si;
  PROCESS_INFORMATION pi;
  memset(&si, 0, sizeof(STARTUPINFO));
  si.cb = sizeof(si);
  LPCH env = GetEnvironmentStrings();
  //si.wShowWindow = true;
  //si.dwFlags = STARTF_USESHOWWINDOW;
  if (!CreateProcess(FileName,
    cmd,
    NULL,
    NULL,
    FALSE,
    0,
    //NULL,
    env,
    NULL,
    &si,
    &pi))
  {
    auto er = GetLastError();
    return false;
  }
  else
  {
    *handle = pi.hProcess;
    return true;
  }
#else
  return false;
#endif
}

class BaseGameCommand : public BaseCommand
{
protected:
  CGame* game;
public:
  BaseGameCommand(CGame* game) : game(game), BaseCommand()
  {

  }

};

class LastCommand : public BaseGameCommand
{
public:
  LastCommand(CGame* game);
  ~LastCommand();
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    bool result = false;
    if (cd.history->size() == 0)
      return false;
    if (cd.args.size() == 1)
    {
      for (auto i = cd.history->size() - 1 - static_cast<size_t>(_wtof(cd.args[0].c_str())); i < cd.history->size();)
      {
        result = true;
        GetISystem()->GetIConsole()->ExecuteString(wstr_to_str(cd.history->back()).c_str());
      }
    }
    else
    {
      result = true;
      GetISystem()->GetIConsole()->ExecuteString(wstr_to_str(cd.history->back()).c_str());
    }
    return result;
  }
};

LastCommand::LastCommand(CGame* game) : BaseGameCommand(game)
{
}

LastCommand::~LastCommand()
{
}

class ClearCommand : public BaseGameCommand
{
public:
  ClearCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    GetISystem()->GetIConsole()->Clear();
    return true;
  }
};

ClearCommand::ClearCommand(CGame* game) : BaseGameCommand(game)
{
}

//*******************************************************
class GotoCommand : public BaseGameCommand
{
public:
  GotoCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    if (cd.args.size() == 1)
    {
      std::wstring mode = cd.args[0];
      if (mode == L"MENU")
        game->gotoMenu();
      else if (mode == L"FLY")
        game->gotoFly();
      else if (mode == L"EDIT")
        game->gotoEdit();
      else if (mode == L"FPS")
        game->gotoGame();
      return true;
    }
    return false;
  }
};

GotoCommand::GotoCommand(CGame* game) : BaseGameCommand(game)
{
}
//*******************************************************
//*******************************************************
class QuitCommand : public BaseGameCommand
{
public:
  QuitCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    game->Stop();
    return true;
  }
};

QuitCommand::QuitCommand(CGame* game) : BaseGameCommand(game)
{
}
//*******************************************************
class MoveCommand : public BaseGameCommand
{
  IWorld* m_World;
public:
  MoveCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    auto obj = m_World->GetActiveScene()->selectedObject()->second;
    auto args_it = cd.args.begin();
    if (cd.args.size() >= 3)
    {
      if (cd.args.size() == 4)
      {
        std::string name = wstr_to_str(*args_it++);
        obj = m_World->GetActiveScene()->getObject(name);
      }
      auto pos = unpack_vector(args_it);
      obj->move(pos);
      return true;
    }
    return false;
  }
};

MoveCommand::MoveCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}
//*******************************************************
class ScaleCommand : public BaseGameCommand
{
  IWorld* m_World;
public:
  ScaleCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    auto obj = m_World->GetActiveScene()->selectedObject()->second;
    auto args_it = cd.args.begin();
    if (cd.args.size() >= 3)
    {
      if (cd.args.size() == 4)
      {
        std::string name = wstr_to_str(*args_it++);
        obj = m_World->GetActiveScene()->getObject(name);
      }
      auto scale = unpack_vector(args_it);
      obj->scale(scale);
      return true;
    }
    return false;
  }
};

ScaleCommand::ScaleCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}
//*******************************************************
class RotateCommand : public BaseGameCommand
{
  IWorld* m_World;
public:
  RotateCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    if (cd.args.size() >= 4 && cd.args.size() <= 5)
    {
      auto args_it = cd.args.begin();
      auto obj = m_World->GetActiveScene()->selectedObject()->second;
      if (cd.args.size() == 5) {
        std::string name = wstr_to_str(*args_it++);
        obj = m_World->GetActiveScene()->getObject(name);
      }
      if (obj != nullptr)
      {
        glm::vec3 angles(static_cast<float>(_wtof((*args_it++).c_str())));
        glm::vec3 vector = unpack_vector(args_it);
        if (vector[0] != 0.0f) angles[0] += obj->m_transform.rotation[0];
        if (vector[1] != 0.0f) angles[1] += obj->m_transform.rotation[1];
        if (vector[2] != 0.0f) angles[2] += obj->m_transform.rotation[2];
        obj->rotateX(angles.x);
        obj->rotateY(angles.y);
        obj->rotateZ(angles.z);
        return true;
      }
    }
    return false;
  }
};

RotateCommand::RotateCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}
//*******************************************************
class SelectCommand : public BaseGameCommand
{
  IWorld* m_World;
public:
  SelectCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    std::string name = wstr_to_str(cd.args[0]);
    return m_World->GetActiveScene()->selectObject(name);
  }
};

SelectCommand::SelectCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}
//*******************************************************
class ExecCommand : public BaseGameCommand, public IWorkerCommand
{
  IWorld* m_World;
  IConsole* console;
  int wait_cnt = 0;
#ifdef _WIN32
  HANDLE process;
#endif
public:
  ExecCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    if (cd.args.size() == 1)
    {
      std::string name = wstr_to_str(cd.args[0]);
      if (name.substr(name.length() - 4, name.length()) == ".lua")
      {
        GetISystem()->GetIScriptSystem()->ExecuteFile(name.c_str());
      }
      else
      {
        console->ExecuteFile(name.c_str());
      }
      return true;
    }
    else if (cd.get(0) == L"os")
    {
      auto command = cd.get(1);
      std::string args;
      if (cd.args.size() > 2)
      {
        for (int i = 1; i < cd.args.size(); i++)
        {
          args += "\"" + wstr_to_str(cd.get(i)) + "\" ";
        }
      }
#ifdef _WIN32
      if (MyExec(const_cast<char*>(wstr_to_str(command).c_str()), const_cast<char*>(args.c_str()), &process))
      {
        console->AddWorkerCommand(this);
      }
#endif
    }
    return false;
  }

  // Унаследовано через IWorkerCommand
  virtual bool OnUpdate() override
  {
#ifdef _WIN32
    auto res = WaitForSingleObject(process, 0);
    if (res == WAIT_OBJECT_0)
    {
      console->PrintLine("Finish");
      console->RemoveWorkerCommand(this);
    }
    if ((wait_cnt % 60) == 0)
      console->PrintLine("wait");
    wait_cnt++;
#endif
    return false;
  }
};

ExecCommand::ExecCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
  console = GetISystem()->GetIConsole();
}
//*******************************************************
class CameraCommand : public BaseGameCommand
{
  IWorld* m_World;
public:
  CameraCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    if (cd.args.size() > 0)
    {
      if (cd.args[0] == L"move")
        return move(cd);
      if (cd.args[0] == L"lookat")
        return lookat(cd);
    }
    return false;
  }
  bool lookat(CommandDesc& cd);
  bool move(CommandDesc& cd);
};

CameraCommand::CameraCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}

bool CameraCommand::lookat(CommandDesc& cd)
{
  auto object = m_World->GetActiveScene()->selectedObject();
  auto camera = m_World->GetActiveScene()->getCurrentCamera();

  return false;
}

bool CameraCommand::move(CommandDesc& cd)
{
  if (cd.args.size() == 4)
  {
    auto pos = unpack_vector((cd.args.begin()++)++);
    auto camera = m_World->GetActiveScene()->getCurrentCamera();
    camera->SetPos(pos);
    return true;
  }
  return false;
}
//*******************************************************
class SceneCommand : public BaseGameCommand
{
  IWorld* m_World;
  CGame* m_Game;
public:
  SceneCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    if (cd.args.size() > 0)
    {
      if (cd.args[0] == L"load")
        return load(cd);
      if (cd.args[0] == L"save")
        return save(cd);
      if (cd.args[0] == L"active")
        return activate(cd);
    }
    return false;
  }
  bool load(CommandDesc& cd);
  bool save(CommandDesc& cd);
  bool activate(CommandDesc& cd);
};

SceneCommand::SceneCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}
bool SceneCommand::load(CommandDesc& cd)
{
  std::string path = wstr_to_str(cd.args[1]);
  return game->loadScene(path);
}
bool SceneCommand::save(CommandDesc& cd)
{
  if (cd.args.size() >= 2)
  {
    std::string path = wstr_to_str(cd.args[1]);
    {
      std::string as = "";
      if (cd.args.size() == 3)
        as = wstr_to_str(cd.args[2]);
      game->saveScene(path, as);
      return true;
    }
  }
  return false;
}
bool SceneCommand::activate(CommandDesc& cd)
{
  IScene* scene;
  std::string name = wstr_to_str(cd.args[1]);
  if ((scene = SceneManager::instance()->getScene(name, game)) != nullptr)
  {
    //=====================
    game->getWorld()->SetScene(scene);
    game->setCamera(scene->getCurrentCamera());
    game->setPlayer((CPlayer*)scene->getObject("MyPlayer"));
    //=====================
    //m_World->setScene(scene);
    //game->setCamera(scene->getCamera());
    //m_World->setCamera(m_camera1);
    //initPlayer();
    return true;
  }
  return false;
}
//*******************************************************
class TagPointCommand : public BaseGameCommand
{
  IWorld* m_World;
public:
  TagPointCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    auto obj = m_World->GetActiveScene()->selectedObject()->second;
    auto args_it = cd.args.begin();
    if (cd.args.size() == 2)
    {
      auto subcmd = wstr_to_str(cd.get(0));
      if (subcmd == "create")
      {
        auto tag = wstr_to_str(cd.get(1));
        if (tag.length() == 0) return false;
        game->CreateTagPoint(tag, m_World->GetActiveScene()->getCurrentCamera()->GetPos(), m_World->GetActiveScene()->getCurrentCamera()->GetAngles());
        return true;
      }
      else if (subcmd == "goto")
      {
        auto tag = wstr_to_str(cd.get(1));
        if (tag.length() == 0) return false;
        ITagPoint* tag_point = game->GetTagPoint(tag);
        if (tag_point == nullptr)
        {
          GetISystem()->GetIConsole()->PrintLine("Error, tagpoint [%s] not exist", tag.c_str());
          return false;
        }
        Vec3 pos, ang;
        tag_point->GetPos(pos);
        tag_point->GetAngles(ang);
        m_World->GetActiveScene()->getCurrentCamera()->SetPos(pos);
        m_World->GetActiveScene()->getCurrentCamera()->SetAngles(ang);
        m_World->GetActiveScene()->getCurrentCamera()->updateCameraVectors();
        return true;
      }

      auto pos = unpack_vector(args_it);
      obj->move(pos);
      return true;
    }
    return false;
  }
};

TagPointCommand::TagPointCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}
//*******************************************************
class ObjDumpCommand : public BaseGameCommand
{
  IWorld* m_World;
public:
  ObjDumpCommand(CGame* game);
private:
  // Inherited via IConsoleCommand
  virtual bool execute(CommandDesc& cd) override
  {
    /*
    if (cd.args.size() > 0)
    {
      if (cd.args[0] == L"reload")
        return reload(cd);
      if (cd.args[0] == L"set")
        return move(cd);
    }
    */
    FILE* fp;

    auto obj = m_World->GetActiveScene()->selectedObject()->second;
    auto path = obj->m_path;
    auto pos = path.find_last_of('.');
    if (pos == path.npos)
      return false;

    path = path.replace(pos, path.size(), ".bin");
    fp = fopen(path.c_str(), "wb");
    fprintf(fp, "bbg");
    //obj->m_Mesh

    fclose(fp);
    return false;
  }
};
//*******************************************************
#endif
void CGame::initCommands()
{
#if 0
  m_Console->AddCommand("last", new LastCommand(this));
  m_Console->AddCommand("clear", new ClearCommand(this), "Clear command buffer");
  m_Console->AddCommand("goto", new GotoCommand(this), "Change mode [FPS/FLY/MENU/EDIT]");
  m_Console->AddCommand("quit", new QuitCommand(this));
  m_Console->AddCommand("move", new MoveCommand(this));
  m_Console->AddCommand("scale", new ScaleCommand(this));
  m_Console->AddCommand("rotate", new RotateCommand(this));
  m_Console->AddCommand("select", new SelectCommand(this));
  m_Console->AddCommand("exec", new ExecCommand(this), "Load end execute scripts");
  //m_Console->AddCommand("material", new MaterialCommand(this));
  //m_Console->AddCommand("shader", new ShaderCommand(this), "Set shader to object and reload shader");
  m_Console->AddCommand("camera", new CameraCommand(this));
  m_Console->AddCommand("scene", new SceneCommand(this), "Scene managment");
  m_Console->AddCommand("tagpoint", new TagPointCommand(this), "TagPoint managment, tagpoint create|goto \"name\"");
  m_Console->AddCommand("objdump", new ObjDumpCommand(this), "Dump model to disk in binary");
#endif
}
#if 0
void CGame::initVariables()
{
}

ObjDumpCommand::ObjDumpCommand(CGame* game) : BaseGameCommand(game)
{
  m_World = game->getWorld();
}
#endif
