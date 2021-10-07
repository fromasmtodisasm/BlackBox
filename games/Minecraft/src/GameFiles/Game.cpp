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

#include "ScriptObjectUI.h"

#include "PlayerSystem.h"
#include "XVehicleSystem.h"
#include <GameMods.hpp>

#include <Client/Client.hpp>
#include <thread>

#include "UISystem.h"
#include "UIHud.h"
#include "IngameDialog.h"

static int g_FontSize = 32;

#ifdef USE_GUI
#	include <imgui.h>
#	include <imgui_internal.h>
#endif

#include "TextEditorDemo.hpp"

#include <BlackBox/GUI/ControlPanel.hpp>

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

#if 0
class CRender : public IQuadTreeRender {
public:
	CRender(IRenderer* pRender)
	{

	}
  void draw_plane(double ox, double oy, double size, color3 color) override {
		m_Plane->moveTo(Legacy::Vec3(ox, 0, oy));
		m_Plane->scale(Legacy::Vec3(size, size, size));

  }
	std::shared_ptr<Object> m_Plane;
};

class TreeRender : public ITreeVisitorCallback {
public:
  TreeRender(IQuadTreeRender *render) : render(render) {}
  // Inherited via ITreeVisitorCallback
  virtual void BeforVisit(QuadTree *qt) override {

  }
  virtual void OnLeaf(QuadTree *qt, bool is_last, int level) override {
	render->draw_plane(qt->m_x, qt->m_y, qt->m_size, qt->m_color);
  }

  IQuadTreeRender *render = nullptr;
};

class TreeObject : public Object
{
	TreeObject(TreeRender *treeRender) : m_TreeRender(std::unique_ptr<TreeRender>(treeRender)) {
	
	
	}
	virtual void draw(SRendParams& renderParams) final
	{
		Object::draw(renderParams);
	}

	std::unique_ptr<TreeRender> m_TreeRender;
};
namespace {
#	if 0
  sf::RenderWindow& getWindow()
  {
	return *static_cast<sf::RenderWindow*>(GetISystem()->GetIWindow()->getHandle());
  }
#	endif
}
#endif

namespace gui
{
	void init()
	{
		// (optional) set browser properties
	}
	void update()
	{
	}

} // namespace gui

//float CameraRayLength = 40.f;

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
	#if 0
#if !defined(LINUX)
	// to avoid all references to movie user in this file
	m_pMovieUser = new CMovieUser(this);
#endif
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

#if 0
#	if !defined(LINUX)
	if (m_pMovieUser)
	{
		if (m_pSystem)
		{
#		if 0
			IMovieSystem *pMovieSystem=m_pSystem->GetIMovieSystem();
			if (pMovieSystem)
			{
				if (pMovieSystem->GetUser()==m_pMovieUser)
					pMovieSystem->SetUser(NULL);
			}
#		else
#		endif
		}
		delete m_pMovieUser;
	}
#	endif
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
#if 0
	CScriptObjectLanguage::ReleaseTemplate();
#endif

#if 0
	CScriptObjectAI::ReleaseTemplate();
#endif
	CScriptObjectServer::ReleaseTemplate();
#if 0
	CScriptObjectServerSlot::ReleaseTemplate();
#endif
	CScriptObjectClient::ReleaseTemplate();
	CScriptObjectStream::ReleaseTemplate();

	#if 0
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
	#endif

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
	SAFE_DELETE(m_pPlayerSystem);

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
	#if 0
	SAFE_DELETE(m_pTimeDemoRecorder);
	#endif
	SAFE_DELETE(m_pGameMods);

	#if 0
	delete m_pTagPointManager;
	#endif
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
				#if 0
				// cannot be loaded at that point - other scripts must be loaded before
				pWeaponSystemEx->AddProjectileClass(ClassId);
				#else
				NOT_IMPLEMENTED_V;
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
	#if 0

	std::vector<string> vLoadedWeapons;

	for (int i = 0; i != m_pWeaponSystemEx->GetNumWeaponClasses(); ++i)
		vLoadedWeapons.push_back(m_pWeaponSystemEx->GetWeaponClass(i)->GetName());

	m_pWeaponSystemEx->Reset();
	#endif
	#if 0
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
	#else
	NOT_IMPLEMENTED;
	#endif

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

	#if 0
	// Unload all music.
	if (m_pSystem->GetIMusicSystem())
		m_pSystem->GetIMusicSystem()->Unload();
	#endif

	//allow to reload scripts with(LoadScript)
	m_pScriptSystem->SetGlobalToNull("_localplayer");
	if (m_pScriptSystem)
		m_pScriptSystem->UnloadScripts();

	#if 0
	m_pWeaponSystemEx->Reset();
	#endif
	m_XSurfaceMgr.Reset();
	m_XAreaMgr.Clear();
	ClearTagPoints();
	#if 0
#if !defined(LINUX)
	if (m_pSystem->GetIMovieSystem())
		m_pSystem->GetIMovieSystem()->Reset(false);
#endif
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
	#if 0
	return m_pServer ? m_pServer->m_pISystem : m_pClient ? m_pClient->m_pISystem
														 : NULL;
	#else
	NOT_IMPLEMENTED_V;
	#endif
}

bool CXGame::Init(ISystem* pSystem, bool bDedicatedSrv, bool bInEditor, const char* szGameMod)
{
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
	#if 0
#if !defined(LINUX)
	IMovieSystem* pMovieSystem = m_pSystem->GetIMovieSystem();
	if (pMovieSystem)
		pMovieSystem->SetUser(m_pMovieUser);
#endif
	if (!m_pTimeDemoRecorder)
		m_pTimeDemoRecorder = new CTimeDemoRecorder(pSystem);
	#endif

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
	#if 0
	m_pWeaponSystemEx = new CWeaponSystemEx();
	#endif
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
	#if 0
	m_pScriptObjectLanguage = new CScriptObjectLanguage;
	CScriptObjectLanguage::InitializeTemplate(m_pScriptSystem);
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

	#if 0
	m_pScriptObjectBoids->Init(m_pScriptSystem, m_pSystem, m_pFlockManager);
	m_pScriptObjectLanguage->Init(m_pScriptSystem, &m_StringTableMgr);
	m_pScriptObjectAI->Init(m_pScriptSystem, m_pSystem, this);
	#endif

	#if 0
	CScriptObjectServerSlot::InitializeTemplate(m_pScriptSystem);
	#endif
	CScriptObjectClient::InitializeTemplate(m_pScriptSystem);
	CScriptObjectStream::InitializeTemplate(m_pScriptSystem);

	m_pScriptTimerMgr = new CScriptTimerMgr(m_pScriptSystem, m_pSystem->GetIEntitySystem(), this);

	// making some constants accessable to the script
	#if 0
	m_pScriptSystem->SetGlobalValue("FireActivation_OnPress", ePressing);
	m_pScriptSystem->SetGlobalValue("FireActivation_OnRelease", eReleasing);
	m_pScriptSystem->SetGlobalValue("FireActivation_OnHold", eHolding);
	#endif

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
	#if 0
	m_XSurfaceMgr.Init(m_pScriptSystem, m_p3DEngine, GetSystem()->GetIPhysicalWorld());
	#endif

	// init key-bindings
	if (!m_bDedicatedServer)
		InitInputMap();

	// create various console-commands/variables
	InitConsoleCommands();

	// loading the main language-string-table
	#if 0
	if (!m_StringTableMgr.Load(GetSystem(), *m_pScriptObjectLanguage, g_language->GetString()))
		m_pLog->Log("cannot load language file [%s]", g_language->GetString());
	#endif

	// creating HUD interface
	m_pLog->Log("Initializing UI");
	#if 0
	m_pUIHud = new CUIHud(this, m_pSystem);
	#endif

	LoadConfiguration("", "game.cfg");

	//////////////////////////////////////////////////////////////////////////
	// Materials
	// load materials (once before all, this info stays till we quit the game - no need to load material later)
	// first load normal materials
	#if 0
	m_XSurfaceMgr.LoadMaterials("scripts/materials");
	#endif

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
		#if 0
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
		#endif
	}

	if (!bInEditor)
		m_pEntitySystem->SetDynamicEntityIdMode(true);

#if !defined(_XBOX) && !defined(PS2) && !defined(LINUX)
	SetCursor(NULL);
#endif

	DevModeInit();
	m_bOK				 = true;
	e_deformable_terrain = NULL;
	//FIXME: client should be created in other place
	m_pClient			 = new CClient(this);
	LoadScene("empty");
	m_pClient->Init();

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
		if (!m_bMenuOverlay || !m_pUISystem || m_pUISystem->GetScriptObjectUI()->CanRenderGame()
			)
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

	ITimer *pTimer=m_pSystem->GetITimer();
	#if 0
	pTimer->MeasureTime("EnterGameUp");
	#endif


	static const auto& render_game	= true;
	const bool		   bRenderFrame = !m_bDedicatedServer && gEnv->pRenderer != nullptr;
	static int		   num_frames	= 0xffff;
	//*m_CameraController.CurrentCamera() = m_pSystem->GetViewCamera();
	#if 0
	m_pSystem->Update(0, IsInPause());
	#endif
#ifdef USE_STEAM
	SteamAPI_RunCallbacks();
#endif
	bool bPause = false; //IsInPause(nullptr);
	{
			
		if (!bPause || (m_pClient /*&& !m_pClient->IsConnected() */))
		{
			// network start
			FRAME_PROFILER("GameUpdate:Client", PROFILE_GAME);

			// update client
			if (m_pClient)
			{
				#if 0
				m_pClient->UpdateClientNetwork();
				pTimer->MeasureTime("Net");
				#endif

				assert(m_pClient);
				m_pClient->Update();

				if (m_pClient->DestructIfMarked()) //  to make sure the client is only released in one place - here
					m_pClient = 0;
			}

			#if 0
			pTimer->MeasureTime("ClServ Up");
			#endif

			////////UPDATE THE NETWORK
			// [Anton] moved from after the rendering, so that the server has access to the most recent physics data
			// update server
			if (m_pServer)
			{
				FRAME_PROFILER("GameUpdate:Server", PROFILE_GAME);

				#if 0
				m_pServer->Update();
				#endif
			}

			#if 0
			pTimer->MeasureTime("EndServUp");
			#endif
		}

		m_pNetwork->UpdateNetwork(); // used to update things like the UBI.com services

		if (g_bRenderGame)
		{
			m_pClient->Render();
		}

//PROFILER_PUSH_CPU_MARKER("DrawHud", Utils::COLOR_CYAN);
#if 0
			m_pScriptSystem->BeginCall(Gui, "OnDraw");
			m_pScriptSystem->PushFuncParam(Gui);
			m_pScriptSystem->EndCall();
#endif

#if USE_UI
		if (g_DrawUI)
		{
			glslEditor->Update();
			m_Gui.Update();
			gui::update();
		}
#endif

		//PROFILER_POP_CPU_MARKER();

	}
	//////////////////////////////////////////////////////////////////////////
	// Special update function for developers mode.
	//////////////////////////////////////////////////////////////////////////
	if (IsDevModeEnable())
		DevModeUpdate();
	//////////////////////////////////////////////////////////////////////////
	while (!m_qMessages.empty())
	{
		string smsg = m_qMessages.front();
		m_qMessages.pop();
		ProcessPMessages(smsg.c_str());
	}
	//////////////////////////////////////////////////////////////////////////

	#if 0
	if (bRenderFrame)
		m_pSystem->RenderEnd();
	#endif

	//num_frames--;

	if (!num_frames)
		CryFatalError("Game Over!");
	return m_bUpdateRet && num_frames > 0;
}

//////////////////////////////////////////////////////////////////////
void CXGame::UpdateDuringLoading()
{
	#if 0
	if(m_pServer)
		m_pServer->UpdateXServerNetwork();
	#else
	NOT_IMPLEMENTED;
	#endif
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
	if ((pProcess->GetFlags() & PROC_MENU) && !IsMultiplayer())
		bPause=true;

	return (bPause);
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
			m_pClient->OnLoadScene();
		}
		return true;
	}

	return false;
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
	#if 0
	m_pSystem->GetISoundSystem()->Pause(true, true);
	#endif
	m_pScriptTimerMgr->Pause(true);

	#if 0
	if (m_pSystem->GetIMusicSystem())
	{
		m_pSystem->GetIMusicSystem()->Pause(true);
		//m_pSystem->GetIMusicSystem()->Silence();
	}
	#endif

	//FIXME:
	#if 0
	if (m_pUISystem && m_pUISystem->IsEnabled())
	{
		m_pSystem->GetIInput()->AddEventListener(m_pUISystem);
		m_pSystem->GetIInput()->ClearKeyState();
		m_pUISystem->GetScriptObjectUI()->OnSwitch(1);
	}
	#endif

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
	#if 0
	m_pSystem->GetISoundSystem()->Pause(false);
	m_pSystem->GetIMusicSystem()->Pause(false);
	#endif
	m_pScriptTimerMgr->Pause(false);

	//FIXME:
	#if 0
	if (m_pUISystem && m_pUISystem->IsEnabled())
	{
		m_pSystem->GetIInput()->RemoveEventListener(m_pUISystem);
		m_pSystem->GetIInput()->ClearKeyState();
		m_pUISystem->GetScriptObjectUI()->OnSwitch(0);

		if (GetMyPlayer())
			m_XAreaMgr.ReTriggerArea(GetMyPlayer(), GetMyPlayer()->GetPos(), false);
	}
	#endif

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
	#if 0
#if !defined(LINUX)
	if (m_pMovieUser)
		m_pMovieUser->PlaySubtitles(pSound);
#endif
	#else
	NOT_IMPLEMENTED;
	#endif
}

//////////////////////////////////////////////////////////////////////
vector2f CXGame::GetSubtitleSize(const string& szMessage, float sizex, float sizey, const string& szFontName, const string& szFontEffect)
{
	#if 0
	IFFont* pFont = m_pSystem->GetICryFont()->GetFont(szFontName.c_str());

	pFont->Reset();
	pFont->SetEffect(szFontEffect.c_str());
	pFont->SetSize(vector2f(sizex, sizey));

	wstring szwString;

	m_StringTableMgr.Localize(szMessage, szwString);

	return pFont->GetTextSizeW(szwString.c_str());
	#else
	NOT_IMPLEMENTED_V;
	#endif
}

//////////////////////////////////////////////////////////////////////
void CXGame::WriteSubtitle(const string& szMessage, float x, float y, float sizex, float sizey, const color4f& cColor, const string& szFontName, const string& szFontEffect)
{
	#if 0
	IFFont* pFont = m_pSystem->GetICryFont()->GetFont(szFontName.c_str());

	pFont->Reset();
	pFont->SetEffect(szFontEffect.c_str());
	pFont->SetSize(vector2f(sizex, sizey));
	pFont->SetColor(cColor);

	wstring szwString;

	m_StringTableMgr.Localize(szMessage, szwString);

	pFont->DrawStringW(x, y, szwString.c_str());
	#else
	NOT_IMPLEMENTED;
	#endif
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
	if (stricmp(szMsg, "Quit") == 0) // quit message
	{
		GetISystem()->Log("Quiting");
		m_bUpdateRet = false;
		return;
	}
	else if (stricmp(szMsg, "Relaunch") == 0) // relaunch message
	{
		m_bRelaunch	 = true;
		m_bUpdateRet = false;
		return;
	}
	else if (strnicmp(szMsg, "SaveGame", 8) == 0) // save current game
	{
		if (!m_bEditor)
		{
			const char* sname = "quicksave";
			if (strlen(szMsg) > 8)
			{
				sname = szMsg + 9;
			}
			Save(sname, NULL, NULL);
		}
	}
}

void CXGame::ReloadScripts()
{
	NOT_IMPLEMENTED;
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
	#if 0
		pSizer->AddObject(m_pClient, sizeof(*m_pClient));
	#else
		m_pClient->GetMemoryUsage(pSizer);
	#endif

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