#include "ProjectManager.hpp"
#include <BlackBox/Core/Path.hpp>
#include <BlackBox/System/ConsoleRegistration.h>

CProjectManager::CProjectManager()
#if 0
	: m_sys_project(nullptr)
#endif
	: m_sys_game_name(nullptr)
	, m_sys_dll_game(nullptr)
	, m_sys_game_folder(nullptr)
{
}

const char* CProjectManager::GetCurrentProjectName() const
{
	return m_sys_game_name->GetString();
}

void CProjectManager::RegisterCVars()
{
	// Legacy
	m_sys_game_name = REGISTER_STRING("sys_game_name", "BlackBox", VF_DUMPTODISK, "Specifies the name to be displayed in the Launcher window title bar");
	m_sys_dll_game = REGISTER_STRING("sys_dll_game", "", VF_REQUIRE_APP_RESTART | VF_DUMPTODISK, "Specifies the game DLL to load");
	m_sys_game_folder = REGISTER_STRING("sys_game_folder", "", VF_REQUIRE_APP_RESTART, "Specifies the game folder to read all data from. Can be fully pathed for external folders or relative path for folders inside the root.");
}

bool CProjectManager::ParseProjectFile()
{
	RegisterCVars();

	const char* szEngineRootDirectory = gEnv->pSystem->GetRootFolder();

	// Assign project file path
	{
		string projectFile = PathUtil::GetProjectFile();
		if (projectFile.empty())
		{
			CryLogAlways("\nRunning BlackBox without a project!");
			CryLogAlways("	Using Engine Folder %s", szEngineRootDirectory);

			m_sys_game_name->Set("BlackBox - No Project");
			// Specify an assets directory despite having no project, this is to prevent CryPak scanning engine root
			m_sys_game_folder->Set("Assets");
			return false;
		}
	}
	return true;
}
