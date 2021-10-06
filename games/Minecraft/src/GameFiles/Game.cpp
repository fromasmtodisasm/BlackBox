#include <Game.hpp>
#include <GameObject.hpp>
#include <TagPoint.hpp>

#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/Texture.hpp>

#include <ScriptObjects/ScriptObjectInput.hpp>
#include <ScriptObjects/ScriptObjectStream.hpp>
#include <ScriptObjects/ScriptObjectTest.hpp>

#include "PlayerSystem.h"
#include "XVehicleSystem.h"

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

void CGame::PreRender()
{
	//glCheck(glViewport(0, 0, 1366, 768));
}

CGame::CGame()
	: m_pSystem(nullptr)
	, m_pScriptSystem(nullptr)
	, m_pRender(nullptr)
	, m_pInput(nullptr)
	, m_inputHandler(nullptr)
	, m_3DEngine(nullptr)
	, m_pLog(nullptr)
	, m_lastTime(0)
	, listener(nullptr)
	, m_Font(nullptr)
	, m_Console(nullptr)
	, g_scene(nullptr)
	, r_displayinfo(nullptr)
	, r_profile(nullptr)
	, r_cap_profile(nullptr)
	, m_pCVarCheatMode(nullptr)
	, m_pScriptObjectGame(nullptr)
{
	//const auto ltime = time (NULL);
	//auto stime = (unsigned int) ltime/2;
	//srand(stime);
}

void CGame::InitVariables()
{
}

CGame::~CGame()
{
	SaveHistory();
	m_pScriptSystem->BeginCall("Shutdown");
	m_pScriptSystem->PushFuncParam(0);
	m_pScriptSystem->EndCall();

	// shutdown the client if there is one
	ShutdownClient();

	// shutdown the server if there is one
	ShutdownServer();

#ifdef USE_STEAM
	// Выключаем Steam
	SteamAPI_Shutdown();
	// Удаляем SteamAchievements
	if (g_SteamAchievements)
		delete g_SteamAchievements;
#endif
	CScriptObjectGame::ReleaseTemplate();
	CScriptObjectInput::ReleaseTemplate();
	CScriptObjectTest::ReleaseTemplate();
	CScriptObjectClient::ReleaseTemplate();
	CScriptObjectServer::ReleaseTemplate();
	CScriptObjectStream::ReleaseTemplate();
#if 0
	g_scene->Release();
	r_displayinfo->Release();
	r_profile->Release();
	r_cap_profile->Release();
	m_pCVarCheatMode->Release();

	g_LevelName->Release();
	g_MissionName->Release();
	g_StartMission->Release();

	sv_port->Release();
	sv_mapcyclefile->Release();
	sv_cheater_kick->Release();
	sv_cheater_ban->Release();

	sv_timeout->Release();
	cl_timeout->Release();
	cl_loadtimeout->Release();
	cl_snooptimeout->Release();
	cl_snoopretries->Release();
	cl_snoopcount->Release();

	g_playerprofile->Release();

	cv_game_Difficulty->Release();
	cv_game_Aggression->Release();
	cv_game_Accuracy->Release();
	cv_game_Health->Release();
	cv_game_AllowAIMovement->Release();
	cv_game_AllAIInvulnerable->Release();
	cv_game_GliderGravity->Release();
	cv_game_GliderBackImpulse->Release();
	cv_game_GliderDamping->Release();
	cv_game_GliderStartGravity->Release();
	cv_game_physics_quality->Release();
#endif

	SAFE_DELETE(m_pVehicleSystem);
	SAFE_DELETE(m_pPlayerSystem);
	//shutdown script stuff
	SAFE_DELETE(m_pScriptObjectGame);
	SAFE_DELETE(m_pScriptObjectInput);

	// Release the action map
	SAFE_RELEASE(m_pIActionMapManager);
	// release the tags
	if (!m_mapTagPoints.empty())
	{
		TagPointMap::iterator ti;
		for (ti = m_mapTagPoints.begin(); ti != m_mapTagPoints.end(); ti++)
			delete ti->second;
	}
}
#if defined USE_STEAM && defined GLSL_EDITOR
static GLSLEditor* glslEditor = nullptr;
#endif //  USE_STEAM
bool CGame::Init(ISystem* pSystem, bool bDedicatedSrv, bool bInEditor, const char* szGameMod)
{
	if (!SteamInit())
		return false;
	m_pSystem		   = pSystem;
	m_bDedicatedServer = bDedicatedSrv;
	m_pRender		   = m_pSystem->GetIRenderer();
	m_pInput		   = m_pSystem->GetIInput();
	m_pScriptSystem	   = m_pSystem->GetIScriptSystem();
	m_pLog			   = m_pSystem->GetILog();
	m_Console		   = m_pSystem->GetIConsole();
	m_3DEngine		   = gEnv->p3DEngine;
	if (!bDedicatedSrv)
	{
		m_pInput->AddEventListener(this);
		//m_pInput->AddEventListener(&m_CameraController);
		//gEnv->pSystem->GetIHardwareMouse()->AddListener(&m_CameraController);
		gEnv->pSystem->GetIHardwareMouse()->SetHardwareMouseClientPosition(static_cast<float>(m_pRender->GetWidth()), static_cast<float>(m_pRender->GetHeight()));
	}
	pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CGame");
	m_pNetwork		= m_pSystem->GetINetwork();
	m_bUpdateRet	= true;
	m_HardwareMouse = m_pSystem->GetIHardwareMouse();

	m_pVehicleSystem = new CVehicleSystem();
	m_pPlayerSystem	 = new CPlayerSystem();

#ifdef USE_GUI
	glslEditor = new GLSLEditor;
#endif
	gui::init();

#if 0
  if (!m_pNetwork->Init())
	return false;

  m_pClient = m_pNetwork->CreateClient();
  if (!m_pClient || !m_pClient->Init())
	return false;

  m_pServer = m_pNetwork->CreateServer();
  if (!m_pServer)
	return false;
#endif

	InitConsoleVars();
	InitCommands();
	InitScripts();
	m_pDevMode = std::make_unique<CDevMode>();

	SmartScriptObject Gui(m_pScriptSystem, true);
	if (!m_pScriptSystem->GetGlobalValue("Gui", *Gui))
	{
		CryError("Cannot find Gui table in scripts (wrong working folder?)");
		return false;
	}
	else
	{
		Script::CallMethod(Gui, "Init");
	}
	//m_Console->ExecuteString("@Player:OnInit()");

	// init key-bindings
	if (!m_bDedicatedServer)
		InitInputMap();

	LoadConfiguration("", "game.cfg");

	if (!m_bDedicatedServer)
	{
		//m_pSystem->GetIConsole()->ShowConsole(0);
#if 0
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
#endif
	}
	else
		m_pSystem->GetIConsole()->ShowConsole(false);

	InitConsoleCommands();

	DevModeInit();

	InitPlayer();
	//m_pInput->ShowCursor(false);
	m_pInput->GrabInput(true);

	if (m_pRender)
	{
		m_Font = gEnv->pRenderer->GetIFont();
		m_Font->Init("arial.ttf", g_FontSize,g_FontSize);

		m_SelectedEntryFont = gEnv->pRenderer->GetIFont();
		m_SelectedEntryFont->Init("arial.ttf", 60,60);
	}

	// other
	//TODO: FIX IT
#if 0
  mousePrev = sf::Mouse::GetPos();
  mouseDelta = sf::Vector2i(0, 0);
#endif
#if CURSOR_FIXED
	cursor.loadFromSystem(sf::Cursor::Arrow);
	m_pSystem->GetIWindow()->setCursor(reinterpret_cast<Cursor*>(&cursor));
#endif

	m_pClient = new CClient(this);
#if 0
	m_Console->ExecuteFile("res/scripts/postinit.cfg");
#else
	m_Console->ExecuteString("@Player:PostInit()");
#endif
	m_pClient->Init();
	if (m_Console->GetCVar("nsightDebug"))
	{
		m_Console->ExecuteString("r_displayinfo 0");
	}

	m_QuadTree = std::make_shared<QuadTree>(8, 10, 0, 0, color3(1, 0, 0));
	//m_QuadTreeRender = std::make_shared<CRender>(m_pRender);
	//TreeRender treeRender(m_QuadTreeRender.get());
	LoadHistory();
	gEnv->pRenderer->RegisterCallbackClient(this);


	auto fnLoad = gEnv->pScriptSystem->GetFunctionPtr("LoadLevel");

	Script::Call(gEnv->pScriptSystem, fnLoad, "test.lua");

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CGame::InitClassRegistry()
{
	m_EntityClassRegistry.Init(m_pSystem);
	CPlayerSystem*	pPlayerSystem  = GetPlayerSystem();
	CVehicleSystem* pVehicleSystem = GetVehicleSystem();
	//CWeaponSystemEx *pWeaponSystemEx = GetWeaponSystemEx();	// m10

	assert(pPlayerSystem);
	assert(pVehicleSystem);
	//assert( pWeaponSystemEx );

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

#if 0
			if(strcmp("Projectile",entity_type)==0)
			{
				// cannot be loaded at that point - other scripts must be loaded before
				pWeaponSystemEx->AddProjectileClass(ClassId);
			}
#endif
		}
	} while (entCls);
	return true;
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


void CGame::OnRenderer_BeforeEndFrame()
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
		//m_pRender->SetViewport(0, 0, m_pRender->GetWidth() / 2, m_pRender->GetHeight() / 2);

		if (g_bRenderGame)
		{
			switch (m_Mode)
			{
			case CGame::FPS:
				break;
			case CGame::MENU:
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
			case CGame::FLY:
				break;
			case CGame::EDIT:
				break;
			default:
				break;
			}
		}
	}
	DrawHud(fps);
}


static ITexture* splash = nullptr;
bool			 CGame::Update()
{
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
	{
		// TODO: FIX IT
		m_deltaTime = m_pSystem->GetDeltaTime();
		m_time += m_deltaTime;
		fps = 1.0f / m_deltaTime;
		ExecScripts();

		if (bRenderFrame)
		{
#if 0
			SmartScriptObject Gui(m_pScriptSystem,true);
			if (!m_pScriptSystem->GetGlobalValue("Gui",*Gui))
			{
				CryError("Cannot find Gui table in scripts (wrong working folder?)");
				return false;
			}
#endif
			#if 0
			m_pSystem->RenderBegin();
			#endif
			m_pClient->Update();

			if (g_bRenderGame)
			{
				Render();
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

void CGame::ExecScripts()
{
}

void CGame::DrawHud(float fps)
{
	//m_pRender->SetViewport(0, 0, m_pRender->GetWidth(), m_pRender->GetHeight());
	if (r_displayinfo->GetIVal() != 0)
	{
		DisplayInfo(fps);
	}
}

void CGame::DisplayInfo(float fps)
{
	size_t num_objects;
	num_objects = m_3DEngine->GetLoadedObjectCount();
	auto line	= m_pRender->GetHeight();
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
	info.AddLine("Width = " + std::to_string(m_pRender->GetWidth()) + "Height = " + std::to_string(m_pRender->GetHeight()));
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
		render->PrintLine(("Cursor: " + std::to_string(c.x) + std::string(", ") + std::to_string(/*m_pRender->GetHeight() - */ c.y)).c_str(), info.getDTI());
		m_Font->SetYPos((float)m_pRender->GetHeight() / 2);
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

bool CGame::Run(bool& bRelaunch)
{
	//splash = gEnv->pRenderer->LoadTexture("fcsplash.bmp", 0, 0);
	typedef ITexPic ITexture;
	//auto bg = gEnv->pRenderer->LoadTexture("console_background.png", 0, 0);
	//gEnv->pConsole->SetImage((ITexPic*)bg, true);
	m_pLog->Log("[OK] Game started");
	StartupServer(true, "test_server");

	m_bRelaunch = false;
	GotoMenu();
	gEnv->pConsole->ShowConsole(false);
	while (1)
	{
		if (!Update())
			break;
	}

	bRelaunch = m_bRelaunch;

	return true;
}

bool CGame::LoadScene(std::string name)
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
				this->SetPlayer(player);
			}
			m_pClient->OnLoadScene();
		}
		return true;
	}

	return false;
}

void CGame::SaveScene(std::string name, std::string as)
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

void CGame::SetRenderState()
{
#if 0
	m_pRender->SetState(IRenderer::State::DEPTH_TEST, true);
	m_pRender->SetState(IRenderer::State::BLEND, false);
	if (culling)
	{
		m_pRender->SetState(IRenderer::State::CULL_FACE, true);
		m_pRender->SetCullMode(IRenderer::CullMode::BACK);
	}
	else
	{
		m_pRender->SetState(IRenderer::State::CULL_FACE, false);
	}
#endif
}

void CGame::SetPlayer(CPlayer* player)
{
	m_player = player;
}

void CGame::SetCamera(CCamera* camera)
{
	//m_active_camera = camera;
	//m_World->setCamera(camera);
}

void CGame::Render()
{
	m_pSystem->Render();
	m_pClient->Render();
}

IGAME_API IGame* CreateIGame()
{
	CGame* game = new CGame();
	return (game);
}

bool CGame::OnInputEvent(const SInputEvent& event)
{
	{
		bool	   retflag;
		const bool retval = ShouldHandleEvent(event, retflag);
		if (retflag)
			return retval;
	}
	const bool result = false;
	//if (!IsInPause())
		OnInputEventProxy(event);
	PersistentHandler(event);
	return result;
}

void CGame::PersistentHandler(const SInputEvent& event)
{
	auto useBoxFilter = m_Console->GetCVar("bf");
	////////////////////////
	const bool keyPressed = (event.deviceType == eIDT_Keyboard || event.deviceType == eIDT_Mouse) && event.state == eIS_Pressed;
	const bool control	  = event.modifiers & eMM_Ctrl;
	const bool shift	  = event.modifiers & eMM_Shift;
	bool	   alt		  = event.modifiers & eMM_Alt;
	////////////////////////
	if (keyPressed)
	{
		if (event.keyId == eKI_Semicolon)
			openShadowMap = !openShadowMap;
		if (event.keyId >= eKI_F1 && event.keyId <= eKI_F12)
		{
			if (control)
			{
				//DevMode_SavePlayerPos(event.keyId - eKI_F1, "BloomTest");
			}
			else if (shift)
			{
				//DevMode_LoadPlayerPos(event.keyId - eKI_F1, "BloomTest");
			}
		}
		switch (event.keyId)
		{
		case eKI_R:
		{
			if (control)
			{
				gEnv->pRenderer->Sh_Reload();
			}
			break;
		}
		case eKI_Pause:
		{
			m_bInPause = !m_bInPause;
			break;
		}
		default:
			break;
		}
	}
}

void CGame::GotoGame()
{
	//if (m_player != nullptr)
	{
		//m_World->GetActiveScene()->getCurrentCamera()->mode = CCamera::Mode::FPS;

		m_bInPause = false;
		m_Mode	   = FPS;
		m_pInput->ShowCursor(false);
		m_pInput->GrabInput(true);
		m_Console->ShowConsole(false);
		m_pSystem->EnableGui(false);
	}
}

void CGame::GotoFly()
{
	m_Mode = FLY;
}

void CGame::GotoEdit()
{
	m_Mode = EDIT;
}

void CGame::ShowMenu()
{
}

bool CGame::InitPlayer()
{
	// TODO: rewirte it
#if 0
  if (m_player != nullptr)
  {
	delete m_player;
  }
  if ((m_player = reinterpret_cast<CPlayer*>(m_scene->getObject("MyPlayer"))) != nullptr)
  {
	m_player->attachCamera(m_World->GetActiveScene()->getCurrentCamera());
	m_player->setGame(this);
	return true;
  }
#endif
	return false;
}

bool CGame::FpsInputEvent(const SInputEvent& event)
{
	/*
  if (m_Console->IsOpened())
	return false;
  */

	////////////////////////
	const bool keyPressed = event.deviceType == eIDT_Keyboard && event.state == eIS_Pressed;
	const bool control	  = event.modifiers & eMM_Ctrl;
	const bool shift	  = event.modifiers & eMM_Shift;
	const bool alt		  = event.modifiers & eMM_Alt;
	////////////////////////
	auto camera = gEnv->pRenderer->GetCamera();
	if (keyPressed)
	{
		switch (event.keyId)
		{
			/*
	  case eKI_P:
		isWireFrame = !isWireFrame;
		return true;
	  */
		case eKI_NP_0:
			//m_SelectedBox = ++m_SelectedBox % m_testObjects.size();
			return true;
		case eKI_M:
			camera.mode = CCamera::Mode::FLY;
			m_Mode		= Mode::FLY;
			return true;
#if 0
		case eKI_H:
			m_IntersectionState.picked->m_AABB.min.x += 1;
			m_IntersectionState.picked->m_AABB.max.x += 1;
			return true;
		case eKI_L:
			m_IntersectionState.picked->m_AABB.min.x -= 1;
			m_IntersectionState.picked->m_AABB.max.x -= 1;
			return true;
		case eKI_K:
			m_IntersectionState.picked->m_AABB.min.z += 1;
			m_IntersectionState.picked->m_AABB.max.z += 1;
			return true;
		case eKI_J:
			m_IntersectionState.picked->m_AABB.min.z -= 1;
			m_IntersectionState.picked->m_AABB.max.z -= 1;
			return true;
		case eKI_U:
		{
			int module = 1;
			if (control)
				module = -1;
			m_IntersectionState.picked->m_AABB.min.y += module;
			m_IntersectionState.picked->m_AABB.max.y += module;
			return true;
		}
#endif
		case eKI_Escape:
			GotoMenu();
			return true;
		case eKI_P:
			camera.MovementSpeed += 5.0f;
			return true;
		case eKI_E:
			//m_inputHandler->mouseLock(false);
			m_Mode = EDIT;
			return true;
		/*case eKI_M:
	  camera->MovementSpeed->Set(camera->MovementSpeed->GetFVal() + 5.0f);
	  return true;*/
		case eKI_B:
			culling = !culling;
			return true;
		case eKI_F9:
			if (shift)
				++currPP;
			else
				--currPP;
			//m_World->GetActiveScene()->setPostProcessor(postProcessors[currPP % postProcessors.size()]);
			return true;
		case eKI_Enter:
			if (alt == true)
				GotoFullscreen();
			return true;
		default:
			return false;
		}
	}
	//return m_player->OnInputEvent(event);
	return false;
}

bool CGame::FlyInputEvent(const SInputEvent& event)
{
	////////////////////////
	const bool keyPressed = event.deviceType == eIDT_Keyboard && event.state == eIS_Pressed;
	bool	   control	  = event.modifiers & eMM_Ctrl;
	bool	   shift	  = event.modifiers & eMM_Shift;
	bool	   alt		  = event.modifiers & eMM_Alt;
	////////////////////////
	if (keyPressed)
	{
		switch (event.keyId)
		{
		case eKI_P:
			isWireFrame = !isWireFrame;
			return true;
		case eKI_Backspace:
			return true;
		case eKI_Space:
			GotoGame();
			return true;
		case eKI_Escape:
			//TODO: FIX IT
			//m_inputHandler->mouseLock(false);
			m_Mode = Mode::MENU;
			return true;
		default:
			return false;
		}
	}
	return false;
	//return m_player->OnInputEvent(event);
}

bool CGame::MenuInputEvent(const SInputEvent& event)
{
	////////////////////////
	const bool keyPressed = (event.deviceType == eIDT_Keyboard || event.deviceType == eIDT_Mouse) && event.state == eIS_Pressed;
	bool	   control	  = event.modifiers & eMM_Ctrl;
	bool	   shift	  = event.modifiers & eMM_Shift;
	bool	   alt		  = event.modifiers & eMM_Alt;
	static int activatedFrame = 0;
	static int backStepFrame  = 0;
	int		   currentFrame	  = gEnv->pRenderer->GetFrameID();

			//backStepFrame = currentFrame;
	if (m_MenuActived && currentFrame > activatedFrame)
	{
		m_MenuActived = false;
		activatedFrame = 0;
	}
	////////////////////////
	if (keyPressed)
	{
		m_CanBackStep = false;
		switch (event.keyId)
		{
		case eKI_Mouse1: 
			mouseClicked = true;
			return true;
		case eKI_Enter:
			m_MenuActived = true;
			activatedFrame = currentFrame;
			return true;
		case eKI_Escape:
			//m_inputHandler->mouseLock(false);
			//m_Mode = Mode::MENU;
			//Stop();

			if (currentFrame > (backStepFrame))
			{
				m_CanBackStep = true;
			}
			else
			{
				m_CanBackStep = false;
			}
			backStepFrame = currentFrame;

			if (menuOnTopLevel)
				GotoGame();
			return true;
		case eKI_J:
			return true;
		case eKI_K:
			return true;

		case eKI_Up:
			m_CurrentMenuEntry = std::max(1ULL, m_CurrentMenuEntry) - 1;
			return true;
		case eKI_Down:
			m_CurrentMenuEntry = std::min(m_MenuEnries - 1, m_CurrentMenuEntry) + 1;
			return true;

		default:
			return false;
		}
	}
	return false;
}

bool CGame::DefaultInputEvent(const SInputEvent& event)
{
	return false;
}

bool CGame::EditInputEvent(const SInputEvent& event)
{
#if 0
  ////////////////////////
  bool keyPressed = event.deviceType == eIDT_Keyboard && event.state == eIS_Pressed;
  bool control = event.modifiers & eMM_Ctrl;
  bool shift = event.modifiers & eMM_Shift;
  bool alt = event.modifiers & eMM_Alt;
  ////////////////////////
  if (keyPressed)
  {
	IStatObj* obj = m_World->GetActiveScene()->selectedObject()->second;
	switch (event.keyId)
	{
	case eKI_Escape:
	  gotoMenu();
	  return true;
	case eKI_I:
	  obj->move(Movement::FORWARD);
	  return true;
	case eKI_U:
	  obj->move(Movement::BACKWARD);
	  return true;
	case eKI_J:
	  obj->move(Movement::DOWN);
	  return true;
	case eKI_K:
	  obj->move(Movement::UP);
	  return true;
	case eKI_V:
	  obj->setVisibility(!obj->visible());
	  return true;
	case eKI_Tab:
	  if (shift)
	  {
		m_World->GetActiveScene()->selectPrevObject();
	  }
	  else
	  {
		m_World->GetActiveScene()->selectNextObject();
	  }
	  return true;
	//default:
	  //return m_player->OnInputEvent(event);
	}
  }
  else
  {
	//return m_player->OnInputEvent(event);
  }
#endif

	return false;
}

bool CGame::OnInputEventProxy(const SInputEvent& event)
{
	switch (m_Mode)
	{
	case CGame::FPS:
		//if (m_player != nullptr) return FpsInputEvent(event);
		return FpsInputEvent(event);
	case CGame::MENU:
		return MenuInputEvent(event);
	case CGame::FLY:
		return FlyInputEvent(event);
	case CGame::EDIT:
		return EditInputEvent(event);
	default:
		break;
	}
	return false;
}

bool CGame::ShouldHandleEvent(const SInputEvent& event, bool& retflag)
{
	const bool keyPressed = event.deviceType == eIDT_Keyboard && event.state == eIS_Pressed;
	bool	   control	  = event.modifiers & eMM_Ctrl;
	bool	   shift	  = event.modifiers & eMM_Shift;
	bool	   alt		  = event.modifiers & eMM_Alt;
	retflag				  = true;

	if (keyPressed)
	{
		switch (event.keyId)
		{
		case eKI_Tilde:
			if (m_Console->IsOpened())
				m_Console->ShowConsole(false);
			else
				m_Console->ShowConsole(true);
			return true;
		default:
			break;
		}
	}

	if (m_Console->IsOpened())
		return false;
	retflag = false;
	return {};
}

void CGame::ProcessPMessages(const char* szMsg)
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

bool CGame::IsInPause()
{
	return m_bInPause;
}

void CGame::Stop()
{
	GetISystem()->Log("Stopping");
	m_bUpdateRet = false;
}

void CGame::PostRender()
{
}

void CGame::GotoMenu()
{
	m_Mode	   = MENU;
	m_bInPause = true;
	m_pInput->ShowCursor(true);
	m_pInput->GrabInput(false);
	m_pSystem->EnableGui(true);
}

void CGame::GotoFullscreen()
{
	isFullScreen = !isFullScreen;
}

bool CGame::InitScripts()
{
	m_pScriptObjectGame = new CScriptObjectGame();
	m_pScriptObjectGame->InitializeTemplate(m_pScriptSystem);

	m_pScriptObjectInput = new CScriptObjectInput;
	CScriptObjectInput::InitializeTemplate(m_pScriptSystem);

	auto SOT = new CScriptObjectTest();
	SOT->InitializeTemplate(m_pScriptSystem);
	SOT->Init(m_pScriptSystem, this);
	CScriptObjectTest::ReleaseTemplate();
	SAFE_DELETE(SOT);

#if 0
  m_pScriptClient = new CScriptObjectClient();
  m_pScriptClient->InitializeTemplate(m_pScriptSystem);

  m_pScriptServer = new CScriptObjectServer();
  m_pScriptServer->InitializeTemplate(m_pScriptSystem);
#endif

	m_pScriptObjectGame->Init(m_pSystem->GetIScriptSystem(), this);
	m_pScriptObjectInput->Init(m_pScriptSystem, this, m_pSystem);
#if 0
  m_pScriptServer->Init(m_pSystem->GetIScriptSystem(), m_pServer);
  m_pScriptClient->Init(m_pSystem->GetIScriptSystem(), m_pClient);
#endif

	CScriptObjectStream::InitializeTemplate(m_pScriptSystem);

	InitClassRegistry();

	// execute the "main"-script (to pre-load other scripts, etc.)
	m_pScriptSystem->ExecuteFile("scripts/main.lua", true, false);
	m_pScriptSystem->BeginCall("Init");
	m_pScriptSystem->PushFuncParam(0);
	m_pScriptSystem->EndCall();

	fps = 35.f;
	class toogle_viewport_drag : public IConsoleCommand
	{
		CGame* game;

	  public:
		toogle_viewport_drag(CGame* game)
			: game(game)
		{
		}
		bool execute(CommandDesc& cd)
		{
			game->can_drag_vp = !game->can_drag_vp;
			return true;
		}
	};

	//m_Console->AddCommand("toogle_viewport_drag", new toogle_viewport_drag(this));
#if 0
  m_Console->AddCommand(
	"enumd",
	R"(
		local formats = System:EnumDisplayFormats()
		for i=1, #formats do
			Console:PrintLine("["..i.."]".. formats[i].width .. " x " .. formats[i].height .. " x " .. formats[i].bpp)
		end
		Console:PrintLine(%2)
		Console:PrintLine(%1)
		)",
	0,
	"Enum Display formats"
  );
#endif

	m_pScriptSystem->ExecuteFile("scripts/utils.lua");

	return true;
}

bool CGame::TestScriptSystem(bool& retflag)
{
	retflag		   = true;
	m_playerObject = m_pScriptSystem->CreateEmptyObject();
	if (!m_pScriptSystem->GetGlobalValue("Player", m_playerObject))
	{
		m_playerObject->Release();
		m_pSystem->Log("\002 ERROR: can't find player table ");
		return false;
	}
	const char* name;
	int			age;
	m_playerObject->GetValue("name", name);
	m_playerObject->GetValue("age", age);
	CryLogAlways("Player name: %s", name);
	CryLogAlways("Player age: %d", age);

#if 0
  HSCRIPTFUNCTION psina;
  m_playerObject->GetValue("TestChanges", psina);

  m_playerObject->SetValue("name", "Psina");
  //m_pScriptSystem->BeginCall(m_playerObject, "TestChanges");
  IScriptObject* console = nullptr;
  m_pScriptSystem->BeginCall("Console", "PrintLine");
  m_pScriptSystem->PushFuncParam(m_ScriptObjectConsole->GetScriptObject());
  m_pScriptSystem->PushFuncParam("alskdjfa;lsdjf call!!!");
  m_pScriptSystem->EndCall(console);

  m_pScriptSystem->BeginCall("player", "TestChanges");
  m_pScriptSystem->PushFuncParam(m_playerObject);
  m_pScriptSystem->PushFuncParam("Test HSCRIPTFUNCTION call!!!");
  m_pScriptSystem->EndCall(console);

  m_pScriptSystem->BeginCall(m_playerObject, "TestChanges");
  m_pScriptSystem->PushFuncParam(m_playerObject);
  m_pScriptSystem->PushFuncParam("Test lkjakldfj call!!!");
  m_pScriptSystem->EndCall(console);

  HSCRIPTFUNCTION PrintLine = 0;
  console->GetValue("PrintLine", PrintLine);
  m_pScriptSystem->BeginCall(PrintLine);
  m_pScriptSystem->PushFuncParam(console);
  m_pScriptSystem->PushFuncParam("=====================Test HSCRIPTFUNCTION call!!!");
  m_pScriptSystem->EndCall();

  int n;
  m_pScriptSystem->BeginCall(console, "PrintLine");
  m_pScriptSystem->PushFuncParam(console);
  m_pScriptSystem->PushFuncParam("Call by table reference");
  m_pScriptSystem->EndCall(console);

  m_playerObject->GetValue("name", name);
  m_Console->PrintLine("Player name: %s", name);
#endif
	retflag = false;
	return {};
}

void CGame::ReloadScripts()
{
}

bool CGame::GetModuleState(EGameCapability eCap)
{
	return false;
}

void CGame::UpdateDuringLoading()
{
}

IXAreaMgr* CGame::GetAreaManager()
{
	return nullptr;
}

ITagPointManager* CGame::GetTagPointManager()
{
	return nullptr;
}

void CGame::MainMenu()
{
}

#ifdef USE_STEAM
CSteamAchievements* CGame::SteamAchivements()
{
	return g_SteamAchievements;
}
#endif

void CGame::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:
		m_isActive = bool(wparam);
		break;
	case ESYSTEM_EVENT_FAST_SHUTDOWN:
		SendMessage("Quit");
	default:
		break;
	}
}

void CGame::GetMemoryUsage(ICrySizer* pSizer) const
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

bool CGame::SteamInit()
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

#ifdef USE_GUI
void CGame::Gui::Update()
{
	windows.Draw();
}

void CGame::Gui::Graphics::Draw()
{
	if (ImGui::TreeNode("Graphics"))
	{
		auto cnt = gEnv->pRenderer->EnumDisplayFormats(nullptr);
		ImGui::TreePop();
	}
}

void CGame::Gui::Input::Draw()
{
	if (ImGui::TreeNode("Input"))
	{
		ImGui::TreePop();
	}
}

CGame::Gui::Windows::Windows()
{
	widgets.push_back(std::make_shared<Common>());
	widgets.push_back(std::make_shared<Input>());
	widgets.push_back(std::make_shared<Graphics>());
}

void CGame::Gui::Windows::Draw()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Quit"))
			{
				gEnv->pSystem->GetIGame()->SendMessage("Quit");
			}
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();
	ControlPanel::AddEntry([&]
						   {
							   for (const auto w : widgets)
							   {
								   w->Draw();
								   ImGui::Separator();
							   }
						   });
}

void CGame::Gui::Common::Draw()
{
	static char var_buffer[256];
	union CVarValue
	{
		bool		b;
		int			i;
		float		f;
		const char* s;
	};
	static CVarValue cvv;
	static ICVar*	 cur_var = nullptr;

	auto change_value = [&](ICVar* v)
	{
		switch (v->GetType())
		{
		case CVAR_INT:
		{
			cvv.i = v->GetIVal();
			break;
		}
		case CVAR_FLOAT:
		{
			cvv.f = v->GetFVal();
			break;
		}
		case CVAR_STRING:
		{
			auto s = v->GetString();
			auto l = strlen(s);
			strncpy(var_buffer, s, l);
			var_buffer[l] = 0;
			cvv.s		  = var_buffer;
			break;
		}
		default:
			break;
		}
	};

	if (ImGui::TreeNode("Common"))
	{
		static char search_pattern[256] = "sv";
		static bool need_dump			= true;
		ImGui::Button("Test");
		if (ImGui::Button("Quit"))
		{
			gEnv->pSystem->GetIGame()->SendMessage("Quit");
		}
		//ImGui::BeginTooltip();
		if (ImGui::Checkbox("Show Vars", &console_vars) || need_dump)
		{
			need_dump = false;
			vd.SetSubstr(search_pattern);
			vd.vars.resize(0);
			gEnv->pConsole->DumpCVars(&vd);
			if (!vd.vars.empty())
			{
				cur_var = gEnv->pConsole->GetCVar(vd.vars[cvr]);
				change_value(cur_var);
			}
		}
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Open listbox with console variables");
		//ImGui::EndTooltip();

		if (console_vars && ImGui::BeginChild("Console Variables"))
		{
			if (ImGui::InputText("Search", search_pattern, 256))
				need_dump = true;
			if (ImGui::ListBox("", &cvr, vd.vars.data(), vd.vars.size(), 10))
			{
				cur_var = gEnv->pConsole->GetCVar(vd.vars[cvr]);
				change_value(cur_var);
			}
			if (ImGui::IsItemHovered())
				ImGui::SetTooltip("Help:\n%s", cur_var->GetHelp());
			auto v	  = cur_var;
			auto name = cur_var->GetName();

			switch (v->GetType())
			{
			case CVAR_INT:
			{
				ImGui::InputCVarInt(v, name, &cvv.i);
				break;
			}
			case CVAR_FLOAT:
			{
				ImGui::InputCVarFloat(v, name, &cvv.f);
			}
			case CVAR_STRING:
			{
				ImGui::InputCVarString(v, name, const_cast<char*>(cvv.s));
				break;
			}
			default:
				break;
			}
			ImGui::EndChild();
		}
		ImGui::TreePop();
	}
}
#endif
