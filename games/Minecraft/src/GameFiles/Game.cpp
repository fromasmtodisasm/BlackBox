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

#include "PlayerSystem.h"
#include "XVehicleSystem.h"
#include <GameMods.hpp>

#include <Client/Client.hpp>
#include <thread>

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

std::string vec_to_string(Vec3 vec)
{
	auto result = "{ x: " + std::to_string(vec.x);
	result += ", y: " + std::to_string(vec.y);
	result += ", z: " + std::to_string(vec.z) + "}";
	return result;
}

static IVec4 currentQuad{};
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
		m_Plane->moveTo(Vec3(ox, 0, oy));
		m_Plane->scale(Vec3(size, size, size));

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

CXGame::CXGame()
	: m_pSystem(nullptr)
	, m_pScriptSystem(nullptr)
	, m_pRenderer(nullptr)
	, m_p3DEngine(nullptr)
	, m_pLog(nullptr)
	, m_pCVarCheatMode(nullptr)
	, m_pScriptObjectGame(nullptr)
{
	//const auto ltime = time (NULL);
	//auto stime = (unsigned int) ltime/2;
	//srand(stime);
}

//////////////////////////////////////////////////////////////////////
//!destructor
CXGame::~CXGame()
{
	m_pScriptSystem->BeginCall("Shutdown");
	m_pScriptSystem->PushFuncParam(0);
	m_pScriptSystem->EndCall();

#if 0
	if (m_pIngameDialogMgr)
		delete m_pIngameDialogMgr;
	m_pIngameDialogMgr=NULL;
#endif

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

#if 0
	if (m_pUISystem)
	{
		m_pUISystem->Release();
	}
	SAFE_DELETE(m_pUISystem);
#endif

#if 0
	CScriptObjectUI::ReleaseTemplate();
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

#if 0
	if (m_pRenderer && (m_nPlayerIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nPlayerIconTexId);
	if (m_pRenderer && (m_nVehicleIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nVehicleIconTexId);
	if (m_pRenderer && (m_nBuildingIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nBuildingIconTexId);
	if (m_pRenderer && (m_nUnknownIconTexId>=0))
		m_pRenderer->RemoveTexture(m_nUnknownIconTexId);
#endif

#if 0
	SAFE_DELETE(m_pUIHud);
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
	assert(pWeaponSystemEx);

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
	#if 0
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
	#else
	NOT_IMPLEMENTED;
	#endif
}

//////////////////////////////////////////////////////////////////////
//! reset the game before a level reloading
//! this function allows the reloading of all scripts
//! and wipes out all textures from the 3dengine
void CXGame::Reset()
{
	#if 0
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
		//m_pSystem->GetILog()->Log("UISystem: Enabled 3D Engine!");
	}
	if (m_pCurrentUI)
		m_pCurrentUI->Reset();
	if (m_pUIHud)
		m_pUIHud->Reset();

	if (GetMyPlayer())
		GetMyPlayer()->SetNeedUpdate(true);
	#else
	NOT_IMPLEMENTED;
	#endif
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

	#if 0
	CScriptObjectUI::InitializeTemplate(m_pScriptSystem);
	#endif

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
			#if 0
			m_pUISystem = new CUISystem;

			if (m_pUISystem)
			{
				m_pUISystem->Create(this, m_pSystem, m_pScriptSystem, "Scripts/MenuScreens/UISystem.lua", 1);
			}
			else
			{
				m_pLog->Log("Failed to create UI System!");
			}
			#endif
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


#if 0
void CXGame::OnRenderer_BeforeEndFrame()
{
	auto   posY			  = 150.f;
	size_t currentEntry	  = 0;
	auto   PrintMenuEntry = [&, this](const char* szText, bool skip = false) -> bool
	{
		SDrawTextInfo info;
		float		  rightMargin = 60;
		info.font				  = m_Font;
		auto& color				  = info.color;
		Vec3  activeColor		  = Vec3(174, 237, 181) / 255.f;
		Vec3  menuColor			  = Vec3(73, 92, 79) / 255.f;
		bool  active			  = false;

		{
			auto cq = currentQuad;
			if (MouseInQuad((int)rightMargin, (int)posY, (int)m_Font->TextWidth(szText), g_FontSize))
			{
				m_CurrentMenuEntry = currentEntry;
				m_MenuActived	   = mouseClicked;
				mouseClicked	   = false;
			}

			if (m_CurrentMenuEntry == currentEntry)
			{
				UCol col({1,
						  0,
						  0,
						  1});
				currentQuad = Vec4((int)rightMargin,(int)posY, (int)m_Font->TextWidth(szText), g_FontSize);
				Vec4 q = currentQuad; 
				//float dummy;
				//gEnv->pRenderer->UnProjectFromScreen((float)currentQuad.x, (float)currentQuad.y, 0.f, &q.x, &q.y, &dummy);
				//gEnv->pRenderer->UnProjectFromScreen((float)currentQuad.z, (float)currentQuad.w, 0.f, &q.z, &q.w, &dummy);
				auto draw_quad = [](Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, UCol col)
				{
					auto render = gEnv->pRenderer->GetIRenderAuxGeom();
					render->DrawTriangle(p1, col, p2, col, p3, col);
					render->DrawTriangle(p3, col, p4, col, p1, col);
				};
				float x = 40, y = 0, z = -40;
				{
					const UCol col1(50, 125, 0, 20);
					draw_quad({-1, -1, z}, {-1, 1, z}, {1, 1, z}, {1, -1, z}, col1);
				}

				#if 0
				#define Unproject(a,b) gEnv->pRenderer->UnProjectFromScreen(a.x, a.y, a.z, &b.x, &b.y, &b.z);
				Vec3 A;
				Vec3 _A(q.x, posY, 0);
				Unproject(_A, A);
				Vec3 B;
				Vec3 _B(q.x + q.z, posY, 0);
				Unproject(_B, B);
				Vec3 C;
				Vec3 _C(q.x + q.z, posY + g_FontSize, 0);
				Unproject(_C, C);
				Vec3 D;
				Vec3 _D(q.x, posY + g_FontSize, 0);
				Unproject(_D, D);
				gEnv->pAuxGeomRenderer->DrawQuad(
					A, col,
					B, col,
					C, col,
					D, col
					);
				#endif
					
				active	  = true;
				//info.font = m_SelectedEntryFont;
				//posY += 24;
			}
			//auto font = 
			if (active) menuColor = activeColor;
			color[0] = menuColor.g; //green
			color[1] = menuColor.b;
			color[2] = 1.0;			//alpha
			color[3] = menuColor.r; //red
			auto px	 = rightMargin;
			gEnv->pRenderer->Draw2dText(px, posY, szText, info);
			posY += 48;
		}
		m_MenuEnries = currentEntry;
		currentEntry++;
		if (skip) currentEntry--;

		return active && m_MenuActived;
	};
	static bool optionsOpened  = false;
	static bool graphicsOpened = false;
	static bool inputOpened	   = false;
	static bool help		   = false;
	static bool campaign	   = false;
	static bool multiplayer	   = false;
	static bool mods		   = false;
	static bool quitRequested  = false;
	{
		//m_pRenderer->SetViewport(0, 0, m_pRender->GetWidth() / 2, m_pRender->GetHeight() / 2);

		if (g_bRenderGame)
		{
			switch (m_Mode)
			{
			case CXGame::FPS:
				break;
			case CXGame::MENU:
				if (!optionsOpened)
				{
					if (m_playDemo)
					{
						auto stop = PrintMenuEntry("Back");
						menuOnTopLevel	   = false;
						auto t		   = gEnv->pTimer->GetCurrTime();
						auto r		   = cos(t);
						auto g		   = sin(t);
						auto b		   = 1.f;
						gEnv->pRenderer->SetClearColor((Vec3{r, g, b} + 1.f) * 0.5f);
						m_CurrentMenuEntry = 0;
						m_pClient->m_CameraController.ProcessMouseMovement(cos(gEnv->pTimer->GetCurrTime() * 0.1f) * 0.8f, sin(gEnv->pTimer->GetCurrTime() * 0.1f) * 0.5f);
						//m_pClient->m_CameraController.ProcessMouseMovement(cos(gEnv->pTimer->GetCurrTime()*0.001f) * 0.1f , 0);
						m_pClient->m_CameraController.ProcessKeyboard(Movement::FORWARD, m_deltaTime);

#if 0
								auto ang = cam->GetAngles();
								cam->SetAngles({ang.x, cos(gEnv->pTimer->GetCurrTime()), ang.z});
#endif
						auto k = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							m_playDemo = false;
						}
						m_playDemo = !stop;
						goto end;
					}
					else if (help)
					{
						auto stop = PrintMenuEntry("Back");
						menuOnTopLevel	   = false;
						//m_CurrentMenuEntry = 0;
						PrintMenuEntry("Movements - WASD");
						PrintMenuEntry("Orientation - Mouse");
						PrintMenuEntry("Console - ~");
						PrintMenuEntry("(c) BlackBox");

						auto k = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							help = false;
							m_CurrentMenuEntry = 0;
						}
						help = !stop;
						goto end;
					}
					else if (campaign)
					{

						menuOnTopLevel	   = false;
						auto selected = m_CurrentMenuEntry == currentEntry;
						if (PrintMenuEntry("New Game"))
						{
						}
						if (PrintMenuEntry("Save Game"))
						{
						}

						selected = m_CurrentMenuEntry == currentEntry;
						if (PrintMenuEntry("Load Game"))
						{
						}
						if (selected)
						{
							PrintMenuEntry("$4Saved games not found");
						}

						auto k = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							campaign = false;
						}
						goto end;
					}
					else if (multiplayer)
					{
						static bool findServer = false;
						menuOnTopLevel	   = false;
						if (findServer)
						{
							PrintMenuEntry("$4Servers not founds");
						}
						else
						{
							if (findServer = PrintMenuEntry("Find server"))
							{
								m_CurrentMenuEntry = 0;
							}
							if (PrintMenuEntry("Create server"))
							{
							}
						}



						auto k = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							multiplayer = false;
						}
						goto end;
					}
					else if (mods)
					{
						menuOnTopLevel		   = false;
						int	 n				   = 0;
						for (const auto& mod : mods_str)
						{
							if (PrintMenuEntry(mod))
							{
								selected_mod = n;	
							}
							n++;
						}
						auto k = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							mods = false;
						}
						goto end;
						
					}
					else if (quitRequested)
					{
						menuOnTopLevel		   = false;
						int	 n				   = 0;
						if (PrintMenuEntry("Yes"))
						{
							gEnv->pSystem->Quit();
						}
						if (PrintMenuEntry("No"))
						{
							quitRequested = false;
						}
						auto k = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							quitRequested = false;
						}
						goto end;
						
					}

					static char buffer[256];
					sprintf(buffer, "USER: %s", gEnv->pSystem->GetUserName());
					PrintMenuEntry(buffer);
					sprintf(buffer, "$7Current MOD: %s", mods_str[selected_mod]);
					PrintMenuEntry(buffer);
					if (PrintMenuEntry("Return to Game"))
					{
						GotoGame();
					}
					if (campaign = PrintMenuEntry("Campaign"))
					{
						menuOnTopLevel	   = false;
						m_CurrentMenuEntry = 0;
					}

					if (multiplayer = PrintMenuEntry("Multiplayer"))
					{
						menuOnTopLevel	   = false;
						m_CurrentMenuEntry = 0;
					}
					if (optionsOpened = PrintMenuEntry("Options"))
					{
						menuOnTopLevel	   = false;
						m_CurrentMenuEntry = 0;
					}
					if (mods = PrintMenuEntry("Mods"))
					{
						menuOnTopLevel	   = false;
						m_CurrentMenuEntry = 0;
					}
					m_playDemo = PrintMenuEntry("Demo Loop");
					if (PrintMenuEntry("Credits"))
					{
						CryLogAlways("Credits activated");
					}
					if (PrintMenuEntry("Editor"))
					{
						std::thread notepad([]
											{ gEnv->pConsole->ExecuteString(R"(#os.execute("code"))"); });

						notepad.detach();
					}
					if (help = PrintMenuEntry("Help"))
					{
						menuOnTopLevel	   = false;
						m_CurrentMenuEntry = 0;
					}
					if (quitRequested = PrintMenuEntry("Quit"))
					{
						menuOnTopLevel	   = false;
						m_CurrentMenuEntry = 0;
						//gEnv->pSystem->Quit();
					}
					menuOnTopLevel = true;
				}
				else
				{
					menuOnTopLevel = false;
					auto k		   = gEnv->pInput->GetDevice(0, EInputDeviceType::eIDT_Keyboard);
					if (graphicsOpened)
					{
						static char grphics[256];
						sprintf(grphics, "Width: %d", gEnv->pRenderer->GetWidth());
						PrintMenuEntry(grphics);

						sprintf(grphics, "Height: %d", gEnv->pRenderer->GetHeight());
						PrintMenuEntry(grphics);

						sprintf(grphics, "Display info: %c", r_displayinfo->GetIVal() ? '+' : '-');
						if (PrintMenuEntry(grphics))
						{
							r_displayinfo->Set(!r_displayinfo->GetIVal());
						}
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							graphicsOpened = false;
							CryLogAlways("Graphics closed on frame: %d", gEnv->pRenderer->GetFrameID());
							CryLogAlways("_____________________");
						}
					}
					else if (inputOpened)
					{
						static char grphics[256];
						auto		i_d = gEnv->pConsole->GetCVar("i_debug");
						sprintf(grphics, "Debug: %c", i_d->GetIVal() ? '+' : '-');
						if (PrintMenuEntry(grphics))
						{
							i_d->Set(!i_d->GetIVal());
						}
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							inputOpened = false;
						}
					}
					else
					{
						// Print options
						graphicsOpened = PrintMenuEntry("Graphics");
						inputOpened	   = PrintMenuEntry("Input");
						if (k->InputState("escape", EInputState::eIS_Pressed) && m_CanBackStep)
						{
							optionsOpened = false;
						}
					}
					m_CanBackStep = false;
				}
			end:
				static char mouse[256];
				float		mx, my;
				gEnv->pHardwareMouse->GetHardwareMousePosition(&mx, &my);
				sprintf(mouse, "MouseX: %f MouseY: %f", mx, my);
				PrintMenuEntry(mouse);
				sprintf(mouse, "Button quad: x: %d y: %d w: %d h %d", currentQuad.x, currentQuad.y, currentQuad.z, currentQuad.w);
				PrintMenuEntry(mouse);
				break;
			case CXGame::FLY:
				break;
			case CXGame::EDIT:
				break;
			default:
				break;
			}
		}
	}
	DrawHud(fps);
}
#endif

static ITexture* splash = nullptr;

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
		#if 0
		if (!m_bMenuOverlay || !m_pUISystem || m_pUISystem->GetScriptObjectUI()->CanRenderGame())
		#endif
		{
			m_p3DEngine->Enable(1);
		}
		#if 0
		else
		{
			m_p3DEngine->Enable(0);
		}
		#endif
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

#if 0
void CXGame::ExecScripts()
{
}

void CXGame::DrawHud(float fps)
{
	//m_pRenderer->SetViewport(0, 0, m_pRender->GetWidth(), m_pRender->GetHeight());
	if (r_displayinfo->GetIVal() != 0)
	{
		DisplayInfo(fps);
	}
}

void CXGame::DisplayInfo(float fps)
{
	size_t num_objects;
	num_objects = m_3DEngine->GetLoadedObjectCount();
	auto line	= m_pRenderer->GetHeight();
	auto step	= 18;

	const std::string mode = m_Mode == MENU	 ? "MENU"
							 : m_Mode == FPS ? "FPS"
							 : m_Mode == FLY ? "FLY"
											 : "EDIT";

	// Info
	TextRenderInfo info(m_Font, Vec4(1.f, 1.f, 0.6f, 1.0));
	SDrawTextInfo  dti = info.getDTI();
	SDrawTextInfo  MenuDTI;
	MenuDTI.color[0] = 1;
	MenuDTI.color[0] = 1;
	MenuDTI.color[0] = 1;
	MenuDTI.color[0] = 1;
	MenuDTI.font	 = m_Font;

	//
	auto render = m_pSystem->GetIRenderer();

	//===========

	m_Font->SetXPos(0);
	m_Font->SetYPos(18);
	auto& text	= info.m_Text;
	auto& color = info.m_Color;
	//auto camera = m_World->getActiveScene()->getCurrentCamera();

	//auto objPos = m_World->getActiveScene()->selectedObject()->second->m_transform.position;
	info.AddLine("FPS: " + std::to_string(fps));
	//info.AddLine("NUM OBJECTS: "			+ std::to_string(num_objects));
	info.AddLine("Current mode: " + mode);
	info.AddLine("Width = " + std::to_string(m_pRenderer->GetWidth()) + "Height = " + std::to_string(m_pRender->GetHeight()));
	//info.AddLine("Active scene: "			+ m_World->getActiveScene()->name);
	//info.AddLine("Selected Object: "	+ m_World->getActiveScene()->selectedObject()->first);
	//info.AddLine("  visible: "				+ std::to_string(m_World->getActiveScene()->selectedObject()->second->visible()));
	/*
  info.AddLine("  Pos: "						+
	std::to_string(objPos.x) + ", " +
	std::to_string(objPos.y) + ", " +
	std::to_string(objPos.z) + "; ");
  */
	//info.AddLine("Camera speed: " + std::to_string(camera->MovementSpeed->GetFVal()));
	//auto camPos = camera->getPosition();
	//auto camRot = camera->getRotation();
	/*
  auto pos = "Pos: " +
	std::to_string(camPos.x) + ", " +
	std::to_string(camPos.y) + ", " +
	std::to_string(camPos.z) + "; " +
	"Yaw: " +
	std::to_string(camRot.y) + "; " +
	"Pitch: " +
	std::to_string(camRot.x) + "; "
	;
  */

	for (auto& text : info.m_Text)
	{
		render->PrintLine(text.c_str(), dti);
	}

	//render->PrintLine("To hide depth buffer press <;>\n", dti);
	//render->PrintLine((std::string("Camera position = ") + vec_to_string(m_CameraController.RenderCamera()->transform.position) + "\n").c_str(), dti);

	info.m_Color = Vec4(1.0f, 0.f, 0.f, 1.0f);
	//render->PrintLine(pos.c_str(), info.getDTI());
	if (canDragViewPortWidth)
		render->PrintLine("CanDrag\n", info.getDTI());
	if (mousePressed)
		render->PrintLine("Mouse pressed\n", info.getDTI());

	if (m_Mode == MENU)
	{
		Vec2 c;
		m_pSystem->GetIHardwareMouse()->GetHardwareMouseClientPosition(&c.x, &c.y);
		render->PrintLine(("Cursor: " + std::to_string(c.x) + std::string(", ") + std::to_string(/*m_pRenderer->GetHeight() - */ c.y)).c_str(), info.getDTI());
		m_Font->SetYPos((float)m_pRenderer->GetHeight() / 2);
		{
#if 0
			auto& lpp	 = m_IntersectionState.m_LastPickedPos;
			auto pos = std::to_string(lpp.x) + ",";
			pos += std::to_string(lpp.y) + ",";
			pos += std::to_string(lpp.z) + ";\n";
			render->PrintLine((std::string("Last picking pos: ") + pos).data(), info.getDTI());
			render->PrintLine((std::string("Current distant: ") + std::to_string(m_IntersectionState.m_CurrentDistant)).data(), info.getDTI());
#endif
		}
	}
}
#endif

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
		#if 0
		m_pUISystem->GetScriptObjectUI()->OnSwitch(1);
		#endif
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
		#if 0
		m_pUISystem->GetScriptObjectUI()->OnSwitch(0);
		#endif
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
		#if 0
		if (m_pUISystem->GetScriptObjectUI()->CanRenderGame())
		{
			m_pSystem->GetILog()->Log("UISystem: Enabled 3D Engine!");
			m_p3DEngine->Enable(1);
		}
		else
		#endif
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

	#if 0
	if (m_pUISystem && m_pUISystem->IsEnabled())
	{
		m_pSystem->GetIInput()->RemoveEventListener(m_pUISystem);
		m_pSystem->GetIInput()->ClearKeyState();
		m_pUISystem->GetScriptObjectUI()->OnSwitch(0);

		if (GetMyPlayer())
			m_XAreaMgr.ReTriggerArea(GetMyPlayer(), GetMyPlayer()->GetPos(), false);
	}

	m_bMenuOverlay = 0;

	m_pSystem->SetIProcess(m_p3DEngine);
	m_pSystem->GetIProcess()->SetFlags(PROC_3DENGINE);
	#endif

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
	m_pRenderer->ClearColorBuffer(Vec3(0, 0, 0));
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
}

void CXGame::UpdateDuringLoading()
{
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