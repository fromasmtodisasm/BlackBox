#include <BlackBox/Game/Game.hpp>
#include <BlackBox/Utils.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/TechniqueManager.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/IMarkers.hpp>
#include <process.h>

bool MyExec(char* FileName, LPSTR cmd, HANDLE *handle)
{

	DWORD res;
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
}

class BaseCommand : public IConsoleCommand
{
protected:
	CGame* game;
	glm::vec3 unpack_vector(std::vector<std::wstring>::iterator it, int size = 3)
	{
			glm::vec3 pos;
			pos[0] = static_cast<float>(_wtof(it->c_str()));
			it++;
			pos[1] = static_cast<float>(_wtof(it->c_str()));
			if (size == 3)
			{
				it++;
				pos[2] =static_cast<float>(_wtof(it->c_str()));
			}
			return pos;
	};
public:
	BaseCommand(CGame *game);
	~BaseCommand();

private:

};

BaseCommand::BaseCommand(CGame *game) : game(game)
{
}

BaseCommand::~BaseCommand()
{
}

class LastCommand : public BaseCommand 
{
public:
	LastCommand(CGame *game);
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
			for (auto i = cd.history->size() - 1 - _wtoi(cd.args[0].c_str()); i < cd.history->size();)
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

LastCommand::LastCommand(CGame *game) : BaseCommand(game)
{
}

LastCommand::~LastCommand()
{
}

class ClearCommand : public BaseCommand 
{
public:
	ClearCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		GetISystem()->GetIConsole()->Clear();
		return true;
	}
};

ClearCommand::ClearCommand(CGame *game) : BaseCommand(game)
{
}

//*******************************************************
class GotoCommand : public BaseCommand 
{
public:
	GotoCommand(CGame *game);
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

GotoCommand::GotoCommand(CGame *game) : BaseCommand(game)
{
}
//*******************************************************
//*******************************************************
class VsyncCommand : public BaseCommand 
{
public:
	VsyncCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		if (cd.args.size() == 1)
		{
			sf::RenderWindow *rw = reinterpret_cast<sf::RenderWindow *>(game->getWindow()->getHandle());
			std::wstring mode = cd.args[0];
			if (mode == L"on")
				rw->setVerticalSyncEnabled(true); 
			else if (mode == L"off")
				rw->setVerticalSyncEnabled(false); 
			return true;
		}
		return false;
	}
};

VsyncCommand::VsyncCommand(CGame *game) : BaseCommand(game)
{
}
//*******************************************************
//*******************************************************
class QuitCommand : public BaseCommand 
{
public:
	QuitCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		game->Stop();
		return true;
	}
};

QuitCommand::QuitCommand(CGame *game) : BaseCommand(game)
{
}
//*******************************************************
class MoveCommand : public BaseCommand 
{
	World* m_World;
public:
	MoveCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		auto obj = m_World->getActiveScene()->selectedObject()->second;
		auto args_it = cd.args.begin();
		if (cd.args.size() >= 3)
		{
			if (cd.args.size() == 4)
			{
				std::string name = wstr_to_str(*args_it++);
				obj = m_World->getActiveScene()->getObject(name);
			}
			auto pos = unpack_vector(args_it);
			obj->move(pos);
			return true;
		}
		return false;
	}
};

MoveCommand::MoveCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class RotateCommand : public BaseCommand 
{
	World* m_World;
public:
	RotateCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		if (cd.args.size() >= 4 && cd.args.size() <= 5)
		{
			auto args_it = cd.args.begin();
			auto obj = m_World->getActiveScene()->selectedObject()->second;
			if (cd.args.size() == 5) {
				std::string name = wstr_to_str(*args_it++);
				obj = m_World->getActiveScene()->getObject(name);
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

RotateCommand::RotateCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class SelectCommand : public BaseCommand 
{
	World* m_World;
public:
	SelectCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		std::string name = wstr_to_str(cd.args[0]);
		return m_World->getActiveScene()->selectObject(name);
	}
};

SelectCommand::SelectCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class WireframeCommand : public BaseCommand 
{
	World* m_World;
public:
	WireframeCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		int mode = m_World->getActiveScene()->selectedObject()->second->getRenderMode();
		if (mode == GL_FILL) mode = GL_LINE;
		else if (mode == GL_LINE) mode = GL_FILL;
		auto obj = m_World->getActiveScene()->selectedObject();
		obj->second->setRenderMode(mode);
		GetISystem()->GetIConsole()->PrintLine("Name of wired object %s\n", obj->first.c_str());
		return true;
	}
};

WireframeCommand::WireframeCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class ExecCommand : public BaseCommand, public IWorkerCommand
{
	World* m_World;
	IConsole* console;
	int wait_cnt = 0;
	HANDLE process;
public:
	ExecCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		if (cd.args.size() == 1)
		{
			std::string name = wstr_to_str(cd.args[0]);
			console->ExecuteFile(name.c_str());
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
			if (MyExec(const_cast<char*>(wstr_to_str(command).c_str()), const_cast<char*>(args.c_str()), &process))
			{
				console->AddWorkerCommand(this);
			}
		}
		return false;
	}

	// Унаследовано через IWorkerCommand
	virtual bool OnUpdate() override
	{
		auto res = WaitForSingleObject(process, 0);
		if (res == WAIT_OBJECT_0)
		{
			console->PrintLine("Finish");
			console->RemoveWorkerCommand(this);
		}
		if ((wait_cnt % 60) == 0)
			console->PrintLine("wait");
		wait_cnt++;
		return false;
	}
};

ExecCommand::ExecCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
	console = GetISystem()->GetIConsole();
}
//*******************************************************
class MaterialCommand : public BaseCommand 
{
	World* m_World;
public:
	MaterialCommand(CGame *game);
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
			game->getWorld()->getActiveScene()->selectedObject()->second->setMaterial(m);
			return true;
		}
		return false;
	}
};

MaterialCommand::MaterialCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class ShaderCommand : public BaseCommand, public IMaterialShaderSink
{
	World* m_World;
public:
	ShaderCommand(CGame *game);
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

ShaderCommand::ShaderCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
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
		auto s = MaterialManager::instance()->getProgram(wstr_to_str(cd.args[1]));
		game->getWorld()->getActiveScene()->selectedObject()->second->getMaterial()->program = s;
		return true;
	}
	return false;
}
bool ShaderCommand::dump(CommandDesc& cd)
{
	if (cd.args.size() == 2)
	{
		auto s = MaterialManager::instance()->getProgram(wstr_to_str(cd.args[1]));
		if (s == nullptr)
			return false;
		s->dump();
		GetISystem()->GetIConsole()->ExecuteString("exec os @EDITOR dump.bin");
		return true;
	}
	return false;
}
bool ShaderCommand::edit(CommandDesc& cd)
{
	if (cd.args.size() == 3)
	{
		auto s = MaterialManager::instance()->getProgram(wstr_to_str(cd.args[2]));
		if (s == nullptr)
			return false;
		std::string shader_name;
		std::string type = wstr_to_str(cd.get(1));
		if (type == "vs")
			shader_name = s->m_Vertex.name;
		else if (type == "fs")
			shader_name = s->m_Fragment.name;
		else if (type == "gs")
			shader_name = s->m_Geometry.name;
		else if (type == "cs")
			shader_name = s->m_Compute.name;
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
//*******************************************************
class CameraCommand : public BaseCommand 
{
	World* m_World;
public:
	CameraCommand(CGame *game);
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

CameraCommand::CameraCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}

bool CameraCommand::lookat(CommandDesc& cd)
{
	auto object = m_World->getActiveScene()->selectedObject();
	auto camera = m_World->getActiveScene()->getCurrentCamera();

	return false;
}

bool CameraCommand::move(CommandDesc& cd)
{
	
	if (cd.args.size() == 4)
	{
		auto pos = unpack_vector((cd.args.begin()++)++);
		auto camera = m_World->getActiveScene()->getCurrentCamera();
		camera->setPosition(pos);
		return true;
	}
	return false;

}
//*******************************************************
class SceneCommand : public BaseCommand 
{
	World* m_World;
public:
	SceneCommand(CGame *game);
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

SceneCommand::SceneCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
bool SceneCommand::load(CommandDesc& cd)
{
	//MessageBox(NULL, "Save scene", "scene name", MB_OKCANCEL);
	Scene *scene;
	std::string path = wstr_to_str(cd.args[1]);
  SceneManager::instance()->removeScene(path);
	if ((scene = SceneManager::instance()->getScene(path)) != nullptr)
	{
		/*
		if (game->initPlayer())
			game->gotoGame();
		*/
		//FrameBufferObject *sceneBuffer = new FrameBufferObject(FrameBufferObject::buffer_type::SCENE_BUFFER, game->getWindow()->getWidth(), game->getWindow()->getHeight());
		//FrameBufferObject *sceneBuffer = new FrameBufferObject(FrameBufferObject::buffer_type::HDR_BUFFER, game->getWindow()->getWidth(), game->getWindow()->getHeight());
		//sceneBuffer->create();
		//scene->setRenderTarget(sceneBuffer);
    m_World->setScene(scene);
    auto tech = TechniqueManager::get("hdr");
    tech->Init(m_World->getActiveScene(), nullptr);
    scene->setTechnique(tech);

		//scene->setCamera("main", new CCamera());
		CPlayer *player = static_cast<CPlayer*>(scene->getObject("MyPlayer"));
		player->attachCamera(scene->getCurrentCamera());
		player->setGame(game);
    game->setPlayer(player);
    return true;
	}
	return false;
}
bool SceneCommand::save(CommandDesc& cd)
{
  if (cd.args.size() >= 2)
  {
    std::string path = wstr_to_str(cd.args[1]);
    if (SceneManager::instance()->exist(path))
    {
      auto scene = SceneManager::instance()->getScene(path);
      std::string as = "";
      if (cd.args.size() == 3)
        as = wstr_to_str(cd.args[2]);
      return scene->save(as);
    }
  }
	return false;
}
bool SceneCommand::activate(CommandDesc& cd)
{
	Scene *scene;
	std::string name = wstr_to_str(cd.args[1]);
	if ((scene = SceneManager::instance()->getScene(name)) != nullptr)
	{
		//=====================
		game->getWorld()->setScene(scene);
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
class TagPointCommand : public BaseCommand 
{
	World* m_World;
public:
	TagPointCommand(CGame *game);
private:
	// Inherited via IConsoleCommand
	virtual bool execute(CommandDesc& cd) override
	{
		auto obj = m_World->getActiveScene()->selectedObject()->second;
		auto args_it = cd.args.begin();
		if (cd.args.size() == 2)
		{
      auto subcmd = wstr_to_str(cd.get(0));
      if (subcmd == "create")
      {
        auto tag = wstr_to_str(cd.get(1));
        if (tag.length() == 0) return false;
        game->CreateTagPoint(tag, m_World->getActiveScene()->getCurrentCamera()->getPosition(), m_World->getActiveScene()->getCurrentCamera()->getRotation());
        return true;
      }
      else if (subcmd == "goto")
      {
        auto tag = wstr_to_str(cd.get(1));
        if (tag.length() == 0) return false;
        ITagPoint *tag_point = game->GetTagPoint(tag);
        if (tag_point == nullptr)
        {
          GetISystem()->GetIConsole()->PrintLine("Error, tagpoint [%s] not exist", tag.c_str());
          return false;
        }
        Vec3 pos, ang;
        tag_point->GetPos(pos);
        tag_point->GetAngles(ang);
        m_World->getActiveScene()->getCurrentCamera()->setPosition(pos);
        m_World->getActiveScene()->getCurrentCamera()->setRotation(ang);
        m_World->getActiveScene()->getCurrentCamera()->updateCameraVectors();
        return true;
      }
			
			auto pos = unpack_vector(args_it);
			obj->move(pos);
			return true;
		}
		return false;
	}
};

TagPointCommand::TagPointCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class ObjDumpCommand : public BaseCommand 
{
	World* m_World;
public:
	ObjDumpCommand(CGame *game);
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

		auto obj = m_World->getActiveScene()->selectedObject()->second;
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

void CGame::initCommands()
{
	m_Console->AddCommand("last", new LastCommand(this));
	m_Console->AddCommand("clear", new ClearCommand(this), "Clear command buffer");
	m_Console->AddCommand("goto", new GotoCommand(this), "Change mode [FPS/FLY/MENU/EDIT]");
	m_Console->AddCommand("vsync", new VsyncCommand(this), "Enable/Disable vsync [on/off]");
	m_Console->AddCommand("quit", new QuitCommand(this));
	m_Console->AddCommand("move", new MoveCommand(this));
	m_Console->AddCommand("rotate", new RotateCommand(this));
	m_Console->AddCommand("select", new SelectCommand(this));
	m_Console->AddCommand("wire", new WireframeCommand(this));
	m_Console->AddCommand("exec", new ExecCommand(this), "Load end execute scripts");
	m_Console->AddCommand("material", new MaterialCommand(this));
	m_Console->AddCommand("shader", new ShaderCommand(this), "Set shader to object and reload shader");
	m_Console->AddCommand("camera", new CameraCommand(this));
	m_Console->AddCommand("scene", new SceneCommand(this), "Scene managment");
	m_Console->AddCommand("tagpoint", new TagPointCommand(this), "TagPoint managment");
	m_Console->AddCommand("objdump", new ObjDumpCommand(this), "Dump model to disk in binary");
}

void CGame::initVariables()
{
  r_displayinfo = m_Console->CreateVariable("r_displayinfo", 1, 0, "Display info [1/0]");
  r_profile = m_Console->CreateVariable("r_profile", 1, 0, "Profile [1/0]");
  r_cap_profile = m_Console->CreateVariable("r_cap_profile", 1, 0, "Capture frame [1/0]");
}

ObjDumpCommand::ObjDumpCommand(CGame* game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
