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
	m_sys_game_name   = REGISTER_STRING("sys_game_name", "BlackBox", VF_DUMPTODISK, "Specifies the name to be displayed in the Launcher window title bar");
	m_sys_dll_game    = REGISTER_STRING("sys_dll_game", "", VF_REQUIRE_APP_RESTART | VF_DUMPTODISK, "Specifies the game DLL to load");
	m_sys_game_folder = REGISTER_STRING("sys_game_folder", "", VF_REQUIRE_APP_RESTART, "Specifies the game folder to read all data from. Can be fully pathed for external folders or relative path for folders inside the root.");
}

bool CProjectManager::ParseProjectFile()
{
	RegisterCVars();

	const char* szEngineRootDirectory = Env::System()->GetRootFolder();

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
		string extension = PathUtil::GetExt(projectFile.c_str());
		if (extension.empty())
		{
			projectFile = PathUtil::ReplaceExtension(projectFile.c_str(), "project");
		}

		if (PathUtil::IsRelativePath(projectFile.c_str()))
		{
			projectFile = PathUtil::Make(Env::System()->GetRootFolder(), projectFile.c_str());
		}

		m_project.filePath = projectFile;
	}
	if (Env::ScriptSystem()->ExecuteFile(m_project.filePath.c_str()))
	{
		m_project.rootDirectory = PathUtil::RemoveSlash(PathUtil::ToUnixPath(PathUtil::GetPathWithoutFilename(m_project.filePath)));

		const char*       sys_dll_game{};
		SmartScriptObject project(Env::ScriptSystem());
		if (Env::ScriptSystem()->GetGlobalValue("project", project))
		{
			SmartScriptObject info(Env::ScriptSystem());
			project->GetValue("info", info);
			if (info->GetValue("name", sys_dll_game))
			{
				CryLog("sys_dll_game: %s", sys_dll_game);
				Env::Console()->LoadConfigVar("sys_dll_game", sys_dll_game);
				m_project.name           = sys_dll_game;
				m_project.assetDirectory = "Assets";
			}
			else
			{
				return false;
			}
			// m_sys_game_name->Set("BlackBox - No Project");
		}
		// Create the full path to the asset directory
		m_project.assetDirectoryFullPath = PathUtil::Make(m_project.rootDirectory, m_project.assetDirectory);
// Ensure compatibility with all supported platform filesystems
#if 0
		m_project.assetDirectoryFullPath.MakeLower();
#endif

		// Does directory exist
		if (!CryDirectoryExists(m_project.assetDirectoryFullPath.c_str()))
		{
#if 0
			EQuestionResult result = CryMessageBox(string().Format("Attempting to start the engine with non-existent asset directory %s!\nDo you want to create it?", m_project.assetDirectoryFullPath.c_str()), "Non-existent asset directory", eMB_YesCancel);
			if (result == eQR_Cancel)
			{
				CryLogAlways("\tNon-existent asset directory %s detected, user opted to quit", m_project.assetDirectoryFullPath.c_str());
				return false;
			}
#endif

			CryCreateDirectory(m_project.assetDirectoryFullPath.c_str());
		}

		// Set the legacy game folder and name
		if (strlen(m_sys_game_folder->GetString()) == 0)
		{
			m_sys_game_folder->Set(m_project.assetDirectory.c_str());
		}
		m_sys_game_name->Set(m_project.name.c_str());

		for (const std::pair<string, string>& consoleVariablePair : m_project.consoleVariables)
		{
			Env::Console()->LoadConfigVar(consoleVariablePair.first.c_str(), consoleVariablePair.second.c_str());
		}

		for (const std::pair<string, string>& consoleCommandPair : m_project.consoleCommands)
		{
			Env::Console()->LoadConfigCommand(consoleCommandPair.first.c_str(), consoleCommandPair.second.c_str());
		}

		auto gameDllIt = m_project.legacyGameDllPaths.find("any");

		if (gameDllIt == m_project.legacyGameDllPaths.end())
		{
			gameDllIt = m_project.legacyGameDllPaths.find("win_x64");
		}
		string legacyGameDllPath;

		// Set legacy Game DLL
		if (gameDllIt != m_project.legacyGameDllPaths.end())
		{
			legacyGameDllPath = gameDllIt->second;

			m_sys_dll_game->Set(legacyGameDllPath.c_str());
		}
		else
		{
			//legacyGameDllPath =
		}

#if 0
		Env::Console()->LoadConfigVar("sys_dll_game", legacyGameDllPath.c_str());
#else
		auto path = PathUtil::Make(m_project.rootDirectory, string("bin/win_x64"));
		path      = PathUtil::Make(path, m_project.name);
		Env::Console()->LoadConfigVar("sys_dll_game", path.c_str());
#endif

#ifdef BB_PLATFORM_WINDOWS
	#if 0
		SetDllDirectoryW(CryStringUtils::UTF8ToWStr(m_project.rootDirectory));
	#else
		SetDllDirectoryA(m_project.rootDirectory.c_str());
	#endif
#endif

#ifndef BB_PLATFORM_ORBIS
		CrySetCurrentWorkingDirectory(m_project.rootDirectory.c_str());
#endif
	}
	else if (m_sys_game_folder->GetString()[0] == '\0')
	{
		// No project folder found, and no legacy context to migrate from.
		m_project.filePath.clear();
	}
	if (m_project.filePath.empty())
	{
		//if (gEnv->bTesting)
		//{
		//	CryLogAlways("\nRunning engine in unit testing mode without project");

		//	// Create a temporary asset directory, as some systems rely on an assets directory existing.
		//	m_project.assetDirectory		 = "NoAssetFolder";
		//	m_project.assetDirectoryFullPath = PathUtil::Make(szEngineRootDirectory, m_project.assetDirectory);
		//	m_project.assetDirectoryFullPath.MakeLower();

		//	CryCreateDirectory(m_project.assetDirectoryFullPath.c_str());

		//	m_sys_game_folder->Set(m_project.assetDirectory.c_str());

		//	return true;
		//}
		//else
		{
			CryMessageBox("Attempting to start the engine without a project!\nPlease use a .cryproject file!", "Engine initialization failed", eMB_Error);
			return false;
		}
	}
	// Detect running without asset directory
	else if (m_project.assetDirectory.empty())
	{
		//if (!gEnv->bTesting)
		{
			EQuestionResult result = CryMessageBox("Attempting to start the engine without an asset directory!\nContinuing will put the engine into a readonly state where changes can't be saved, do you want to continue?", "No Assets directory", eMB_YesCancel);
			if (result == eQR_Cancel)
			{
				CryLogAlways("\tNo asset directory detected, user opted to quit");
				return false;
			}
		}

		// Engine started without asset directory, we have to create a temporary directory in this case
		// This is done as many systems rely on checking for files in the asset directory, without one they will search the root or even the entire drive.
		m_project.assetDirectory = "NoAssetFolder";
		CryLogAlways("\tSkipped use of assets directory");
	}

	CryLogAlways("\nProject %s", GetCurrentProjectName());
	CryLogAlways("\tUsing Project Folder %s", GetCurrentProjectDirectoryAbsolute());
	CryLogAlways("\tUsing Engine Folder %s", szEngineRootDirectory);
#if 0
	CryLogAlways("\tUsing Asset Folder %s", GetCurrentAssetDirectoryAbsolute());
#endif

	return true;
}

const char* CProjectManager::GetCurrentProjectDirectoryAbsolute() const
{
	return m_project.rootDirectory.c_str();
}
