#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/System/System.hpp>

#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/Profiler/Drawer2D.h>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/3DEngine/3DEngine.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/System/Console.hpp>
#include "RemoteConsole/RemoteConsole.h"

#ifndef LINUX
#	include <BlackBox/System/File/CryPak.hpp>
#endif
#include <BlackBox/Core/Stream.hpp>
#include <BlackBox/System/HardwareMouse.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/IWindow.hpp>
//#include <BlackBox/System/NullLog.hpp>
#include <BlackBox/System/SystemEventDispatcher.hpp>
#include <BlackBox/System/VersionControl.hpp>
#include <BlackBox/World/IWorld.hpp>
#include <BlackBox/System/CVarOverrides.h>
//#include <BlackBox/Profiler/HP_Timer.h>
#include <SDL2/SDL.h>

#include "WindowsConsole.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <thread>

namespace fs = std::filesystem;

CSystem::CSystem(SSystemInitParams& m_startupParams)
	:
#if defined(SYS_ENV_AS_STRUCT)
	  m_env(m_env),
#endif
	  m_startupParams(m_startupParams),
	  cvGameName(nullptr),
	  m_Render(nullptr),
	  m_pConsole(nullptr),
	  //m_pInput(nullptr),
	  //m_pFont(nullptr),
	  m_pGame(nullptr),
	  m_pLog(nullptr),
	  m_pWindow(nullptr),
	  m_pScriptSystem(nullptr),
	  m_ScriptObjectConsole(nullptr),
	  m_pTextModeConsole(nullptr)
#if ENABLE_DEBUG_GUI
#endif
{
	m_pSystemEventDispatcher = new CSystemEventDispatcher(); // Must be first.
	if (m_pSystemEventDispatcher)
	{
		m_pSystemEventDispatcher->RegisterListener(this, "CSystem");
	}
	//////////////////////////////////////////////////////////////////////////
	// Initialize global environment interface pointers.
	m_env.pSystem = this;
	m_env.pTimer = &m_Time;

#if !defined(SYS_ENV_AS_STRUCT)
	gEnv = &m_env;
#endif
	InitThreadSystem();
}

CSystem::~CSystem()
{
	ShutDown();
}

void CSystem::Start()
{
	bool bRelaunch = m_env.IsDedicated();

	m_pGame->Run(bRelaunch);

	NOW	 = SDL_GetPerformanceCounter();
	LAST = 0;

	m_DeltaTime = 0.0;

	while (bRelaunch)
	{
		m_pGame->Release();
		m_pGame = CreateGame(nullptr);
		if (!m_pGame->Init(this, m_env.IsDedicated(), m_startupParams.bEditor, "Normal"))
			break;
		m_pGame->Run(bRelaunch);
	}
}

void CSystem::Release()
{
	delete this;
}

IRenderer* CSystem::GetIRenderer()
{
	return m_Render;
}

ILog* CSystem::GetILog()
{
	return m_pLog;
}

IConsole* CSystem::GetIConsole()
{
	return m_pConsole;
}

IInput* CSystem::GetIInput()
{
	return m_env.pInput;
}

IGame* CSystem::GetIGame()
{
	return m_pGame;
}

void CSystem::Quit()
{
	// clean up properly the console
	if (m_pTextModeConsole)
		m_pTextModeConsole->OnShutdown();

	m_pSystemEventDispatcher->OnSystemEvent(ESYSTEM_EVENT_FAST_SHUTDOWN, 0, 0);

	GetIRemoteConsole()->Stop();

	Release();

	exit(0);
}

IFont* CSystem::GetIFont()
{
	assert(0);
	return nullptr;
	//return new FreeTypeFont;
}

IWindow* CSystem::GetIWindow()
{
	return m_pWindow;
}

#if 0
IInputHandler* CSystem::GetIInputHandler()
{
  return m_InputHandler;
}
#endif

bool CSystem::ConfigLoad(const char* file)
{
	//m_pConsole->ExecuteFile(file);
	LoadConfiguration(file);

	return true;
}

void CSystem::RunMainLoop()
{
	for (;;)
	{
		if (!DoFrame())
		{
			break;
		}
	}
}

bool CSystem::DoFrame()
{
	return Update();
}

bool CSystem::CreateConsole()
{
	m_env.pConsole = m_pConsole = new CXConsole(*this);
	if (m_pConsole == nullptr)
		return false;
	return true;
}

void CSystem::ParseCMD()
{
	std::string cmd = m_startupParams.szSystemCmdLine;
	if (cmd.find("-nsightDebug") != std::string::npos)
	{
		m_pConsole->CreateVariable("nsightDebug", 1, VF_NULL, "Debuggin via Nsight Graphics");
	}
}

void CSystem::LoadScreen()
{
	if (m_env.IsDedicated())
	{
		return;
	}
	m_pConsole->Clear();
	m_pConsole->SetScrollMax(600);
	m_pConsole->ShowConsole(true);

	string sLoadingScreenTexture = string("loading.png");

	m_pConsole->SetLoadingImage(sLoadingScreenTexture.c_str());
	#if 0
	m_pConsole->ResetProgressBar(0x7fffffff);
	#endif
	//GetILog()->UpdateLoadingScreen("");	// just to draw the console
}

bool CSystem::InitScripts()
{
	m_ScriptObjectConsole = new CScriptObjectConsole();
	CScriptObjectConsole::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectScript = new CScriptObjectScript();
	CScriptObjectScript::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectRenderer = new CScriptObjectRenderer();
	CScriptObjectRenderer::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectConsole->Init(GetIScriptSystem(), m_pConsole);
	m_ScriptObjectScript->Init(GetIScriptSystem());

	return m_pScriptSystem->ExecuteFile("scripts/engine.lua");
}

void CSystem::SetWorkingDirectory(const std::string& path) const
{
	fs::current_path(fs::path(path));
}

void CSystem::LogCommandLine() const
{
	std::cout << "Log command line" << std::endl;
	for (int i = 0; i < m_pCmdLine->GetArgCount(); i++)
	{
		std::cout << "\t" << m_pCmdLine->GetArg(i)->GetValue() << std::endl;
	}
}

void CSystem::Tests()
{
#if 0
	auto plane_mesh = CreatePlane(4, 4);
	mesh = std::make_shared<std::vector<Mesh>>();
	mesh->push_back(*plane_mesh.get());
	obj = Object::getEmpty();
	obj->m_Mesh = mesh;
	obj->m_path = "";
#endif

	//SceneManager::instance()->currentScene()->addObject("subdiveded plane", obj);
	//Sleep(3000);
}

void CSystem::PollEvents()
{
}

void CSystem::CreateRendererVars(const SSystemInitParams& startupParams)
{
	REGISTER_CVAR2("r_InitialWindowSizeRatio", &m_rIntialWindowSizeRatio, 0.666f, VF_DUMPTODISK,
				   "Sets the size ratio of the initial application window in relation to the primary monitor resolution.\n"
				   "Usage: r_InitialWindowSizeRatio [1.0/0.666/..]");

	int iFullScreenDefault	= 0;
	int iDisplayInfoDefault = 1;
	int iWidthDefault		= 1280;
	int iHeightDefault		= 720;
#if BB_PLATFORM_WINDOWS && 0
	iFullScreenDefault				   = 0;
	const float initialWindowSizeRatio = m_rIntialWindowSizeRatio->GetFVal();
	iWidthDefault					   = static_cast<int>(GetSystemMetrics(SM_CXSCREEN) * initialWindowSizeRatio);
	iHeightDefault					   = static_cast<int>(GetSystemMetrics(SM_CYSCREEN) * initialWindowSizeRatio);
#elif BB_PLATFORM_LINUX || BB_PLATFORM_APPLE
	iFullScreenDefault		 = 0;
#endif

#if defined(RELEASE)
	iDisplayInfoDefault = 0;
#endif

	// load renderer settings from engine.ini
	REGISTER_CVAR2("r_Width", &m_rWidth, iWidthDefault, VF_DUMPTODISK,
				   "Sets the display width, in pixels.\n"
				   "Usage: r_Width [800/1024/..]");
	REGISTER_CVAR2("r_Height", &m_rHeight, iHeightDefault, VF_DUMPTODISK,
				   "Sets the display height, in pixels.\n"
				   "Usage: r_Height [600/768/..]");
	REGISTER_CVAR2("r_ColorBits", &m_rColorBits, 32, VF_DUMPTODISK | VF_REQUIRE_APP_RESTART,
				   "Sets the color resolution, in bits per pixel. Default is 32.\n"
				   "Usage: r_ColorBits [32/24/16/8]");
	REGISTER_CVAR2("r_DepthBits", &m_rDepthBits, 24, VF_DUMPTODISK | VF_REQUIRE_APP_RESTART,
				   "Sets the depth precision, in bits per pixel. Default is 24.\n"
				   "Usage: r_DepthBits [32/24/16]");
	REGISTER_CVAR2("r_StencilBits", &m_rStencilBits, 8, VF_DUMPTODISK,
				   "Sets the stencil precision, in bits per pixel. Default is 8.\n");

	REGISTER_CVAR2("r_Fullscreen", &m_rFullscreen, iFullScreenDefault, VF_DUMPTODISK,
				   "Toggles fullscreen mode. Default is 1 in normal game and 0 in DevMode.\n"
				   "Usage: r_Fullscreen [0=window/1=fullscreen]");

#if 0
	REGISTER_CVAR2("r_FullscreenNativeRes", &m_rFullscreenNativeRes, 0, VF_DUMPTODISK,
		"Toggles native resolution upscaling.\n"
		"If enabled, scene gets upscaled from specified resolution while UI is rendered in native resolution.");
#endif

	REGISTER_CVAR2("r_DisplayInfo", &m_rDisplayInfo, 1, VF_RESTRICTEDMODE | VF_DUMPTODISK,
				   "Toggles debugging information display.\n"
				   "Usage: r_DisplayInfo [0=off/1=show/2=enhanced/3=minimal/4=fps bar/5=heartbeat]");
	REGISTER_CVAR2("r_Debug", &m_rDebug, 0, VF_RESTRICTEDMODE | VF_DUMPTODISK,
				   "Toggles debugging of renderer.\n"
				   "Usage: r_DisplayInfo [0=off/1=on]");
	REGISTER_CVAR2("r_Tonemap", &m_rTonemap, 1, VF_DUMPTODISK,
				   "Using tonemap.\n"
				   "Usage: r_Tonemap [0=off/1=on]");
}

void CSystem::CreateSystemVars()
{
}

void CSystem::ShutDown()
{
	CSystem::Log("Releasing system");

	if (m_pSystemEventDispatcher)
	{
		m_pSystemEventDispatcher->RemoveListener(this);
	}

	if (m_pUserCallback)
	{
		m_pUserCallback->OnShutdown();
		m_pUserCallback = nullptr;
	}

	GetIRemoteConsole()->Stop();
	GetIRemoteConsole()->UnregisterConsoleVariables();

	SAFE_DELETE(m_pTextModeConsole);

	SAFE_RELEASE(m_pGame);
	//SAFE_DELETE(m_pFont);
	SAFE_RELEASE(m_pWindow);
	SAFE_RELEASE(m_pConsole);
	SAFE_RELEASE(m_Render);

	SAFE_DELETE(m_ScriptObjectConsole);
	SAFE_DELETE(m_ScriptObjectScript);
	SAFE_DELETE(m_ScriptObjectRenderer);
	SAFE_RELEASE(m_pScriptSystem);

	SAFE_RELEASE(m_pLog);
}

void CSystem::EnableGui(bool enable)
{
#if ENABLE_DEBUG_GUI
	if (enable)
	{
		m_env.pInput->AddEventListener(m_GuiManager);
	}
	else
	{
		m_env.pInput->RemoveEventListener(m_GuiManager);
	}
#endif
}

void CSystem::SaveConfiguration()
{
}

float CSystem::GetDeltaTime()
{
	return static_cast<float>(m_DeltaTime);
}

const SFileVersion& CSystem::GetFileVersion()
{
	return m_FileVersion;
}

const SFileVersion& CSystem::GetProductVersion()
{
	return m_ProductVersion;
}

IEntitySystem* CSystem::GetIEntitySystem()
{
	return nullptr;
}

ICryPak* CSystem::GetIPak()
{
	return m_pCryPak;
}

INetwork* CSystem::GetINetwork()
{
	return m_pNetwork;
}

ITimer* CSystem::GetITimer()
{
	return &m_Time;
}

bool CSystem::IsDevMode()
{
	return true;
}

void CSystem::Error(const char* message)
{
}

void CSystem::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_CHANGE_FOCUS:
		break;
	case ESYSTEM_EVENT_MOVE:
		break;
	case ESYSTEM_EVENT_RESIZE:
		m_rWidth  = wparam;
		m_rHeight = lparam;
		break;
	case ESYSTEM_EVENT_ACTIVATE:
		break;
	case ESYSTEM_EVENT_LEVEL_LOAD_START:
		break;
	case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:
		break;
	case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:
		break;
	case ESYSTEM_EVENT_LANGUAGE_CHANGE:
		break;
	case ESYSTEM_EVENT_TOGGLE_FULLSCREEN:
		m_rFullscreen = wparam;
		break;
	case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:
		m_bIsActive = bool(wparam);
		break;
	default:
		break;
	}
}

void CSystem::ShowMessage(const char* message, const char* caption, MessageType messageType)
{
#ifdef _WIN32
	::MessageBox(NULL, message, caption, messageType == 0 ? MB_OK : MB_OKCANCEL);
#endif
}

void CSystem::Log(const char* message)
{
	//std::cout << "-- " << message << std::endl;
	gEnv->pLog->Log("%s", message);
}

IScriptSystem* CSystem::GetIScriptSystem()
{
	//2841004695
	return m_pScriptSystem;
}

bool CSystem::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
	if (!strcmp(pVar->GetName(), "r_cap_profile"))
	{
		switch (std::atoi(sNewValue))
		{
		case 0:
			PROFILER_UNFROZE_FRAME();
			return true;
		case 1:
			PROFILER_FROZE_FRAME();
			return true;
		default:
			return false;
		}
	}
	return false;
}

bool CSystem::OnInputEvent(const SInputEvent& event)
{
	bool result = false;
	//TODO: handle resized
	bool resized = false;
	switch (event.deviceType)
	{
	case eIDT_Mouse:
	{
		bool mouseMoved = event.keyId == eKI_MouseX || event.keyId == eKI_MouseY || event.keyId == eKI_MouseZ;
		if (event.state == eIS_Pressed)
		{
			if (event.keyId == eKI_Mouse1)
			{
				PROFILER_ON_LEFT_CLICK();
			}
			break;
		}
		if (mouseMoved)
		{
			//TODO: IMPLEMENT THIS
#if 0
      PROFILER_ON_MOUSE_POS(event.mouseMove.x, event.mouseMove.y);
#endif
			break;
		}
	}
	case eIDT_Keyboard:
	{
		if (event.state == eIS_Pressed)
		{
			if (event.modifiers == eMM_Alt && event.modifiers == eMM_Shift)
			{
				if (event.keyId == eKI_P)
				{
					if (PROFILER_ISFROZEN())
					{
						PROFILER_UNFROZE_FRAME();
					}
					else
					{
						PROFILER_FROZE_FRAME();
					}
				}
			}
			else if (event.modifiers == eMM_Alt)
			{
				if (event.keyId == eKI_Enter)
				{
					//TODO: IMPLEMENT THIS
#if 0
          static_cast<CWindow*>(m_pWindow)->ToogleFullScreen(1366, 768);
#endif
				}
			}
		}
	}
	default:
		break;
	}
	if (resized)
	{
		assert(0 && "Not implemented");
#if 0
    PROFILER_ON_RESIZE(event.size.width, event.size.height);
#endif
	}

	return result;
}

bool CSystem::Update(int updateFlags /* = 0*/, int nPauseMode /* = 0*/)
{
	//PROFILER_SYNC_FRAME();
	// Update input
	LAST = NOW;
	NOW	 = SDL_GetPerformanceCounter();

	//m_pNetwork->Update();
	if (nPauseMode)
	{
#if ENABLE_DEBUG_GUI
		m_env.pInput->AddEventListener(m_GuiManager);
#endif
	}

	if (m_pUserCallback)
		m_pUserCallback->OnUpdate();

	m_pSystemEventDispatcher->Update();
#if !defined(RELEASE) || defined(RELEASE_LOGGING)
	GetIRemoteConsole()->Update();
#endif

	m_DeltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
	{
		PROFILER_PUSH_CPU_MARKER("INPUT", Utils::COLOR_LIGHT_BLUE);
		//FIXME: CHECK IT
		if (m_env.pInput)
			m_env.pInput->Update(true);
		PROFILER_POP_CPU_MARKER();
	}
	if (m_pWindow)
		m_pWindow->update();
	if (m_pConsole)
		m_pConsole->Update();
	if (m_pNetwork)
		m_pNetwork->UpdateNetwork();
	if (m_pWindow && m_pWindow->closed())
	{
		m_pGame->SendMessage("Quit");
	}
	if (!nPauseMode)
	{
		m_env.p3DEngine->Update();
	}

	return true;
}

bool CSystem::WriteCompressedFile(char* filename, void* data, unsigned int bitlen)
{
	FILE* fp	= fopen(filename, "wb");
	bool result = false;
	if (fp != nullptr)
	{
		if (BITS2BYTES(bitlen) == fwrite(data, 1, BITS2BYTES(bitlen), fp))
		{
			result = true;
		}
		fclose(fp);
	}
	return result;
}

unsigned int CSystem::ReadCompressedFile(char* filename, void* data, unsigned int maxbitlen)
{
	FILE* fp   = fopen(filename, "rb");
	int result = 0;
	if (fp != nullptr)
	{
		result = fread(data, 1, BITS2BYTES(maxbitlen), fp);
		fclose(fp);
	}
	return result;
}

unsigned int CSystem::GetCompressedFileSize(char* filename)
{
	FILE* fp = fopen(filename, "rb");
	int size = 0;
	if (fp != nullptr)
	{
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		fclose(fp);
	}
	return BYTES2BITS(size);
}

void CSystem::OnAfterVarChange(ICVar* pVar)
{
}

void CSystem::OnVarUnregister(ICVar* pVar)
{
}

void CSystem::RenderStatistics()
{
}

const char* CSystem::GetUserName()
{
#if BB_PLATFORM_WINDOWS
	static const int iNameBufferSize = 1024;
	static char szNameBuffer[iNameBufferSize];
	memset(szNameBuffer, 0, iNameBufferSize);

	DWORD dwSize = iNameBufferSize;
	wchar_t nameW[iNameBufferSize];
	::GetUserNameW(nameW, &dwSize);
	strcpy(szNameBuffer, wstr_to_str(nameW).c_str());
	return szNameBuffer;
#elif BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID
	static uid_t uid		 = geteuid();
	static struct passwd* pw = getpwuid(uid);
	if (pw)
	{
		return (pw->pw_name);
	}
	else
	{
		return NULL;
	}
#elif BB_PLATFORM_APPLE
	static const int iNameBufferSize = 1024;
	static char szNameBuffer[iNameBufferSize];
	if (AppleGetUserName(szNameBuffer, iNameBufferSize))
		return szNameBuffer;
	else
		return "";
#else
	return "";
#endif
}

void CSystem::FatalError(const char* format, ...)
{
	// format message
	va_list ArgList;
	char szBuffer[MAX_WARNING_LENGTH];
	const char* sPrefix = "";
	strcpy(szBuffer, sPrefix);
	va_start(ArgList, format);
	vsprintf(szBuffer + strlen(szBuffer)/*, sizeof(szBuffer) - strlen(szBuffer)*/, format, ArgList);
	va_end(ArgList);

	#
	// get system error message before any attempt to write into log
	const char* szSysErrorMessage = 0;
	//CryGetLastSystemErrorMessage();

	CryLogAlways("=============================================================================");
	CryLogAlways("*ERROR");
	CryLogAlways("=============================================================================");
	// write both messages into log
	CryLogAlways("%s", szBuffer);

	if (szSysErrorMessage)
		CryLogAlways("<CrySystem> Last System Error: %s", szSysErrorMessage);

	assert(szBuffer[0] >= ' ');
	//	strcpy(szBuffer,szBuffer+1);	// remove verbosity tag since it is not supported by ::MessageBox

	//LogSystemInfo();

	//CollectMemStats(0, nMSP_ForCrashLog);

	OutputDebugString(szBuffer);
	//OnFatalError(szBuffer);

	#if 0
	if (!g_cvars.sys_no_crash_dialog)
	{
		CryMessageBox(szBuffer,"CRYENGINE FATAL ERROR", eMB_Error);
	}
	#endif

	//GetITextModeConsole()->OnShutdown
	DebugBreak();

}

//////////////////////////////////////////////////////////////////////////
void CSystem::ExecuteCommandLine()
{
	// should only be called once
	{
		static bool bCalledAlready = false;
		assert(!bCalledAlready);
		bCalledAlready = true;
	}

	// execute command line arguments e.g. +g_gametype ASSAULT +map "testy"

	ICmdLine* pCmdLine = GetICmdLine();
	assert(pCmdLine);

	const int iCnt = pCmdLine->GetArgCount();

	for (int i = 0; i < iCnt; ++i)
	{
		const ICmdLineArg* pCmd = pCmdLine->GetArg(i);

		if (pCmd->GetType() == eCLAT_Post)
		{
			string sLine = pCmd->GetName();

			if (gEnv->pSystem->IsCVarWhitelisted(sLine.c_str(), false))
			{
				if (pCmd->GetValue())
					sLine += string(" ") + pCmd->GetValue();

				GetILog()->Log("Executing command from command line: \n%s\n", sLine.c_str()); // - the actual command might be executed much later (e.g. level load pause)
				GetIConsole()->ExecuteString(sLine.c_str());
			}
#if defined(DEDICATED_SERVER)
			else
			{
				GetILog()->LogError("Failed to execute command: '%s' as it is not whitelisted\n", sLine.c_str());
			}
#endif
		}
	}

	//gEnv->pConsole->ExecuteString("sys_RestoreSpec test*"); // to get useful debugging information about current spec settings to the log file
}

//////////////////////////////////////////////////////////////////////////
bool CSystem::IsCVarWhitelisted(const char* szName, bool silent) const
{
	CRY_ASSERT(szName != nullptr);

	if (szName[0] == '?')
	{
		return true;
	}

	if (szName[0] == '+')
	{
		++szName;
	}

	const char* pNameEnd = std::max(strchr(szName, ' '), strchr(szName, '='));
	if (pNameEnd == nullptr)
	{
		return ::IsCVarWhitelisted(szName);
	}
	else
	{
		const string name(szName, pNameEnd);
		return ::IsCVarWhitelisted(name.c_str());
	}
}

//////////////////////////////////////////////////////////////////////////
IStreamEngine* CSystem::GetStreamEngine()
{
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
IRemoteConsole* CSystem::GetIRemoteConsole()
{
	return CRemoteConsole::GetInst();
}

ITextModeConsole* CSystem::GetITextModeConsole()
{
	if (m_env.IsDedicated())
		return m_pTextModeConsole;
	return 0;
}

ISYSTEM_API ISystem* CreateSystemInterface(SSystemInitParams& initParams)
{
	std::unique_ptr<CSystem> pSystem = std::make_unique<CSystem>(initParams);
	initParams.pSystem				 = pSystem.get();
	ModuleInitISystem(pSystem.get(), "System");
#if CRY_PLATFORM_DURANGO
#	if !defined(_LIB)
	m_env = pSystem->GetGlobalEnvironment();
#	endif
	m_env.pWindow = startupParams.hWnd;
#endif
	if (!pSystem->Init())
	{
		pSystem.release();
		initParams.pSystem = nullptr;
		gEnv->pSystem	   = nullptr;
		return nullptr;
	}
	pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_SYSTEM_INIT_DONE, 0, 0);
	// run main loop
	if (initParams.bManualEngineLoop)
	{
		pSystem->RunMainLoop();
		return nullptr;
	}

	return pSystem.release();
}
