#include <Game.hpp>
#include <GameObject.hpp>
#include <TagPoint.hpp>

#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/Texture.hpp>

#include <ScriptObjects/ScriptObjectInput.hpp>
#include <ScriptObjects/ScriptObjectStream.hpp>
#include <ScriptObjects/ScriptObjectTest.hpp>
#include <ScriptObjects/ScriptTimerMgr.hpp>
#include <Localization/ScriptObjectLanguage.h>
#include <Localization/StringTableMgr.h>

#include "ScriptObjectUI.h"

#include "PlayerSystem.h"
#include "Server/XServer.hpp"
#include "XPlayer.h"
#include "XVehicleSystem.h"
#include "TimeDemoRecorder.h"
#include <GameMods.hpp>

#include <WeaponSystemEx.h>
#include <WeaponClass.h>
#include <CMovieUser.h>

//#include "CMovieUser.h"



#include <Client/Client.hpp>
#include <thread>

#include "UISystem.h"
#include "UIHud.h"
#include "IngameDialog.h"

static int g_FontSize = 32;
#define MEASURETIME(str) \
/* CryLog("[MEASURETIME] %s", str)*/

#ifdef USE_GUI
#	include <imgui.h>
#	include <imgui_internal.h>
#endif

static CXGame* gGame;

#include "TextEditorDemo.hpp"

#include <BlackBox/GUI/ControlPanel.hpp>
#ifdef WIN32
#	include <tchar.h>
#	include <winioctl.h>

typedef std::basic_string<TCHAR> tstring;
typedef std::vector<TCHAR>		 tvector;
#endif


int render_camera = 0;

static bool menuOnTopLevel = false;
char*		mods_str[]	   = {"$0BlackBox", "$1WightBox", "$9GreyBox"};
int			selected_mod	   = 0;

std::string vec_to_string(Legacy::Vec3 vec)
{
	auto result = "{ x: " + std::to_string(vec.x);
	result += ", y: " + std::to_string(vec.y);
	result += ", z: " + std::to_string(vec.z) + "}";
	return result;
}

static Legacy::IVec4 currentQuad{};
static bool	 mouseClicked{};

#ifdef USE_GUI
namespace ImGui
{
	void InputCVarFloat(ICVar* v, const char* name, float* val)
	{
		if (ImGui::InputFloat(name, val))
		{
			gEnv->pConsole->ExecuteString(std::string(std::string(name) + " " + std::to_string(*val)).data());
			//v->Set(cvv.f);
		}
	}
	void InputCVarInt(ICVar* v, const char* name, int* val)
	{
		if (ImGui::InputInt(name, val))
		{
			gEnv->pConsole->ExecuteString(std::string(std::string(name) + " " + std::to_string(*val)).data());
			//v->Set(cvv.f);
		}
	}
	void InputCVarString(ICVar* v, const char* name, char* val)
	{
		if (ImGui::InputText(name, val, 256))
		{
			gEnv->pConsole->ExecuteString(std::string(std::string(name) + " " + val).data());
			//v->Set(cvv.f);
		}
	}
} // namespace ImGui
#endif

namespace
{
#ifdef USE_STEAM
	// определяем достижения
	enum EAchievements
	{
		TEST_ACHIEVEMENT_1_0 = 0,
#	if 0
		ACH_WIN_100_GAMES = 1,
		ACH_TRAVEL_FAR_ACCUM = 2,
		ACH_TRAVEL_FAR_SINGLE = 3,
#	endif
	};

	// массив достижений, содержащий данные о достижениях и их состоянии
	Achievement_t g_Achievements[] =
		{
			_ACH_ID(TEST_ACHIEVEMENT_1_0, "10 Hits To Box"),
#	if 0
		_ACH_ID( ACH_WIN_100_GAMES, "Champion" ),
		_ACH_ID( ACH_TRAVEL_FAR_ACCUM, "Interstellar" ),
		_ACH_ID( ACH_TRAVEL_FAR_SINGLE, "Orbiter" ),
#	endif
	};

	// глобальный доступ к объекту Achievements
	CSteamAchievements* g_SteamAchievements = NULL;
#endif
} // namespace

int g_bRenderGame = true;

void LoadHistory()
{
	std::ifstream	   is("history.txt");
	std::stack<string> history;
	if (is.is_open())
	{
		std::string line;
		while (std::getline(is, line))
		{
			history.push(line);
			gEnv->pConsole->AddCommandToHistory(line.data());
		}
		/*
		while (!history.empty())
		{
			is << history.top() << std::endl;
			history.pop();
		}
		*/
	}
}

void SaveHistory()
{
	std::ofstream	   is("history.txt");
	std::stack<string> history;
	if (is.is_open())
	{
		while (auto h = gEnv->pConsole->GetHistoryElement(true))
		{
			history.push(h);
		}
		while (!history.empty())
		{
			is << history.top() << std::endl;
			history.pop();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CTagPointManager
class CTagPointManager : public ITagPointManager
{
public:
	CTagPointManager( IGame *pGame )  { m_pGame = (CXGame*) pGame; };
	virtual ~CTagPointManager() {};

	// This function creates a tag point in the game world
	virtual ITagPoint *CreateTagPoint(const string &name, const Legacy::Vec3 &pos, const Legacy::Vec3 &angles) 
	{
		return m_pGame->CreateTagPoint( name, pos, angles );
	};

	// Retrieves a tag point by name
	virtual ITagPoint *GetTagPoint(const string &name)
	{
		return m_pGame->GetTagPoint( name );
	}

	// Deletes a tag point from the game
	virtual void RemoveTagPoint(ITagPoint *pPoint)
	{
		m_pGame->RemoveTagPoint( pPoint );
	}

	virtual void AddRespawnPoint(ITagPoint *pPoint)
	{
		m_pGame->AddRespawnPoint( pPoint );
	}

	virtual void RemoveRespawnPoint(ITagPoint *pPoint)
	{
		m_pGame->RemoveRespawnPoint( pPoint );
	}

private:
	CXGame *m_pGame;
};


//////////////////////////////////////////////////////////////////////////////////////////////
// CXGame
//!constructor
//////////////////////////////////////////////////////////////////////
CXGame::CXGame()
{
	m_pTimeDemoRecorder = NULL;
	m_pScriptObjectGame = NULL;
	m_pScriptTimerMgr = NULL;
	m_pScriptSystem = NULL;
	m_pServer = NULL;
	m_pClient = NULL;
	m_pLog = NULL;
	m_pServerSnooper = NULL;
	m_pNETServerSnooper = 0;
	m_pRConSystem = 0;
	m_pWeaponSystemEx = NULL;
	m_mapTagPoints.clear();
	m_bMenuInitialized = false;
	m_pCurrentUI = 0;	
	m_pIActionMapManager=NULL;
	m_pIngameDialogMgr = new CIngameDialogMgr();
	m_pUISystem = 0;
	mp_model = 0;
#if !defined(LINUX)
	// to avoid all references to movie user in this file
	m_pMovieUser = new CMovieUser(this);
#endif
	m_nPlayerIconTexId = -1;
	m_nVehicleIconTexId = -1;
	m_nBuildingIconTexId = -1;
	m_nUnknownIconTexId = -1;
	m_bMenuOverlay = false;
	m_bUIOverlay = false;
	m_bUIExclusiveInput = false;
	m_bHideLocalPlayer = false;
	m_pCVarCheatMode=NULL;

	m_fTimeGran=m_fFixedStep=m_fTimeGran2FixedStep=m_frFixedStep = 0;
	m_iFixedStep2TimeGran = 0;
	g_language=0;
	g_playerprofile=0;
	g_serverprofile=0;

	m_tPlayerPersistentData.m_bDataSaved=false;
	m_fFadingStartTime=-1.0f;
	cv_game_physics_quality=NULL;
	m_bMapLoadedFromCheckpoint=false;
	m_bSynchronizing = false;

	//m_fTimeToSaveThumbnail = 0;
	m_pGameMods = NULL;
	m_bLastDoLateSwitch = 0;
	m_bLastCDAuthentication = 0;
	m_bAllowQuicksave = true;

	m_sGameName = "FarCry";
	m_pTagPointManager = new CTagPointManager( this );
	m_nDEBUG_TIMING = 0;
	m_fDEBUG_STARTTIMER = 0;
}

//////////////////////////////////////////////////////////////////////
//!destructor
CXGame::~CXGame()
{
	m_pScriptSystem->BeginCall("Shutdown");
	m_pScriptSystem->PushFuncParam(0);
	m_pScriptSystem->EndCall();

	if (m_pIngameDialogMgr)
		delete m_pIngameDialogMgr;
	m_pIngameDialogMgr=NULL;

#if !defined(LINUX)
	if (m_pMovieUser)
	{
		if (m_pSystem)
		{
			IMovieSystem *pMovieSystem=m_pSystem->GetIMovieSystem();
			if (pMovieSystem)
			{
				if (pMovieSystem->GetUser()==m_pMovieUser)
					pMovieSystem->SetUser(NULL);
			}
		}
		delete m_pMovieUser;
	}
#endif

	m_pMovieUser = NULL;

	// shutdown the client if there is one
	ShutdownClient();

	// shutdown the server if there is one
	ShutdownServer();

	if (m_pUISystem)
	{
		m_pUISystem->Release();
	}
	SAFE_DELETE(m_pUISystem);

	CScriptObjectUI::ReleaseTemplate();
#if 0
	CScriptObjectPlayer::ReleaseTemplate();
	CScriptObjectFireParam::ReleaseTemplate();
	CScriptObjectWeaponClass::ReleaseTemplate();
	CScriptObjectVehicle::ReleaseTemplate();
	CScriptObjectSpectator::ReleaseTemplate();
	CScriptObjectAdvCamSystem::ReleaseTemplate();
	CScriptObjectSynched2DTable::ReleaseTemplate();
	CScriptObjectBoids::ReleaseTemplate();
	CScriptObjectRenderer::ReleaseTemplate();
#endif
	CScriptObjectGame::ReleaseTemplate();
	CScriptObjectInput::ReleaseTemplate();
	CScriptObjectLanguage::ReleaseTemplate();

#if 0
	CScriptObjectAI::ReleaseTemplate();
#endif
	CScriptObjectServer::ReleaseTemplate();
	CScriptObjectServerSlot::ReleaseTemplate();
	CScriptObjectClient::ReleaseTemplate();
	CScriptObjectStream::ReleaseTemplate();

	SAFE_RELEASE(cl_scope_flare);
	SAFE_RELEASE(cl_ThirdPersonRange);

	SAFE_RELEASE(cl_ThirdPersonOffs);
	SAFE_RELEASE(cl_ThirdPersonOffsAngHor);
	SAFE_RELEASE(cl_ThirdPersonOffsAngVert);

	SAFE_RELEASE(cl_display_hud);
	SAFE_RELEASE(cl_motiontracker);
	SAFE_RELEASE(cl_hud_pickup_icons);
	SAFE_RELEASE(cl_msg_notification);
	SAFE_RELEASE(cl_hud_name);
	SAFE_RELEASE(ai_num_of_bots);
	SAFE_RELEASE(p_name);
	SAFE_RELEASE(p_model);
	SAFE_RELEASE(mp_model);
	SAFE_RELEASE(p_color);
	SAFE_RELEASE(p_always_run);
	SAFE_RELEASE(g_language);
	SAFE_RELEASE(g_playerprofile);
	SAFE_RELEASE(g_serverprofile);
	SAFE_RELEASE(g_GC_Frequence);
	SAFE_RELEASE(p_speed_run);
	SAFE_RELEASE(p_sprint_scale);

	SAFE_RELEASE(p_sprint_decoy);
	SAFE_RELEASE(p_sprint_restore_run);
	SAFE_RELEASE(p_sprint_restore_idle);

	SAFE_RELEASE(p_speed_walk);
	SAFE_RELEASE(p_speed_crouch);
	SAFE_RELEASE(p_speed_prone);
	SAFE_RELEASE(p_jump_force);
	SAFE_RELEASE(p_jump_run_time);
	SAFE_RELEASE(p_jump_walk_time);
	SAFE_RELEASE(p_lean_offset);
	SAFE_RELEASE(p_bob_pitch);
	SAFE_RELEASE(p_bob_roll);
	SAFE_RELEASE(p_bob_length);
	SAFE_RELEASE(p_bob_weapon);
	SAFE_RELEASE(p_bob_fcoeff);
	SAFE_RELEASE(p_weapon_switch);

	SAFE_RELEASE(cv_game_physics_quality);

	SAFE_RELEASE(m_jump_vel);
	SAFE_RELEASE(m_jump_arc);

	SAFE_RELEASE(b_camera);

	SAFE_RELEASE(sv_timeout);
	SAFE_RELEASE(cl_timeout);
	SAFE_RELEASE(cl_loadtimeout);

	SAFE_RELEASE(g_LevelName);
	SAFE_RELEASE(g_GameType);
	SAFE_RELEASE(g_LeftHanded);

	SAFE_RELEASE(p_DeadBody);
	SAFE_RELEASE(p_HitImpulse);
	SAFE_RELEASE(p_RotateHead);
	SAFE_RELEASE(a_DrawArea);
	SAFE_RELEASE(a_LogArea);

	SAFE_RELEASE(m_pCVarCheatMode);

	SAFE_RELEASE(pl_JumpNegativeImpulse);

	if (m_pRenderer && (m_nPlayerIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nPlayerIconTexId);
	if (m_pRenderer && (m_nVehicleIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nVehicleIconTexId);
	if (m_pRenderer && (m_nBuildingIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nBuildingIconTexId);
	if (m_pRenderer && (m_nUnknownIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nUnknownIconTexId);

	SAFE_DELETE(m_pUIHud);
#if 0
	SAFE_DELETE(m_pWeaponSystemEx);
	SAFE_DELETE(m_pVehicleSystem);
#endif
	SAFE_DELETE(m_pPlayerSystem);

#if 0
	SAFE_DELETE(m_pFlockManager);
#endif

#if 0
	CScriptObjectPlayer::ReleaseTemplate();
#endif

	//shutdown script stuff
	SAFE_DELETE(m_pScriptObjectGame);
	SAFE_DELETE(m_pScriptObjectInput);

#if 0
	SAFE_DELETE(m_pScriptObjectBoids);
	SAFE_DELETE(m_pScriptObjectLanguage);
	SAFE_DELETE(m_pScriptObjectAI);
#endif

	// Release the action map
	SAFE_RELEASE(m_pIActionMapManager);
	SAFE_DELETE(m_pScriptTimerMgr);
	// release the tags
	if (!m_mapTagPoints.empty())
	{
		TagPointMap::iterator ti;
		for (ti = m_mapTagPoints.begin(); ti != m_mapTagPoints.end(); ti++)
			delete ti->second;
	}

	SAFE_RELEASE(m_pServerSnooper);
	SAFE_RELEASE(m_pNETServerSnooper);
	SAFE_RELEASE(m_pRConSystem);
	SAFE_DELETE(m_pTimeDemoRecorder);
	SAFE_DELETE(m_pGameMods);

	delete m_pTagPointManager;
}

//////////////////////////////////////////////////////////////////////
bool CXGame::InitClassRegistry()
{
	m_EntityClassRegistry.Init(m_pSystem);
	CPlayerSystem*	 pPlayerSystem	 = GetPlayerSystem();
	CVehicleSystem*	 pVehicleSystem	 = GetVehicleSystem();
	CWeaponSystemEx* pWeaponSystemEx = GetWeaponSystemEx(); // m10

	assert(pPlayerSystem);
	assert(pVehicleSystem);
#if 0
	assert(pWeaponSystemEx);
#endif

	// Enumerate entity classes.
	EntityClass* entCls = NULL;
	m_EntityClassRegistry.MoveFirst();
	do {
		entCls = m_EntityClassRegistry.Next();
		if (entCls)
		{
			const char*	  entity_type = entCls->strGameType.c_str();
			EntityClassId ClassId	  = entCls->ClassId;
			if (strcmp("Player", entity_type) == 0)
				pPlayerSystem->AddPlayerClass(ClassId);

			if (strcmp("Vehicle", entity_type) == 0)
				pVehicleSystem->AddVehicleClass(ClassId);

			if (strcmp("Projectile", entity_type) == 0)
			{
				// cannot be loaded at that point - other scripts must be loaded before
				#if 0
				pWeaponSystemEx->AddProjectileClass(ClassId);
				#endif
			}
		}
	} while (entCls);
	return true;
}

//////////////////////////////////////////////////////////////////////////
void CXGame::SoftReset()
{
	m_pLog->Log("Soft Reset Begin");
	//allow to reload scripts with(LoadScript)
	m_pScriptSystem->SetGlobalToNull("_localplayer");
	if (m_pScriptSystem)
		m_pScriptSystem->UnloadScripts();

	std::vector<string> vLoadedWeapons;

	for (int i = 0; i != m_pWeaponSystemEx->GetNumWeaponClasses(); ++i)
		vLoadedWeapons.push_back(m_pWeaponSystemEx->GetWeaponClass(i)->GetName());

	m_pWeaponSystemEx->Reset();
#if !defined(LINUX)
	if (m_pSystem->GetIMovieSystem())
		m_pSystem->GetIMovieSystem()->StopAllSequences();
#endif
	m_pScriptObjectGame->Reset();

	m_pScriptSystem->ForceGarbageCollection();
	//m_p3DEngine->ClearRenderResources();

	m_iLastCmdIdx = 0;

	m_pWeaponSystemEx->Init(this);

	for (std::vector<string>::iterator i = vLoadedWeapons.begin(); i != vLoadedWeapons.end(); ++i)
		AddWeapon((*i).c_str());

	if (m_pCurrentUI)
	{
		m_pCurrentUI->Reset();
		m_pCurrentUI->Init(m_pScriptSystem);
	}

	m_pLog->Log("Soft Reset End");
}

bool CXGame::OnInputEvent(const SInputEvent& event)
{
	return false;
}

bool CXGame::OnInputEventUI(const SUnicodeEvent& event)
{
	return false;
}

//////////////////////////////////////////////////////////////////////
//! reset the game before a level reloading
//! this function allows the reloading of all scripts
//! and wipes out all textures from the 3dengine
void CXGame::Reset()
{
	m_pEntitySystem->Reset();

	// Unload all music.
	if (m_pSystem->GetIMusicSystem())
		m_pSystem->GetIMusicSystem()->Unload();

	//allow to reload scripts with(LoadScript)
	m_pScriptSystem->SetGlobalToNull("_localplayer");
	if (m_pScriptSystem)
		m_pScriptSystem->UnloadScripts();

	m_pWeaponSystemEx->Reset();
	m_XSurfaceMgr.Reset();
	m_XAreaMgr.Clear();
	ClearTagPoints();
#if !defined(LINUX)
	if (m_pSystem->GetIMovieSystem())
		m_pSystem->GetIMovieSystem()->Reset(false);
#endif
	m_pScriptObjectGame->Reset();

	m_pScriptSystem->ForceGarbageCollection();
	m_p3DEngine->ClearRenderResources();

	// Must reset all timers.
	m_pScriptTimerMgr->Reset();

	//clen up the input buffer
	if (m_pSystem->GetIInput())
	{
		m_pSystem->GetIInput()->Update(true);
		m_pSystem->GetIInput()->Update(true);
	}
	if (m_pIActionMapManager)
		m_pIActionMapManager->Reset();

	m_iLastCmdIdx = 0;

	//////////////////////////////////////////////////////////////////////////
	// Reset UI.
	//////////////////////////////////////////////////////////////////////////
	if (m_pUISystem)
	{
		m_pUISystem->UnloadAllModels();
		m_pUISystem->StopAllVideo();
		m_p3DEngine->Enable(1);
		m_pSystem->GetILog()->Log("UISystem: Enabled 3D Engine!");
	}
	if (m_pCurrentUI)
		m_pCurrentUI->Reset();
	if (m_pUIHud)
		m_pUIHud->Reset();

	if (GetMyPlayer())
		GetMyPlayer()->SetNeedUpdate(true);
	NOT_IMPLEMENTED;
}

//////////////////////////////////////////////////////////////////////
IXSystem* CXGame::GetXSystem()
{
	#if 1
	return m_pServer ? m_pServer->m_pISystem : m_pClient ? m_pClient->m_pISystem
														 : NULL;
	#else
	NOT_IMPLEMENTED_V;
	#endif
}

bool CXGame::Init(ISystem* pSystem, bool bDedicatedSrv, bool bInEditor, const char* szGameMod)
{
	gGame = this;
	m_pLegacyInput = new Legacy::CInput(gEnv->pInput);
	// Setup the system and 3D Engine pointers
	m_pSystem = pSystem;

	m_pGameMods = new CGameMods(this);

	m_bDedicatedServer = bDedicatedSrv;
	m_XAreaMgr.Init(pSystem);
	m_bEditor = bInEditor;

	m_bRelaunch				  = false;
	m_bMovieSystemPaused	  = false;
	m_bIsLoadingLevelFromFile = false;

	m_bOK		 = false;
	m_bUpdateRet = true;
	m_pClient	 = NULL;
	m_pServer	 = NULL;

	m_pSystem->GetILog()->Log("Game Initialization");
#if !defined(LINUX)
	IMovieSystem* pMovieSystem = m_pSystem->GetIMovieSystem();
	if (pMovieSystem)
		pMovieSystem->SetUser(m_pMovieUser);
#endif
	if (!m_pTimeDemoRecorder)
		m_pTimeDemoRecorder = new CTimeDemoRecorder(pSystem);

	m_pUIHud		= NULL;
	m_pNetwork		= m_pSystem->GetINetwork();
	m_pLog			= m_pSystem->GetILog();
	m_p3DEngine		= m_pSystem->GetI3DEngine();
	m_pRenderer		= m_pSystem->GetIRenderer();
	m_pScriptSystem = pSystem->GetIScriptSystem();
	m_pEntitySystem = m_pSystem->GetIEntitySystem();

	// Register game rendering callback.
	//[Timur] m_p3DEngine->SetRenderCallback( OnRenderCallback,this );

	// init subsystems
#ifndef _XBOX
	m_pServerSnooper	= m_pNetwork->CreateServerSnooper(this);
	m_pNETServerSnooper = m_pNetwork->CreateNETServerSnooper(this);
	m_pRConSystem		= m_pNetwork->CreateRConSystem();
#endif
	m_pWeaponSystemEx = new CWeaponSystemEx();
	m_pVehicleSystem  = new CVehicleSystem();
	m_pPlayerSystem	  = new CPlayerSystem();
	#if 0
	m_pFlockManager	  = new CFlockManager(m_pSystem);
	#endif

	CScriptObjectUI::InitializeTemplate(m_pScriptSystem);

	// init is not necessary for now, but add here if it later is
	m_pScriptObjectGame = new CScriptObjectGame;
	m_pScriptObjectGame->InitializeTemplate(m_pScriptSystem);

	m_pScriptObjectInput = new CScriptObjectInput;
	CScriptObjectInput::InitializeTemplate(m_pScriptSystem);
	m_pScriptObjectLanguage = new CScriptObjectLanguage;
	CScriptObjectLanguage::InitializeTemplate(m_pScriptSystem);
	#if 0
	m_pScriptObjectBoids = new CScriptObjectBoids;
	CScriptObjectBoids::InitializeTemplate(m_pScriptSystem);
	m_pScriptObjectAI = new CScriptObjectAI;
	CScriptObjectAI::InitializeTemplate(m_pScriptSystem);
	#endif
	CScriptObjectServer::InitializeTemplate(m_pScriptSystem);

	#if 0
	CScriptObjectPlayer::InitializeTemplate(m_pScriptSystem);
	CScriptObjectFireParam::InitializeTemplate(m_pScriptSystem);
	CScriptObjectWeaponClass::InitializeTemplate(m_pScriptSystem);
	CScriptObjectVehicle::InitializeTemplate(m_pScriptSystem);
	CScriptObjectSpectator::InitializeTemplate(m_pScriptSystem);
	CScriptObjectAdvCamSystem::InitializeTemplate(m_pScriptSystem);
	CScriptObjectSynched2DTable::InitializeTemplate(m_pScriptSystem);
	CScriptObjectRenderer::InitializeTemplate(m_pScriptSystem);
	#endif

	m_pScriptObjectGame->Init(m_pScriptSystem, this);
	m_pScriptObjectInput->Init(m_pScriptSystem, this, m_pSystem);

	m_pScriptObjectLanguage->Init(m_pScriptSystem, &m_StringTableMgr);
	#if 0
	m_pScriptObjectBoids->Init(m_pScriptSystem, m_pSystem, m_pFlockManager);
	m_pScriptObjectAI->Init(m_pScriptSystem, m_pSystem, this);
	#endif

	CScriptObjectServerSlot::InitializeTemplate(m_pScriptSystem);
	CScriptObjectClient::InitializeTemplate(m_pScriptSystem);
	CScriptObjectStream::InitializeTemplate(m_pScriptSystem);

	m_pScriptTimerMgr = new CScriptTimerMgr(m_pScriptSystem, m_pSystem->GetIEntitySystem(), this);

	// making some constants accessable to the script
	m_pScriptSystem->SetGlobalValue("FireActivation_OnPress", ePressing);
	m_pScriptSystem->SetGlobalValue("FireActivation_OnRelease", eReleasing);
	m_pScriptSystem->SetGlobalValue("FireActivation_OnHold", eHolding);

	m_pScriptSystem->SetGlobalValue("ENTITYTYPE_PLAYER", ENTITYTYPE_PLAYER);
	m_pScriptSystem->SetGlobalValue("ENTITYTYPE_WAYPOINT", ENTITYTYPE_WAYPOINT);
	m_pScriptSystem->SetGlobalValue("ENTITYTYPE_OWNTEAM", ENTITYTYPE_OWNTEAM);

	InitConsoleVars();

	if (szGameMod && szGameMod[0])
	{
		// apply the mod without restarting as the game just started!
		GetModsInterface()->SetCurrentMod(szGameMod, false);
	}

	InitClassRegistry();

	// execute the "main"-script (to pre-load other scripts, etc.)
	m_pScriptSystem->ExecuteFile("scripts/main.lua");
	m_pScriptSystem->BeginCall("Init");
	m_pScriptSystem->PushFuncParam(0);
	m_pScriptSystem->EndCall();

	// initialize the surface-manager
	m_XSurfaceMgr.Init(m_pScriptSystem, m_p3DEngine, GetSystem()->GetIPhysicalWorld());

	// init key-bindings
	if (!m_bDedicatedServer)
		InitInputMap();

	// create various console-commands/variables
	InitConsoleCommands();

	// loading the main language-string-table
	if (!m_StringTableMgr.Load(GetSystem(), *m_pScriptObjectLanguage, g_language->GetString()))
		m_pLog->Log("cannot load language file [%s]", g_language->GetString());

	// creating HUD interface
	m_pLog->Log("Initializing UI");
	m_pUIHud = new CUIHud(this, m_pSystem);

	LoadConfiguration("", "game.cfg");

	//////////////////////////////////////////////////////////////////////////
	// Materials
	// load materials (once before all, this info stays till we quit the game - no need to load material later)
	// first load normal materials
	m_XSurfaceMgr.LoadMaterials("scripts/materials");

	if (!m_bDedicatedServer)
	{
		m_pSystem->GetIConsole()->ShowConsole(0);
		if (!bInEditor)
		{
			//////////////////////////////////////////////////////////////////////
			m_pUISystem = new CUISystem;

			if (m_pUISystem)
			{
				m_pUISystem->Create(this, m_pSystem, m_pScriptSystem, "Scripts/MenuScreens/UISystem.lua", 1);
			}
			else
			{
				m_pLog->Log("Failed to create UI System!");
			}
			//////////////////////////////////////////////////////////////////////
		}

		if (m_pUISystem)
		{
			m_bMenuOverlay = 1;
		}
	}
	else
		m_pSystem->GetIConsole()->ShowConsole(true);

	// load textures used as icons by the mini-map
	if (m_pRenderer)
	{
		ITexPic* pPic;
		pPic = m_pRenderer->EF_LoadTexture("gui/map_player", FT_NOREMOVE, 0, eTT_Base);
		if (pPic && pPic->IsTextureLoaded())
			m_nPlayerIconTexId = pPic->GetTextureID();
		pPic = m_pRenderer->EF_LoadTexture("gui/map_vehicle", FT_NOREMOVE, 0, eTT_Base);
		if (pPic && pPic->IsTextureLoaded())
			m_nVehicleIconTexId = pPic->GetTextureID();
		pPic = m_pRenderer->EF_LoadTexture("gui/map_building", FT_NOREMOVE, 0, eTT_Base);
		if (pPic && pPic->IsTextureLoaded())
			m_nBuildingIconTexId = pPic->GetTextureID();
		pPic = m_pRenderer->EF_LoadTexture("gui/map_unknown", FT_NOREMOVE, 0, eTT_Base);
		if (pPic && pPic->IsTextureLoaded())
			m_nUnknownIconTexId = pPic->GetTextureID();
	}

	if (!bInEditor)
		m_pEntitySystem->SetDynamicEntityIdMode(true);

#if !defined(_XBOX) && !defined(PS2) && !defined(LINUX)
	SetCursor(NULL);
#endif

	DevModeInit();
	m_bOK				 = true;
	e_deformable_terrain = NULL;

	Minecraft
	return (true);
}

#include "CameraController.hpp"

bool MouseInQuad(int x, int y, int w, int h)
{
	float mx, my;
	float fx = (float)x;
	float fy = (float)y;
	float fw = (float)w;
	float fh = (float)h;
	gEnv->pHardwareMouse->GetHardwareMousePosition(&mx, &my);

	return (mx >= fx && mx <= (fx + fw)) && (my >= fy - fh && my <= (fy));
}

//////////////////////////////////////////////////////////////////////
//! updates whole game and children
bool CXGame::Update()
{
	if (!m_nDEBUG_TIMING)
	{
		m_fDEBUG_STARTTIMER = m_pSystem->GetITimer()->GetAsyncCurTime();
		m_nDEBUG_TIMING = 1;
	}
	if (!m_bEditor)
	{
		if (!m_bMenuOverlay || !m_pUISystem || m_pUISystem->GetScriptObjectUI()->CanRenderGame())
		{
			m_p3DEngine->Enable(1);
		}
		else
		{
			m_p3DEngine->Enable(0);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	FUNCTION_PROFILER(PROFILE_GAME);

#ifdef USE_STEAM
	SteamAPI_RunCallbacks();
#endif
	bool bRenderFrame = (!Vec3(Legacy::from(m_pSystem->GetViewCamera().GetPos())).IsZero() || m_bMenuOverlay || m_bUIOverlay) 
											&& g_Render->GetIVal() != 0;
	//////////////////////////////////////////////////////////////////////////
	// Start Profiling frame
	m_pSystem->GetIProfileSystem()->StartFrame();
	ITimer* pTimer = m_pSystem->GetITimer();
	MEASURETIME("EnterGameUp");
	//Timur[10/2/2002]
	// Cannot Run Without System.
	assert(m_pSystem);

	#if 0
	float maxFPS = g_maxfps->GetFVal();
	if (maxFPS > 0)
	{
		DWORD extraTime = (DWORD)((1.0f / maxFPS - pTimer->GetFrameTime()) * 1000.0f);
#if !defined(LINUX)
		if (extraTime > 0 && extraTime < 300) //thread sleep not process sleep
			Sleep(extraTime);
#endif
	}
	#endif

	PhysicsVars* pVars	   = m_pSystem->GetIPhysicalWorld()->GetPhysVars();
	float		 fTimeGran = pVars->timeGranularity, fFixedStep = g_MP_fixed_timestep->GetFVal();
	if (fTimeGran != m_fTimeGran || fFixedStep != m_fFixedStep)
	{
		m_fTimeGran	 = fTimeGran;
		m_fFixedStep = fFixedStep;
		if (fFixedStep > 0)
		{
			m_fTimeGran2FixedStep = fTimeGran / fFixedStep;
			m_iFixedStep2TimeGran = (int)(fFixedStep / fTimeGran + 0.5f);
			m_frFixedStep		  = 1.0f / fFixedStep;
		}
		else
			m_iFixedStep2TimeGran = 0;
		m_frTimeGran = 1.0f / fTimeGran;
	}
	pVars->bMultiplayer = IsMultiplayer() ? 1 : 0;

	//check what is the current process
	IProcess* pProcess = m_pSystem->GetIProcess();
	if (!pProcess)
		return false;

	bool bPause = IsInPause(pProcess);
	if (m_bIsLoadingLevelFromFile)
		bPause = false;
#if !defined(LINUX)
	// Pauses or unpauses movie system.
	#if 0
	if (bPause != m_bMovieSystemPaused)
	{
		m_bMovieSystemPaused = bPause;
		if (bPause)
			m_pSystem->GetIMovieSystem()->Pause();
		else
			m_pSystem->GetIMovieSystem()->Resume();
	}
	#endif
#endif

	// [marco] check current sound and vis areas
	// for music etc.
	CheckSoundVisAreas();

	MEASURETIME("SomeStuff");
	// update system
	int nPauseMode = 0;
	if (bPause)
		nPauseMode = 1;

	if (IsMultiplayer())
	{
		pe_params_flags pf;
		pf.flagsOR = pef_update;
		for (ListOfPlayers::iterator pl = m_DeadPlayers.begin(); pl != m_DeadPlayers.end(); pl++)
			if ((*pl)->GetEntity() && (*pl)->GetEntity()->GetPhysics())
				(*pl)->GetEntity()->GetPhysics()->SetParams(&pf);
	}

	// We use engine for update
	if (!m_pSystem->Update(IsMultiplayer() ? ESYSUPDATE_MULTIPLAYER : 0, nPauseMode)) //Update returns false when quitting
		return (false);

	if (IsMultiplayer())
	{
		pe_params_flags pf;
		pf.flagsAND = ~pef_update;
		for (ListOfPlayers::iterator pl = m_DeadPlayers.begin(); pl != m_DeadPlayers.end(); pl++)
			if ((*pl)->GetEntity() && (*pl)->GetEntity()->GetPhysics())
				(*pl)->GetEntity()->GetPhysics()->SetParams(&pf);
	}

	MEASURETIME("SysUpdate");

	// [marco] after system update, retrigger areas if necessary
	if (!bPause)
		RetriggerAreas();

	if (!bPause || (m_pClient && !m_pClient->IsConnected()))
	{
		// network start
		FRAME_PROFILER("GameUpdate:Client", PROFILE_GAME);

		// update client
		if (m_pClient)
		{
			m_pClient->UpdateClientNetwork();
			MEASURETIME("Net");

			assert(m_pClient);
			m_pClient->Update();

			if (m_pClient->DestructIfMarked()) //  to make sure the client is only released in one place - here
				m_pClient = 0;
		}

		MEASURETIME("ClServ Up");

		////////UPDATE THE NETWORK
		// [Anton] moved from after the rendering, so that the server has access to the most recent physics data
		// update server
		if (m_pServer)
		{
			FRAME_PROFILER("GameUpdate:Server", PROFILE_GAME);

			m_pServer->Update();
		}

		MEASURETIME("EndServUp");
	}

	m_pNetwork->UpdateNetwork(); // used to update things like the UBI.com services

	DWORD dwCurrentTimeInMS = GetCurrentTime();

	if (!m_pSystem->IsDedicated())
	{
		if (m_pServerSnooper)
			m_pServerSnooper->Update(dwCurrentTimeInMS);

		if (m_pNETServerSnooper)
			m_pNETServerSnooper->Update(dwCurrentTimeInMS);
	}

	if (m_pRConSystem)
	{
		if (m_pClient)
			m_pRConSystem->Update(dwCurrentTimeInMS, m_pClient->GetNetworkClient());
		else
			m_pRConSystem->Update(dwCurrentTimeInMS);
	}

	// network end

	// system rendering
	if (bRenderFrame)
	{
		// Rendering in Engine
		// render begin must be always called anyway to clear buffer, draw buttons etc.
		// even in menu mode
		m_pSystem->RenderBegin();

		m_pSystem->Render();
		MEASURETIME("3SysRend");
	}

	// update the HUD
	if (m_pCurrentUI && !bPause && m_pClient && m_pClient->m_bDisplayHud)
	{
		FRAME_PROFILER("GameUpdate:HUD", PROFILE_GAME);

		// update hud itself
		if (!m_pCurrentUI->Update())
			m_bUpdateRet = false;

		// update ingame-dialog-manager
		if (m_pIngameDialogMgr)
			m_pIngameDialogMgr->Update();

		MEASURETIME("HUD Up");
	}

	if (m_pUISystem && m_pUISystem->IsEnabled())
	{
		FRAME_PROFILER("GameUpdate:UI", PROFILE_GAME);

		if (m_bMenuOverlay || m_bUIOverlay)
		{
			m_pUISystem->Update();
			m_pUISystem->Draw();
		}
	}

	if (a_DrawArea->GetIVal())
	{
		m_XAreaMgr.DrawAreas(m_pSystem);
		MEASURETIME("XAreaDraw");
	}

	// print time profiling results
#ifndef PS2
	#if 0
	MEASURETIME((const char*)-1);
	#endif
#else
	MEASURETIME("Time");
#endif

	MEASURETIME("3TimeProf");

	//NETWORK DEBUGGING
	if (m_pClient && m_pClient->cl_netstats->GetIVal() != 0)
	{
		m_pClient->DrawNetStats();
	}
	if (m_pServer && m_pServer->sv_netstats->GetIVal() != 0)
	{
		m_pServer->DrawNetStats(m_pRenderer);
	}

	MEASURETIME("NetStats");

	// end of frame
	if (bRenderFrame)
	{
		// same thing as for render begin
		if (m_pTimeDemoRecorder)
			m_pTimeDemoRecorder->RenderInfo();

		m_pSystem->RenderEnd();
	}
	MEASURETIME("3Rend Up");

	// process messages from process
	{
		if (m_fFadingStartTime > 0)
		{
			if (((m_pSystem->GetITimer()->GetCurrTime()) - m_fFadingStartTime) > 1.5f)
			{
				m_fFadingStartTime = -1;
				SendMessage(m_szLoadMsg);
			}
		}

		while (!m_qMessages.empty())
		{
			string smsg = m_qMessages.front();
			m_qMessages.pop();
			ProcessPMessages(smsg.c_str());
		}

		// the messages can switch the game to menu or viceversa
		bPause = IsInPause(pProcess);
	}

	//update script timers
	if (m_pScriptTimerMgr)
		m_pScriptTimerMgr->Update((unsigned long)(pTimer->GetCurrTime() * 1000));

	MEASURETIME("ScrTimerUp");

	if (g_GC_Frequence->GetFVal() > 0)
	{
		// Change Script Garbage collection frequency.
		m_pSystem->SetGCFrequency(g_GC_Frequence->GetFVal());
	}

	//////////////////////////////////////////////////////////////////////////
	// Special update function for developers mode.
	//////////////////////////////////////////////////////////////////////////
	if (IsDevModeEnable())
		DevModeUpdate();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////

	MEASURETIME("EndGameUp");

	//////////////////////////////////////////////////////////////////////////
	// End Profiling Frame
	m_pSystem->GetIProfileSystem()->EndFrame();
	//////////////////////////////////////////////////////////////////////////

	return (m_bUpdateRet);
}

//////////////////////////////////////////////////////////////////////
void CXGame::UpdateDuringLoading()
{
	if(m_pServer)
		m_pServer->UpdateXServerNetwork();
}

//////////////////////////////////////////////////////////////////////////
bool CXGame::ParseLevelName(const char *szMsg,char *szLevelName,char *szMissionName)
{	
	const char *szPtr=szMsg;
	char *szDest;

	// get level & mission-name
	szPtr=szMsg+11; //skip "StartLevel "
	//find the level name
	memset(szLevelName,0,32);
	szDest=szLevelName;
	while ((*szPtr) && (*szPtr!=' '))		
		*szDest++=*szPtr++;		

	//find the mission name
	memset(szMissionName,0,32);

	if (*szPtr) //if not eos
	{
		szPtr++; //skip space
		szDest=szMissionName;			
		while (*szPtr)		
			*szDest++=*szPtr++;					
	}

	return (true);
}

//////////////////////////////////////////////////////////////////////
//! game-mainloop
bool CXGame::Run(bool& bRelaunch)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);

	if (m_bDedicatedServer)
	{
		return Update();
	}
	else
	{
		m_bRelaunch = false;
		while (1)
		{
			if (!Update())
				break;
		}

		bRelaunch = m_bRelaunch;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
#if !defined(_XBOX) && !defined(PS2) && !defined(LINUX)
#if 0
#include <Mmsystem.h> // error C2371: 'APIENTRY': redefinition; different basic types
#include ".\game.h"
#endif
#pragma comment (lib , "Winmm.lib")
#else
#define GetCurrentTime() ((unsigned int)(GetSystem()->GetITimer()->GetCurrTime() * 1000.f))
#endif

//////////////////////////////////////////////////////////////////////
bool CXGame::IsInPause(IProcess *pProcess)
{
	bool bPause = (m_bMenuOverlay && (!IsMultiplayer()));
	//check if the game is in pause or in menu mode
	#if 0
	if ((pProcess->GetFlags() & PROC_MENU) && !IsMultiplayer())
		bPause=true;
	#endif

	return (bPause);
}

//////////////////////////////////////////////////////////////////////////
void CXGame::InitVehicleCvars()
{
	IConsole *pConsole = m_pSystem->GetIConsole();
	
	//Dumprot	 9000.4
	//Dumpv		 1500.4
	//Turn		12000.0
	//Speedv		35000.0
	//Speedturnmin	       5.0

	b_dump = pConsole->CreateVariable("b_dump","2000.4",0,"This variable is not used.");
	b_dumpRot = pConsole->CreateVariable("b_dumprot","9000.4",0,"This variable is not used.");
	b_dumpV = pConsole->CreateVariable("b_dumpv","1500.4",0,"This variable is not used.");
	b_dumpVH = pConsole->CreateVariable("b_dumpvh","10000.4",0,"This variable is not used.");
	b_stand = pConsole->CreateVariable("b_stand","10000.5",0,"This variable is not used.");
	b_turn = pConsole->CreateVariable("b_turn","12000.0",0,"This variable is not used.");
	b_tilt = pConsole->CreateVariable("b_tilt","2.0",0,"This variable is not used.");
	b_speedV = pConsole->CreateVariable("b_speedv","35000.0",0,"This variable is not used.");
	b_accelerationV = pConsole->CreateVariable("b_accelerationv","100000.0",0,"This variable is not used.");
	b_speedMinTurn = pConsole->CreateVariable("b_speedminturn","5.0",0,"This variable is not used.");
	b_float = pConsole->CreateVariable("b_float","7",0,"This variable is not used.");
	b_wscale = pConsole->CreateVariable("b_wscale","2.1",0,"This variable is not used.");
	b_wscalew = pConsole->CreateVariable("b_wscalew","2.1",0,"This variable is not used.");
	b_wmomentum = pConsole->CreateVariable("b_wmomentum","500.5",0,"This variable is not used.");
	
	b_camera = pConsole->CreateVariable("b_camera","0",0,"This variable is not used.");

	p_CameraSmoothTime = pConsole->CreateVariable("p_camerasmoothtime",".6",0,"when entering/leaving vehicles.");
	p_CameraSmoothScale = pConsole->CreateVariable("p_camerasmoothscale","5",0,"when driving vehicles.");
	p_CameraSmoothVLimit = pConsole->CreateVariable("p_camerasmoothvlimit","20",0,"camera transition scale to vehicle speed when leaving moving vehicles.");

	p_LeaveVehicleImpuls = pConsole->CreateVariable("p_leavevehicleimpuls","20",0,"impilse scale to vehicle speed when leaving moving vehicles.");
	p_LeaveVehicleBrake = pConsole->CreateVariable("p_leavevehiclebrake","10",0,"speed thrashold to have breaks on when driver is out");
	p_LeaveVehicleBrakeDelay = pConsole->CreateVariable("p_leavevehiclebrakedelay","2",0,"delay before wehicle stops after driver out is out");

	p_AutoCenterDelay = pConsole->CreateVariable("p_autocenterdelay","30",0,"idle time before force autoCenter");
	p_AutoCenterSpeed = pConsole->CreateVariable("p_autocenterspeed","20",0,"speed of autoCentering - inverted (the bigger - the slower)");

	// show bboxes for static objects below helicopter
	h_drawbelow = pConsole->CreateVariable("h_drawbelow","0",0,
		"Toggles bounding boxes below helicopters.\n"
		"Usage: h_drawbelow [0/1]\n"
		"Default is 0 (off). Set 1 to display the bounding\n"
		"boxes of obstacles currently below a helicopter.");
}



bool CXGame::LoadScene(std::string name)
{
	GetISystem()->Log("Scene loading");
	std::string& path = name;
	if (gEnv->p3DEngine->LoadLevel(path.data(), ""))
	{
		if (!gEnv->IsDedicated())
		{
			//scene->setCamera("main", new CCamera());
			//m_CameraController.m_Camera = &gEnv->pRenderer->GetCamera();
			CPlayer* player = nullptr; // static_cast<CPlayer*>(scene->getObject("MyPlayer"));
			if (player != nullptr)
			{
				//player->attachCamera(scene->getCurrentCamera());
				//player->setGame(this);
				//this->SetPlayer(player);
			}
			#if 0
			m_pClient->OnLoadScene();
			#endif
		}
		return true;
	}

	return false;
}
Legacy::IInput* CXGame::GetLegacyInput()
{
	return m_pLegacyInput;
}
#if 0

void CXGame::SaveScene(std::string name, std::string as)
{
	// TODO: Fix it
	// Need implement custom save file format to save needed state
#if 0
  std::string& path = name;
  if (gEnv->pRenderer->GetISceneManager()->exist(path))
  {
	auto scene = gEnv->pRenderer->GetISceneManager()->getScene(path, this);
	scene->save(as.c_str());
  }
#endif
}
#endif
IGAME_API IGame* CreateGameInstance()
{
	CXGame* game = new CXGame();
	return (game);
}

//////////////////////////////////////////////////////////////////////////
void CXGame::GotoMenu(bool bTriggerOnSwitch)
{
	if(m_bEditor)
		return;

	if(!m_pUISystem)			// e.g. dedicated server
		return;

	if (!IsInMenu())
	{
		DeleteMessage("Switch");
		SendMessage("Switch");
	}
	else if (bTriggerOnSwitch)
	{
		assert(0 && __FUNCTION__);
		m_pUISystem->GetScriptObjectUI()->OnSwitch(1);
	}
}

//////////////////////////////////////////////////////////////////////////
void CXGame::GotoGame(bool bTriggerOnSwitch)
{
	if (m_bEditor)
	{
		return;
	}

	if (IsInMenu())
	{
		DeleteMessage("Switch");
		SendMessage("Switch");
	}
	else if (bTriggerOnSwitch)
	{
		assert(0 && __FUNCTION__);
		m_pUISystem->GetScriptObjectUI()->OnSwitch(0);
	}
}

//////////////////////////////////////////////////////////////////////////
void CXGame::MenuOn()
{
	// stop sounds and timers affected by game pause
	m_pSystem->GetISoundSystem()->Pause(true, true);
	m_pScriptTimerMgr->Pause(true);

	if (m_pSystem->GetIMusicSystem())
	{
		m_pSystem->GetIMusicSystem()->Pause(true);
		//m_pSystem->GetIMusicSystem()->Silence();
	}

	//FIXME:
	if (m_pUISystem && m_pUISystem->IsEnabled())
	{
		#if 0
		m_pSystem->GetIInput()->AddEventListener(m_pUISystem);
		#else
		GetLegacyInput()->AddEventListener(m_pUISystem);
		#endif
		m_pSystem->GetIInput()->ClearKeyState();
		m_pUISystem->GetScriptObjectUI()->OnSwitch(1);
	}

	m_bMenuOverlay = 1;

	if (!m_bEditor)
	{
		if (m_pUISystem->GetScriptObjectUI()->CanRenderGame())
		{
			m_pSystem->GetILog()->Log("UISystem: Enabled 3D Engine!");
			m_p3DEngine->Enable(1);
		}
		else
		{
			m_pSystem->GetILog()->Log("UISystem: Disabled 3D Engine!");
			m_p3DEngine->Enable(0);
		}
	}

	_SmartScriptObject pClientStuff(m_pScriptSystem, true);

	if (m_pScriptSystem->GetGlobalValue("ClientStuff", pClientStuff))
	{
		m_pScriptSystem->BeginCall("ClientStuff", "OnPauseGame");
		m_pScriptSystem->PushFuncParam(pClientStuff);
		m_pScriptSystem->EndCall();
	}
}

//////////////////////////////////////////////////////////////////////////
void CXGame::MenuOff()
{
	// resume sounds and timers affected by game pause
	m_pSystem->GetISoundSystem()->Pause(false);
	m_pSystem->GetIMusicSystem()->Pause(false);
	m_pScriptTimerMgr->Pause(false);

	//FIXME:
	if (m_pUISystem && m_pUISystem->IsEnabled())
	{
		#if 0
		m_pSystem->GetIInput()->RemoveEventListener(m_pUISystem);
		#else
		GetLegacyInput()->RemoveEventListener(m_pUISystem);
		#endif
		m_pSystem->GetIInput()->ClearKeyState();
		m_pUISystem->GetScriptObjectUI()->OnSwitch(0);

		if (GetMyPlayer())
			m_XAreaMgr.ReTriggerArea(GetMyPlayer(), GetMyPlayer()->GetPos(), false);
	}

	m_bMenuOverlay = 0;

	m_pSystem->SetIProcess(m_p3DEngine);
	m_pSystem->GetIProcess()->SetFlags(PROC_3DENGINE);

	if (!m_bEditor)
	{
		m_pSystem->GetILog()->Log("UISystem: Enabled 3D Engine!");
		m_p3DEngine->Enable(1);
	}

	_SmartScriptObject pClientStuff(m_pScriptSystem, true);

	if (m_pScriptSystem->GetGlobalValue("ClientStuff", pClientStuff))
	{
		m_pScriptSystem->BeginCall("ClientStuff", "OnResumeGame");
		m_pScriptSystem->PushFuncParam(pClientStuff);
		m_pScriptSystem->EndCall();
	}
}

//////////////////////////////////////////////////////////////////////////
void CXGame::DeleteMessage(const char* szMessage)
{
	StringQueue NewQueue;

	while (!m_qMessages.empty())
	{
		string qMessage = m_qMessages.front();

		m_qMessages.pop();

		if (qMessage != szMessage)
		{
			NewQueue.push(qMessage);
		}
	}

	m_qMessages = NewQueue;
}

//////////////////////////////////////////////////////////////////////////
IScriptObject* CXGame::GetScriptObject()
{
	if (!m_pScriptObjectGame)
	{
		return 0;
	}

	return m_pScriptObjectGame->GetScriptObject();
}

//////////////////////////////////////////////////////////////////////
void CXGame::PlaySubtitle(ISound* pSound)
{
#if !defined(LINUX)
	if (m_pMovieUser)
		m_pMovieUser->PlaySubtitles(pSound);
#endif
}

//////////////////////////////////////////////////////////////////////
vector2f CXGame::GetSubtitleSize(const string& szMessage, float sizex, float sizey, const string& szFontName, const string& szFontEffect)
{
	IFFont* pFont = m_pSystem->GetICryFont()->GetFont(szFontName.c_str());

	pFont->Reset();
	pFont->SetEffect(szFontEffect.c_str());
	pFont->SetSize(vector2f(sizex, sizey));

	wstring szwString;

	m_StringTableMgr.Localize(szMessage, szwString);

	return pFont->GetTextSizeW(szwString.c_str());
}

//////////////////////////////////////////////////////////////////////
void CXGame::WriteSubtitle(const string& szMessage, float x, float y, float sizex, float sizey, const color4f& cColor, const string& szFontName, const string& szFontEffect)
{
	IFFont* pFont = m_pSystem->GetICryFont()->GetFont(szFontName.c_str());

	pFont->Reset();
	pFont->SetEffect(szFontEffect.c_str());
	pFont->SetSize(vector2f(sizex, sizey));
	pFont->SetColor(cColor);

	wstring szwString;

	m_StringTableMgr.Localize(szMessage, szwString);

	pFont->DrawStringW(x, y, szwString.c_str());
}

//////////////////////////////////////////////////////////////////////////
IGameMods* CXGame::GetModsInterface()
{
	return m_pGameMods;
};

//////////////////////////////////////////////////////////////////////////
void CXGame::LoadingError(const char* szError)
{
	m_pRenderer->ClearColorBuffer(Legacy::Vec3(0, 0, 0));
	GetSystem()->GetIConsole()->ResetProgressBar(0);
	m_pSystem->GetIConsole()->ShowConsole(false);
	m_pSystem->GetIConsole()->SetScrollMax(600 / 2);

	m_pScriptSystem->BeginCall("Game", "OnLoadingError");
	m_pScriptSystem->PushFuncParam(GetScriptObject());
	m_pScriptSystem->PushFuncParam(szError);
	m_pScriptSystem->EndCall();
}

void CXGame::ProcessPMessages(const char* szMsg)
{
	if (stricmp(szMsg, "Quit-Yes") == 0) // quit message
	{
		m_bUpdateRet = false;
		return;
	}
	else if (stricmp(szMsg, "Relaunch") == 0) // relaunch message
	{
		m_bRelaunch	 = true;
		m_bUpdateRet = false;
		return;
	}
	else if (stricmp(szMsg, "Switch") == 0) // switch process (menu <> game)
	{
		if (m_bEditor)
			return; // we are probably in the editor?

		if (m_bMenuOverlay) // we're in menu-mode; switch to game
		{
			//check if a game is currently running before switching to
			//the 3d-engine

			CPlayer* pPlayer = 0;
			if (GetMyPlayer())
			{
				GetMyPlayer()->GetContainer()->QueryContainerInterface(CIT_IPLAYER, (void**)&pPlayer);
			}

			// there must be a client for us to be able to go out of the menu
			// if this is a singleplayer game, and the player is alive, we can go back to game
			// otherwise, if this is not multiplayer game and the player is dead, it is locked in the menu
			// also, the client must not be loading, or waiting to connect
			if (m_pClient && m_pClient->IsConnected() && m_pUISystem->GetScriptObjectUI()->CanSwitch(0))
			{
				if (IsMultiplayer() || (!pPlayer || pPlayer->m_stats.health > 0))
				{
					MenuOff();
				}
			}
		}
		else if (m_pUISystem->GetScriptObjectUI()->CanSwitch(1))
		{
			// quit if menu disabled (useful during development)
			ICVar* pCvarNoMenu = m_pSystem->GetIConsole()->CreateVariable("g_NoMenu", "0", 0);

			if (pCvarNoMenu && pCvarNoMenu->GetIVal())
			{
				m_pSystem->Quit();

				return;
			}

			MenuOn();
		}
	}
}

ITagPointManager* CXGame::GetTagPointManager()
{
	return nullptr;
}

#ifdef USE_STEAM
CSteamAchievements* CXGame::SteamAchivements()
{
	return g_SteamAchievements;
}
#endif

void CXGame::GetMemoryStatistics(ICrySizer* pSizer)
{
	unsigned size = 0;

	pSizer->AddObject(this, sizeof *this);
	pSizer->AddObject(&m_EntityClassRegistry, m_EntityClassRegistry.MemStats());
	//pSizer->AddObject(&m_XAreaMgr, m_XAreaMgr.MemStat());
	//pSizer->AddObject(&m_XDemoMgr, sizeof(m_XDemoMgr));

	const auto tpItr = m_mapTagPoints.begin();
	for (const auto &tpItr : m_mapTagPoints)
	{
		size += (tpItr.first).capacity();
		size += (tpItr.second)->MemStats();
	}
	pSizer->AddObject(&m_mapTagPoints, size);
	//pSizer->AddObject(&m_XSurfaceMgr, m_XSurfaceMgr.MemStat());
	#if 0
	if (m_pServer)
		pSizer->AddObject(m_pServer, m_pServer->MemStats());
	#endif
	if (m_pClient)
		pSizer->AddObject(m_pClient, sizeof(*m_pClient));

	pSizer->AddObject(m_pScriptObjectGame, sizeof *m_pScriptObjectGame);
	pSizer->AddObject(m_pScriptObjectInput, sizeof *m_pScriptObjectInput);

	//pSizer->AddObject(m_pScriptObjectBoids, sizeof *m_pScriptObjectBoids);
	//pSizer->AddObject(m_pScriptObjectLanguage, sizeof *m_pScriptObjectLanguage);
	//pSizer->AddObject(m_pScriptObjectAI, sizeof *m_pScriptObjectAI);

	size = 0;
	//for (ActionsEnumMap::iterator aItr = m_mapActionsEnum.begin(); aItr != m_mapActionsEnum.end(); aItr++)
	for (const auto aItr : m_mapActionsEnum)
	{
		size += (aItr.first).capacity();
		size += sizeof(ActionInfo);
		const auto* curA = &(aItr.second);
		size += curA->sDesc.capacity();
		for (unsigned int i = 0; i < curA->vecSetToActionMap.size(); i++)
			size += curA->vecSetToActionMap[i].capacity();
	}
	pSizer->AddObject(&m_mapActionsEnum, size);

	//if (m_pWeaponSystemEx)
	//	pSizer->AddObject(m_pWeaponSystemEx, m_pWeaponSystemEx->MemStats());

	size			= 0;
	#if 0
	IEntityIt* eItr = m_pSystem->GetIEntitySystem()->GetEntityIterator();
	IEntity*   ent;
	while ((ent = eItr->Next()) != NULL)
	{
		IEntityContainer* pCnt	  = ent->GetContainer();
		CPlayer*		  pPlayer = NULL;

		if (pCnt)
		{
			pCnt->QueryContainerInterface(CIT_IPLAYER, (void**)&pPlayer);
			if (pPlayer)
				size += pPlayer->MemStats();
		}
	}

	pSizer->AddObject("players from entSystem", size);
	#endif

}

#if 0
bool CXGame::SteamInit()
{
#ifndef USE_STEAM
	return true;
#else

	// инициализируем Steam
	bool bRet = SteamAPI_Init();
	if (!bRet)
	{
		if (!SteamAPI_IsSteamRunning()) {
			MessageBox(NULL, "Steam not running", "Error", MB_OK);
		}
	}

	// создаем объект SteamAchievements, если инициализация Steam удалась
	if (bRet)
	{
		g_SteamAchievements = new CSteamAchievements(g_Achievements, 1);
		// Получить имена профилей Steam текущих пользователей.
		const char* name = SteamFriends()->GetPersonaName();
		gEnv->pLog->Log("person name: %s", name);
	}

	gEnv->pLog->Log("steam api init: %d", bRet);
	return bRet;
#endif // !USE_STEAM
}
#endif
Legacy::IInput* GetLegacyInput()
{
	return gGame->GetLegacyInput();
}

//////////////////////////////////////////////////////////////////////////
/*! Load a level on the local machine (connecting with a local client)
	@param dedicated if true the local client will not be created
	@param keepclient if true the current client server connection will be kept
	@param szMapName name of the level that has to be loaded
	@param szMissionName name of the mission that has to be loaded
	@param listen allow external clients to connect
*/
//////////////////////////////////////////////////////////////////////////
void CXGame::LoadLevelCS(bool keepclient, const char *szMapName, const char *szMissionName, bool listen)
{
	// need to reset timers as well
	m_pScriptTimerMgr->Reset();

	if (m_pUISystem)
	{
		m_pUISystem->GetScriptObjectUI()->OnSwitch(0);
		m_pUISystem->StopAllVideo();
		m_p3DEngine->Enable(1);

		m_pSystem->GetILog()->Log("UISystem: Enabled 3D Engine!");
	}
#if !defined(LINUX)	
	if (m_pSystem->GetIMovieSystem())
		m_pSystem->GetIMovieSystem()->StopAllCutScenes();	
#endif		
	bool bDedicated=GetSystem()->IsDedicated();

	string strGameType = g_GameType->GetString();

	AutoSuspendTimeQuota AutoSuspender(GetSystem()->GetStreamEngine());

	assert( szMissionName != 0 );
	
	string sLevelFolder = szMapName;
	if (sLevelFolder.find('\\') == string::npos && sLevelFolder.find('/') == string::npos)
	{
		// This is just a map name, not a folder.
		sLevelFolder = GetLevelsFolder() + "/" + sLevelFolder;
	}

	IConsole* pConsole = GetSystem()->GetIConsole();
	//IInput *pInput=GetSystem()->GetIInput();					// might be 0 (e.g. dedicated server)
	auto pInput = GetLegacyInput(); // might be 0 (e.g. dedicated server)

	if(pInput)
		pInput->SetMouseExclusive(false);
		
	if (!IsMultiplayer())
	{
		m_pSystem->GetIConsole()->Clear();
		m_pSystem->GetIConsole()->SetScrollMax(600);
		m_pSystem->GetIConsole()->ShowConsole(true);

		string sLoadingScreenTexture = string("levels/") + string(szMapName) + string("/loadscreen_") + string(szMapName) + ".dds";

		m_pSystem->GetIConsole()->SetLoadingImage(sLoadingScreenTexture.c_str());
		m_pSystem->GetIConsole()->ResetProgressBar(0x7fffffff);
		m_pSystem->GetILog()->UpdateLoadingScreen("");	// just to draw the console
	}

	if (m_pClient && !keepclient)
	{
		ShutdownClient();
	}

	// start server
	if((!m_pServer || !keepclient) && !StartupServer(listen))
	{
		m_pLog->LogToConsole("Unable to load the level %s,%s [startup server failed]", sLevelFolder.c_str(),szMissionName);
		if(pInput)
			pInput->SetMouseExclusive(true);
		LoadingError("@LoadLevelError");
		return;
	}

	bool bNeedClient = !bDedicated && ((keepclient && !m_pClient) || !keepclient);

	// create local client(must be before the level is loaded)
	if(bNeedClient)
	{
		if(!StartupLocalClient())
		{
			m_pLog->LogToConsole("Unable to load the level %s,mission %s [startup client failed]", sLevelFolder.c_str(),szMissionName);
			if(pInput)
				pInput->SetMouseExclusive(true);
			LoadingError("@LoadLevelError");
			return;
		}
	}

	const char *szMission = szMissionName;
	if (!*szMissionName)
		szMission=strGameType.c_str();

	// [KIRILL] lets reset - they will be spawned anyway
	#if 0
	m_pSystem->GetAISystem()->Reset();
	#endif
	
	// refresh the current server info for incoming queries during loading
	m_pServer->GetServerInfo();

	// load the level
	if(!m_pServer->m_pISystem->LoadLevel( sLevelFolder.c_str(),szMission,false))
	{
		m_pLog->LogToConsole("Unable to load the level %s,mission %s \n", sLevelFolder.c_str(),szMissionName);
		if (pInput)
			pInput->SetMouseExclusive(true);
		LoadingError("@LoadLevelError");
		return;
	}

	// start and connect a local client
	if(bNeedClient)
	{
		if(m_pClient)
		{			
			if (IsMultiplayer() && m_pServer->m_pIServer->GetServerType()!=eMPST_LAN)
     			m_pClient->XConnect("127.0.0.1",false,true);
			else
				m_pClient->XConnect("127.0.0.1");
		}
	}
	
	if(m_pClient)
    m_pClient->OnMapChanged();
	if(m_pServer)
    m_pServer->OnMapChanged(); 
	if(pInput)
		pInput->SetMouseExclusive(true);
	AllowQuicksave(true);
};

//////////////////////////////////////////////////////////////////////////
bool CXGame::GetLevelMissions( const char *szLevelDir,std::vector<string> &missions )
{
	string sLevelPath = szLevelDir;
	
	if (!szLevelDir || sLevelPath.empty())	
		return false;

	string sEPath = sLevelPath+string("/levelinfo.xml");
	string sPaks = sLevelPath + "/*.pak";	
	OpenPacks(sPaks.c_str()); //[marco] do not call system->openpack

	bool bResult = false;
	#if 0
	XmlNodeRef root = GetSystem()->LoadXmlFile( sEPath.c_str() );
	if (root)
	{
		XmlNodeRef missionsNode = root->findChild( "Missions" );
		if (missionsNode)
		{
			// we found a mission node - the level is valid
			bResult = true;
			for (int i = 0; i < missionsNode->getChildCount(); i++)
			{
				XmlNodeRef missionNode = missionsNode->getChild(i);
				if (missionNode->isTag( "Mission" ))
				{
					const char *sMissionName = missionNode->getAttr( "Name" );
					if (sMissionName)
						missions.push_back( sMissionName );
				}
			}
		}
	}
	#endif
	
	ClosePacks(sPaks.c_str());
	return bResult;
}


//////////////////////////////////////////////////////////////////////////
bool CXGame::IsMultiplayer()
{
	// cannot be in multiplayer when in editor
	if (m_bEditor)
		return false;

	bool bServer=IsServer();
	bool bClient=IsClient();

	if(!bServer && !bClient)
		return false;

	return !bServer || !bClient || m_pServer->m_bListen;
};

//////////////////////////////////////////////////////////////////////
bool CXGame::GetCDPath(string& szCDPath)
{
	bool bRet(false);
#ifdef WIN32
	DWORD nBufferSize(GetLogicalDriveStrings(0, 0));
	if (0 < nBufferSize)
	{
		// get list of all available logical drives
		tvector rawDriveLetters(nBufferSize + 1);
		GetLogicalDriveStrings(nBufferSize, &rawDriveLetters[0]);

		// quickly scan all drives
		tvector::size_type i(0);
		while (true)
		{
			// check if current drive is cd/dvd drive
			if (DRIVE_CDROM == GetDriveType(&rawDriveLetters[i]))
			{
				// get volume name
				tvector cdVolumeName(MAX_VOLUME_ID_SIZE + 1);
				if (FALSE != GetVolumeInformation(&rawDriveLetters[i],
												  &cdVolumeName[0], (DWORD)cdVolumeName.size(), 0, 0, 0, 0, 0))
				{
					// check volume name to verify it's Far Cry's game cd/dvd
					tstring cdVolumeLabel(&cdVolumeName[0]);
					tstring farCryDisk1Label(_T( "FARCRY_1" ));
					if (cdVolumeLabel == farCryDisk1Label)
					{
						// found Far Cry's game cd/dvd, copy information and bail out
						szCDPath = &rawDriveLetters[i];
						bRet	 = true;
						break;
					}
				}
			}

			// proceed to next drive
			while (0 != rawDriveLetters[i])
			{
				++i;
			}
			++i; // skip null termination of current drive

			// check if we're out of drive letters
			if (0 == rawDriveLetters[i])
			{
				// double null termination found, bail out
				break;
			}
		}
	}
#endif
	return (bRet);
}
