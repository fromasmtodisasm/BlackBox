#include <BlackBox/Game/CGame.hpp>
#include <BlackBox/Utils.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <process.h>

class BaseCommand : public IEditCommand
{
protected:
	CGame* game;
	glm::vec3 unpack_vector(std::vector<std::wstring>::iterator it, int size = 3)
	{
			glm::vec3 pos;
			pos[0] = _wtof(it->c_str());
			it++;
			pos[1] = _wtof(it->c_str());
			if (size == 3)
			{
				it++;
				pos[2] = _wtof(it->c_str());
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
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		
		bool result = false;
		if (cd.history->size() == 0)
			return false;
		if (cd.args.size() == 1)
		{
			for (int i = cd.history->size() - 1 - _wtoi(cd.args[0].c_str()); i < cd.history->size();)
			{
				result = true;
				GetIEngine()->getIConsole()->ExecuteString(wstr_to_str(cd.history->back()).c_str());
			}
		}
		else
		{
			result = true;
			GetIEngine()->getIConsole()->ExecuteString(wstr_to_str(cd.history->back()).c_str());
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
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		cd.history->clear();
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
	// Inherited via IEditCommand
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
	// Inherited via IEditCommand
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
	// Inherited via IEditCommand
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
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		auto obj = m_World->getActiveScene()->selectedObject()->second;
		glm::vec3 pos;
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
	// Inherited via IEditCommand
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
				glm::vec3 angles(_wtof((*args_it++).c_str()));
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
	// Inherited via IEditCommand
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
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		int mode = m_World->getActiveScene()->selectedObject()->second->getRenderMode();
		if (mode == GL_FILL) mode = GL_LINE;
		else if (mode == GL_LINE) mode = GL_FILL;
		m_World->getActiveScene()->selectedObject()->second->setRenderMode(mode);
		return true;
	}
};

WireframeCommand::WireframeCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class ExecCommand : public BaseCommand 
{
	World* m_World;
public:
	ExecCommand(CGame *game);
private:
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		if (cd.args.size() == 1)
		{
			std::string name = wstr_to_str(cd.args[0]);
			auto res = spawnl(P_NOWAIT, name.c_str(), name.c_str(), nullptr);
			return true;
		}
		return false;
	}
};

ExecCommand::ExecCommand(CGame *game) : BaseCommand(game)
{
	m_World = game->getWorld();
}
//*******************************************************
class MaterialCommand : public BaseCommand 
{
	World* m_World;
public:
	MaterialCommand(CGame *game);
private:
	// Inherited via IEditCommand
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
class ShaderCommand : public BaseCommand 
{
	World* m_World;
public:
	ShaderCommand(CGame *game);
private:
	// Inherited via IEditCommand
	virtual bool execute(CommandDesc& cd) override
	{
		if (cd.args.size() > 0)
		{
			if (cd.args[0] == L"reload")
				return reload(cd);
			if (cd.args[0] == L"set")
				return move(cd);
		}
		return false;
	}
	bool reload(CommandDesc& cd);
	bool move(CommandDesc& cd);
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
//*******************************************************
class CameraCommand : public BaseCommand 
{
	World* m_World;
public:
	CameraCommand(CGame *game);
private:
	// Inherited via IEditCommand
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
	auto camera = m_World->getActiveScene()->getCamera();

	return false;
}

bool CameraCommand::move(CommandDesc& cd)
{
	
	if (cd.args.size() == 4)
	{
		auto pos = unpack_vector((cd.args.begin()++)++);
		auto camera = m_World->getActiveScene()->getCamera();
		camera->Position = pos;
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
	// Inherited via IEditCommand
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
	if ((scene = SceneManager::instance()->getScene(path)) != nullptr)
	{
		/*
		if (game->initPlayer())
			game->gotoGame();
		*/
		FrameBufferObject *sceneBuffer = new FrameBufferObject(game->getWindow()->getWidth(), game->getWindow()->getHeight());
		sceneBuffer->create();
		scene->setRenderTarget(sceneBuffer);
		scene->setCamera(new CCamera());
		CPlayer *player = static_cast<CPlayer*>(scene->getObject("MyPlayer"));
		player->attachCamera(scene->getCamera());
		player->setGame(game);
	}
	return false;
}
bool SceneCommand::save(CommandDesc& cd)
{
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
		game->setCamera(scene->getCamera());
		game->setPlayer((CPlayer*)scene->getObject("MyPlayer"));
		//=====================
		//m_World->setScene(scene);
		//game->setCamera(scene->getCamera());
		//m_World->setCamera(m_camera1);
		//initPlayer();
	}
	return false;
}
//*******************************************************

//*******************************************************

void CGame::initCommands()
{
	m_Console->AddCommand("last", new LastCommand(this));
	m_Console->AddCommand("clear", new ClearCommand(this));
	m_Console->AddCommand("goto", new GotoCommand(this));
	m_Console->AddCommand("vsync", new VsyncCommand(this), "Enable/Disable vsync [on/off]");
	m_Console->AddCommand("quit", new QuitCommand(this));
	m_Console->AddCommand("move", new MoveCommand(this));
	m_Console->AddCommand("rotate", new RotateCommand(this));
	m_Console->AddCommand("select", new SelectCommand(this));
	m_Console->AddCommand("wire", new WireframeCommand(this));
	m_Console->AddCommand("exec", new ExecCommand(this));
	m_Console->AddCommand("material", new MaterialCommand(this));
	m_Console->AddCommand("shader", new ShaderCommand(this));
	m_Console->AddCommand("camera", new CameraCommand(this));
	m_Console->AddCommand("scene", new SceneCommand(this));
}
