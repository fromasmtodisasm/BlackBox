#include <BlackBox/Core/Platform/Platform.hpp>
#include <Game.hpp>
#include <GameObject.hpp>

#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/Input/IHardwareMouse.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/Texture.hpp>

#include <ScriptObjects/ScriptObjectInput.hpp>
#include <ScriptObjects/ScriptObjectTest.hpp>

#include "PlayerSystem.h"
#include "XVehicleSystem.h"


#include <cctype>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

int render_camera = 0;
int movement_camera = 0;
namespace
{
	Vec3 RandomVector(Vec3 left, Vec3 right, float floor = 5)
	{
		const Vec3 vec(rand(), rand(), rand());
		return left + static_cast <Vec3> (vec) /( static_cast <Vec3> (Vec3(static_cast<float>(RAND_MAX))/(right - left)));
	}

	std::string vec_to_string(Vec3 vec)
	{
		auto result = "{ x: " + std::to_string(vec.x);
		result += ", y: " + std::to_string(vec.y);
		result += ", z: " + std::to_string(vec.z) + "}";
		return result;
	}
} // namespace

std::vector<Vec3> lineBuffer;

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
	virtual void draw(SRenderParams& renderParams) final
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

//float CameraRayLength = 40.f;

void CGame::PreRender()
{
	//glCheck(glViewport(0, 0, 1366, 768));
}

CGame::CGame()
	:
	m_pSystem(nullptr),
	m_pScriptSystem(nullptr),
	m_pRender(nullptr),
	m_pInput(nullptr),
	m_inputHandler(nullptr),
	m_3DEngine(nullptr), m_pLog(nullptr),
	m_lastTime(0),
	listener(nullptr),
	m_Font(nullptr),
	m_Console(nullptr),
	g_scene(nullptr),
	r_displayinfo(nullptr),
	r_profile(nullptr),
	r_cap_profile(nullptr),
	m_pCVarCheatMode(nullptr),
	m_pScriptObjectGame(nullptr),
	m_CameraController(),
	m_IntersectionState()
{

	//const auto ltime = time (NULL);
	//auto stime = (unsigned int) ltime/2;
	//srand(stime);
}

void CGame::initVariables()
{
}

CGame::~CGame()
{
	m_pScriptSystem->BeginCall("Shutdown");
	m_pScriptSystem->PushFuncParam(0);
	m_pScriptSystem->EndCall();

	// shutdown the client if there is one
	ShutdownClient();

	// shutdown the server if there is one
	ShutdownServer();
}

bool CGame::Init(ISystem* pSystem, bool bDedicatedSrv, bool bInEditor, const char* szGameMod)
{
	m_pSystem		   = pSystem;
	m_bDedicatedServer = bDedicatedSrv;
	m_pRender		   = m_pSystem->GetIRenderer();
	m_pInput		   = m_pSystem->GetIInput();
	m_pScriptSystem	= m_pSystem->GetIScriptSystem();
	m_pLog			   = m_pSystem->GetILog();
	m_Console		   = m_pSystem->GetIConsole();
	m_3DEngine		   = gEnv->p3DEngine;
	if (!bDedicatedSrv)
	{
		m_pInput->AddEventListener(this);
		m_pInput->AddEventListener(&m_CameraController);
		gEnv->pSystem->GetIHardwareMouse()->AddListener(&m_CameraController);
		gEnv->pSystem->GetIHardwareMouse()->SetHardwareMouseClientPosition(static_cast<float>(m_pRender->GetWidth()), static_cast<float>(m_pRender->GetHeight()));
	}
	pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CGame");
	m_pNetwork		= m_pSystem->GetINetwork();
	m_bUpdateRet	= true;
	m_HardwareMouse = m_pSystem->GetIHardwareMouse();

	m_pVehicleSystem = new CVehicleSystem();
	m_pPlayerSystem = new CPlayerSystem();

	m_CrossHair = m_pRender->LoadTexture("crosshair.png", 0, false);
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
	initCommands();
	InitScripts();

	SmartScriptObject Gui(m_pScriptSystem,true);
	if (!m_pScriptSystem->GetGlobalValue("Gui",*Gui))
	{
		CryError("Cannot find Gui table in scripts (wrong working folder?)");
		return false;
	}
	else
	{
		m_pScriptSystem->BeginCall(Gui, "Init");
		m_pScriptSystem->PushFuncParam(Gui);
		m_pScriptSystem->EndCall();

	}

	
	// init key-bindings
	if (!m_bDedicatedServer)
		InitInputMap();

	LoadConfiguration("","game.cfg");

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
		m_pSystem->GetIConsole()->ShowConsole(true);

	InitConsoleCommands();

	DevModeInit();

	initPlayer();
	m_pInput->ShowCursor(false);
	m_pInput->GrabInput(true);

	if (m_pRender)
	{
		m_Font = gEnv->pRenderer->GetIFont();
		m_Font->Init("arial.ttf", 16, 18);
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

	m_Console->ExecuteFile("res/scripts/postinit.cfg");
	if (m_Console->GetCVar("nsightDebug"))
	{
		m_Console->ExecuteString("r_displayinfo 0");
	}

	m_QuadTree = std::make_shared<QuadTree>(8, 10, 0, 0, color3(1, 0, 0));
	//m_QuadTreeRender = std::make_shared<CRender>(m_pRender);
	//TreeRender treeRender(m_QuadTreeRender.get());

	m_testObjects.emplace_back(TestObject(AABB({-6, 0, 0}, {-1, 5, 5}), Vec4(0, 0, 10, 10)));
	m_testObjects.emplace_back(TestObject(AABB({0, 0, 0}, {5, 5, 5}), Vec4(10, 0, 0, 10)));
	m_testObjects.emplace_back(TestObject(AABB({6, 0, 0}, {11, 5, 5}), Vec4(0, 0, 10, 10)));
	m_testObjects.emplace_back(TestObject(AABB({-40, -0.5, 40}, {40, 0.5, -40}), Vec4(10,0,10,10)));


	srand(static_cast<unsigned int>(time(0)));

	Vec3 left(-40, -40, -40);
	Vec3 right(40, 40, 40);
	auto create_obj = [&]()->auto {
		const auto rand_pos = RandomVector(left, right);
		return TestObject(
			rand_pos, {5, 5, 5}, Vec4(RandomVector(Vec3(-5), Vec3(10)), 1.f));
	};
	for (int i = 0; i < 50; i++)
	{
		m_testObjects.emplace_back(
			create_obj()
		);
	}

	auto CameraBox = TestObject(AABB({16, 0, 0}, {21, 5, 5}), Vec4(4, 10, 40, 255));
	CameraBox.m_AABB.Translate(m_CameraController.RenderCamera()->transform.position);
	m_testObjects.emplace_back(CameraBox);
	m_IntersectionState.picked = m_testObjects.begin();

	m_pSystem->SetViewCamera(*m_CameraController.RenderCamera());

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CGame::InitClassRegistry()
{
	m_EntityClassRegistry.Init( m_pSystem );
	CPlayerSystem *pPlayerSystem = GetPlayerSystem();
	CVehicleSystem *pVehicleSystem = GetVehicleSystem();
	//CWeaponSystemEx *pWeaponSystemEx = GetWeaponSystemEx();	// m10

	assert( pPlayerSystem );
	assert( pVehicleSystem );
	//assert( pWeaponSystemEx );

	// Enumerate entity classes.
	EntityClass *entCls = NULL;
	m_EntityClassRegistry.MoveFirst();
	do {
		entCls = m_EntityClassRegistry.Next();
		if (entCls)
		{
			const char* entity_type = entCls->strGameType.c_str();
			EntityClassId ClassId = entCls->ClassId;
			if(strcmp("Player",entity_type)==0)
				pPlayerSystem->AddPlayerClass(ClassId);

			if(strcmp("Vehicle",entity_type)==0)
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


static ITexture* splash = nullptr;
bool CGame::Update()
{
	static const auto& render_game = true;
	const bool bRenderFrame			   = !m_bDedicatedServer;
	//*m_CameraController.CurrentCamera() = m_pSystem->GetViewCamera();
	m_pSystem->Update(0, IsInPause());
	{
		// TODO: FIX IT
		m_deltaTime = m_pSystem->GetDeltaTime();
		m_time += m_deltaTime;
		fps = 1.0f / m_deltaTime;
		ExecScripts();
		m_CameraController.update(m_deltaTime);

		
		SmartScriptObject Gui(m_pScriptSystem,true);
		if (!m_pScriptSystem->GetGlobalValue("Gui",*Gui))
		{
			CryError("Cannot find Gui table in scripts (wrong working folder?)");
			return false;
		}


		if (bRenderFrame)
		{
			SetRenderState();
			m_pSystem->RenderBegin();
			{
				//m_pRender->SetViewport(0, 0, m_pRender->GetWidth() / 2, m_pRender->GetHeight() / 2);
				m_CameraController.SetRenderCamera(0);
				auto cam = m_CameraController.RenderCamera();
				//cam->SetAngles(Vec3(0,0,0));
				//cam->SetPos(Vec3(0, 40, 0));
				cam->updateCameraVectors();
				

				cam->type = CCamera::Type::Perspective;
				Render();
				#if 0
				m_pRender->Flush();
				m_CameraController.SetRenderCamera(1);
				m_CameraController.RenderCamera()->type = CCamera::Type::Ortho;
				m_pRender->SetViewport(0, m_pRender->GetHeight() / 2, m_pRender->GetWidth() / 2, m_pRender->GetHeight() / 2);
				Render();
				#endif
				m_CameraController.SetRenderCamera(render_camera);

			}

			//PROFILER_PUSH_CPU_MARKER("DrawHud", Utils::COLOR_CYAN);
			m_pScriptSystem->BeginCall(Gui, "OnDraw");
			m_pScriptSystem->PushFuncParam(Gui);
			m_pScriptSystem->EndCall();

			DrawHud(fps);
			//PROFILER_POP_CPU_MARKER();
			if (!m_isActive)
			{
				Sleep(40);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// Special update function for developers mode.
	//////////////////////////////////////////////////////////////////////////
	if (IsDevModeEnable() && false)
		DevModeUpdate();
	//////////////////////////////////////////////////////////////////////////
	while (!m_qMessages.empty())
	{
		string smsg = m_qMessages.front();
		m_qMessages.pop();
		ProcessPMessages(smsg.c_str());
	}
	//////////////////////////////////////////////////////////////////////////

	if (bRenderFrame)
		m_pSystem->RenderEnd();

	return m_bUpdateRet;
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
	auto line   = m_pRender->GetHeight();
	auto step   = 18;

	const std::string mode = m_Mode == MENU ? "MENU"
		                         : m_Mode == FPS ? "FPS"
		                         : m_Mode == FLY ? "FLY"
		                         : "EDIT";

	// Info
	TextRenderInfo info(m_Font, Vec4(0.5, 1.0f, 0.6f, 1.0));
	SDrawTextInfo dti = info.getDTI();
	SDrawTextInfo MenuDTI;
	MenuDTI.color[0] = 0;
	MenuDTI.color[0] = 1;
	MenuDTI.color[0] = 0;
	MenuDTI.color[0] = 1;
	MenuDTI.font	 = m_Font;

	//
	auto render = m_pSystem->GetIRenderer();

	//===========

	m_Font->SetXPos(0);
	m_Font->SetYPos(18);
	auto& text  = info.text;
	auto& color = info.color;
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

	for (auto& text : info.text)
	{
		render->PrintLine(text.c_str(), dti);
	}

	//render->PrintLine("To hide depth buffer press <;>\n", dti);
	render->PrintLine((std::string("Camera position = ") + vec_to_string(m_CameraController.RenderCamera()->transform.position) + "\n").c_str(), dti);

	info.color = Vec4(1.0f, 0.f, 0.f, 1.0f);
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
			auto& lpp	 = m_IntersectionState.m_LastPickedPos;
			auto pos = std::to_string(lpp.x) + ",";
			pos += std::to_string(lpp.y) + ",";
			pos += std::to_string(lpp.z) + ";\n";
			render->PrintLine((std::string("Last picking pos: ") + pos).data(), info.getDTI());
			render->PrintLine((std::string("Current distant: ") + std::to_string(m_IntersectionState.m_CurrentDistant)).data(), info.getDTI());
		}
	}
}

bool CGame::Run(bool& bRelaunch)
{
	splash = gEnv->pRenderer->LoadTexture("fcsplash.bmp", 0, 0);
	m_pLog->Log("[OK] Game started");
	m_pSystem->Log("[OK] Game started");
	StartupServer(true, "test_server");

	m_bRelaunch = false;
	while (1)
	{
		if (!Update())
			break;
	}

	bRelaunch = m_bRelaunch;

	return true;
}

bool CGame::loadScene(std::string name)
{
	GetISystem()->Log("Scene loading");
	std::string& path = name;
	if (gEnv->p3DEngine->LoadLevel(path.data(), ""))
	{
		if (!gEnv->IsDedicated())
		{
			//scene->setCamera("main", new CCamera());
			//m_CameraController.m_Camera = &gEnv->pRenderer->GetCamera();
			m_CameraController.AddCamera(new CCamera(Vec3(0,0,0)));
			m_CameraController.AddCamera(new CCamera(Vec3(10,10,10)));
			m_CameraController.SetRenderCamera(1);
			m_CameraController.InitCVars();
			CPlayer* player = nullptr; // static_cast<CPlayer*>(scene->getObject("MyPlayer"));
			if (player != nullptr)
			{
				//player->attachCamera(scene->getCurrentCamera());
				//player->setGame(this);
				this->setPlayer(player);
			}
		}
		return true;
	}

	return false;
}

void CGame::saveScene(std::string name, std::string as)
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
}

void CGame::setPlayer(CPlayer* player)
{
	m_player = player;
}

void CGame::setCamera(CCamera* camera)
{
	//m_active_camera = camera;
	//m_World->setCamera(camera);
}

void CGame::Render()
{
	m_pSystem->SetViewCamera(*m_CameraController.RenderCamera());
	m_pSystem->Render();
	DrawAux();
}

IGAME_API IGame* CreateIGame()
{
	CGame* game = new CGame();
	return (game);
}

bool CGame::OnInputEvent(const SInputEvent& event)
{
	{
		bool retflag;
		const bool retval = ShouldHandleEvent(event, retflag);
		if (retflag)
			return retval;
	}
	const bool result = false;
	if (!IsInPause())
		OnInputEventProxy(event);
	PersistentHandler(event);
	return result;
}

void CGame::PersistentHandler(const SInputEvent& event)
{
	auto useBoxFilter = m_Console->GetCVar("bf");
	////////////////////////
	const bool keyPressed = (event.deviceType == eIDT_Keyboard  || event.deviceType == eIDT_Mouse) && event.state == eIS_Pressed;
	const bool control	= event.modifiers & eMM_Ctrl;
	const bool shift		= event.modifiers & eMM_Shift;
	bool alt		= event.modifiers & eMM_Alt;
	////////////////////////
	if (keyPressed)
	{
		if (event.keyId == eKI_Semicolon)
			openShadowMap = !openShadowMap;
		if (event.keyId >= eKI_F1 && event.keyId <= eKI_F12)
		{
			if (control)
			{
				DevMode_SavePlayerPos(event.keyId - eKI_F1, "BloomTest");
			}
			else if (shift)
			{
				DevMode_LoadPlayerPos(event.keyId - eKI_F1, "BloomTest");
			}
		}
		switch (event.keyId)
		{
    case eKI_Mouse1: 
		{
			if (event.state == eIS_Pressed)
			{
				auto& lpp = m_IntersectionState.m_LastPickedPos;
				m_IntersectionState.m_NeedIntersect = true;
				gEnv->pHardwareMouse->GetHardwareMousePosition(&m_IntersectionState.mx, &m_IntersectionState.my);
				if (m_Mode != MENU)
				{
					m_IntersectionState.mx= (float)m_pRender->GetWidth() / 2;
					m_IntersectionState.my= (float)m_pRender->GetHeight() / 2;
				}
			}
      break;
		}
		case eKI_1:
		case eKI_2:
		{
			const auto id = event.keyId - eKI_1;
			if (control)
			{
				movement_camera = id;
				m_CameraController.SetMovementCamera(id);
				m_CameraController.SetRenderCamera(id);
			}
			else
			{
				render_camera = id;
				m_CameraController.SetMovementCamera(id);
				m_CameraController.SetRenderCamera(id);
			}
			break;
		}
		case eKI_Insert:
		{
			m_InsertLines = !m_InsertLines;
			break;
		}
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

void CGame::gotoGame()
{
	//if (m_player != nullptr)
	{
		//m_World->GetActiveScene()->getCurrentCamera()->mode = CCamera::Mode::FPS;

		m_bInPause = false;
		m_Mode = FPS;
		m_pInput->ShowCursor(false);
		m_pInput->GrabInput(true);
		m_Console->ShowConsole(false);
		m_pSystem->EnableGui(false);
	}
}

void CGame::gotoFly()
{
	m_Mode = FLY;
}

void CGame::gotoEdit()
{
	m_Mode = EDIT;
}

void CGame::showMenu()
{
}

bool CGame::initPlayer()
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
	const bool control	= event.modifiers & eMM_Ctrl;
	const bool shift		= event.modifiers & eMM_Shift;
	const bool alt		= event.modifiers & eMM_Alt;
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
			camera.mode						  = CCamera::Mode::FLY;
			m_Mode							  = Mode::FLY;
			m_CameraController.CurrentCamera()->mode = CCamera::Mode::FLY;
			return true;
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
		case eKI_Escape:
			gotoMenu();
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
				gotoFullscreen();
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
	bool control	= event.modifiers & eMM_Ctrl;
	bool shift		= event.modifiers & eMM_Shift;
	bool alt		= event.modifiers & eMM_Alt;
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
			gotoGame();
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
	const bool keyPressed = event.deviceType == eIDT_Keyboard && event.state == eIS_Pressed;
	bool control	= event.modifiers & eMM_Ctrl;
	bool shift		= event.modifiers & eMM_Shift;
	bool alt		= event.modifiers & eMM_Alt;
	////////////////////////
	if (keyPressed)
	{
		switch (event.keyId)
		{
		case eKI_Escape:
			//m_inputHandler->mouseLock(false);
			//m_Mode = Mode::MENU;
			//Stop();
			return true;
		case eKI_Enter:
			gotoGame();
			return true;
		case eKI_J:
			return true;
		case eKI_K:
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
	bool control	= event.modifiers & eMM_Ctrl;
	bool shift		= event.modifiers & eMM_Shift;
	bool alt		= event.modifiers & eMM_Alt;
	retflag			= true;

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
	else
	if (stricmp(szMsg, "Relaunch") == 0) // relaunch message
	{
		m_bRelaunch  = true;
		m_bUpdateRet = false;
		return;
	}
	else
	if (strnicmp(szMsg,"SaveGame", 8)==0)		// save current game
	{
		if(!m_bEditor)
		{
			const char *sname="quicksave";
			if(strlen(szMsg)>8) { 
				sname=szMsg+9;
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

void CGame::gotoMenu()
{
	m_Mode	 = MENU;
	m_bInPause = true;
	m_pInput->ShowCursor(true);
	m_pInput->GrabInput(false);
	m_pSystem->EnableGui(true);
}

void CGame::gotoFullscreen()
{
	isFullScreen = !isFullScreen;
}

bool CGame::InitScripts()
{
	m_pScriptObjectGame = new CScriptObjectGame();
	m_pScriptObjectGame->InitializeTemplate(m_pScriptSystem);

	m_pScriptObjectInput=new CScriptObjectInput;
	CScriptObjectInput::InitializeTemplate(m_pScriptSystem);

	auto SOT = new CScriptObjectTest();
	SOT->InitializeTemplate(m_pScriptSystem);
	SOT->Init(m_pScriptSystem, this);

#if 0
  m_pScriptClient = new CScriptObjectClient();
  m_pScriptClient->InitializeTemplate(m_pScriptSystem);

  m_pScriptServer = new CScriptObjectServer();
  m_pScriptServer->InitializeTemplate(m_pScriptSystem);
#endif

	m_pScriptObjectGame->Init(m_pSystem->GetIScriptSystem(), this);
	m_pScriptObjectInput->Init(m_pScriptSystem,this,m_pSystem);
#if 0
  m_pScriptServer->Init(m_pSystem->GetIScriptSystem(), m_pServer);
  m_pScriptClient->Init(m_pSystem->GetIScriptSystem(), m_pClient);
#endif

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

	m_Console->AddCommand("toogle_viewport_drag", new toogle_viewport_drag(this));
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
	int age;
	m_playerObject->GetValue("name", name);
	m_playerObject->GetValue("age", age);
	m_Console->PrintLine("Player name: %s", name);
	m_Console->PrintLine("Player age: %d", age);

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

void CGame::DrawAux()
{
	//m_RenderAuxGeom->DrawLine({-0, -0.0, 0}, col, {0.25, 0.1, 0.5}, col);
	auto draw_quad = [](Vec3 p1, Vec3 p2, Vec3 p3, Vec3 p4, UCol col) {
		auto render = gEnv->pRenderer->GetIRenderAuxGeom();
		render->DrawTriangle(p1, col, p2, col, p3, col);
		render->DrawTriangle(p3, col, p4, col, p1, col);
	};
	const UCol col(255, 255, 255, 255);
	auto render   = gEnv->pRenderer->GetIRenderAuxGeom();
	render->DrawLine(
		{-10, 10, -5}, col, {10, 10, -5}, col);
	float x = 40, y = 0, z = -40;
	{
		const UCol col1(50, 125, 0, 100);
		draw_quad({-1, -1, z}, {-1, 1, z}, {1, 1, z}, {1, -1, z}, col1);
	}

	UCol selected_color(0,1,0,1);
	IntersectionTest();
	int _idx = 0;
	for (auto& object : m_testObjects)
	{
		if (m_IntersectionState.picked != m_testObjects.begin() + _idx)
			render->DrawAABB(object.m_AABB.min, object.m_AABB.max, object.intersected ? selected_color : object.m_Color);
		else
		{
			render->DrawAABB(
				object.m_AABB.min, object.m_AABB.max, selected_color
			);
		}
		object.intersected = false;
		_idx++;
	}
	if (lineBuffer.size() >= 2)
	{
		#if 0
		for (int i = 0; i < (lineBuffer.size() - 1); i ++)
		{
			render->DrawLine(lineBuffer[i] + Vec3(0, 0.1, 0), UCol(255,255,255,255), lineBuffer[i + 1] + Vec3(0, 0.1, 0), UCol(255,255,255,255));	
		}
		#else
			render->DrawLines(lineBuffer.data(), lineBuffer.size(), UCol(255,255,255,255));	
		#endif
	}

	Ray ray;

	ray.origin = m_CameraController.RenderCamera()->transform.position;
	ray.direction = m_CameraController.RenderCamera()->Front;

	render->DrawLine(
		ray.origin + ray.direction, col, ray.origin + ray.direction * 40.f, col);

	DrawAxis(render, Vec3(40));
	//m_pRender->DrawFullScreenImage(m_CrossHair->getId());
	m_pRender->DrawImage(static_cast<float>(m_pRender->GetWidth()) / 2, static_cast<float>(m_pRender->GetHeight()) / 2, 20,20, m_CrossHair->getId(), 0, 0, 1, 1, 0, 1, 0, 0.5);
}

void CGame::DrawAxis(IRenderAuxGeom* render, Vec3 axis)
{
	// Axis
	///////////////////////////////////////
	RSS(gEnv->pRenderer, DEPTH_TEST, false);
	{
		auto& a = axis;
		{
			const UCol c = Vec3(1, 0, 0);
			render->DrawLine(
				{-a.x, 0, 0}, c, {a.x, 0, 0}, c);
		}
		{
			const UCol c = Vec3(0, 1, 0);
			render->DrawLine(
				{0, -a.y, 0}, c, {0, a.y, 0}, c);
		}
		{
			const UCol c = Vec3(0, 0, 1);
			render->DrawLine(
				{0, 0, -a.z}, c, {0, 0, a.z}, c);
		}
	}
}

void CGame::IntersectionTest()
{
	if (m_IntersectionState.m_NeedIntersect)
	{
		IntersectionByRayCasting();
		m_IntersectionState.m_NeedIntersect = false;
	}
}

void CGame::Jump(float fValue, XActivationEvent ae)
{
}

void CGame::IntersectionByRayCasting()
{
	auto& start = m_IntersectionState.ray.start;
	gEnv->pRenderer->UnProjectFromScreen(
		m_IntersectionState.mx, m_IntersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = m_IntersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
		m_IntersectionState.mx, m_IntersectionState.my, 1, &end.x, &end.y, &end.z);

	float tMin = std::numeric_limits<float>::max();
	Ray eyeRay;

	m_CameraController.RenderCamera()->type = CCamera::Type::Ortho;
	eyeRay.origin = m_CameraController.RenderCamera()->GetPos();
	eyeRay.direction = glm::normalize(end-start);

	const auto lastPos = m_IntersectionState.m_LastPickedPos; 
	for (size_t i = 0; i < m_testObjects.size(); i++){
		const glm::vec2 tMinMax = m_testObjects[i].m_AABB.intersectBox(eyeRay);
		if (tMinMax.x < 0 || tMinMax.y < 0)
			continue;
		if(tMinMax.x<tMinMax.y && tMinMax.x<tMin) {
			m_IntersectionState.picked = m_testObjects.begin() + i;
			tMin = tMinMax.x;
			m_IntersectionState.m_LastPickedPos = eyeRay.origin + eyeRay.direction * tMin;
			m_IntersectionState.m_CurrentDistant = glm::distance(eyeRay.origin, m_IntersectionState.m_LastPickedPos);
		}
	}
	if (lastPos != m_IntersectionState.m_LastPickedPos)
	{
		lineBuffer.push_back(m_IntersectionState.m_LastPickedPos);
	}
}

void CGame::TriggerMoveLeft(float fValue, XActivationEvent ae)
{
	m_CameraController.ProcessKeyboard(Movement::LEFT, m_deltaTime);
}

void CGame::TriggerMoveRight(float fValue, XActivationEvent ae)
{
	m_CameraController.ProcessKeyboard(Movement::RIGHT, m_deltaTime);
}

void CGame::TriggerMoveForward(float fValue, XActivationEvent ae)
{
	m_CameraController.ProcessKeyboard(Movement::FORWARD, m_deltaTime);
}

void CGame::TriggerMoveBackward(float fValue, XActivationEvent ae)
{
	m_CameraController.ProcessKeyboard(Movement::BACKWARD, m_deltaTime);
}

void CGame::TriggerUse(float fValue, XActivationEvent ae)
{
}

void CGame::TriggerTurnLR(float fValue, XActivationEvent ae)
{
	//float rotation_speed = m_deltaTime * (fValue > 0 ? 1.f : -1.f);
		fValue *=m_deltaTime;	
	m_CameraController.ProcessMouseMovement(fValue, 0);
}

void CGame::TriggerTurnUD(float fValue, XActivationEvent ae)
{
	//float rotation_speed = m_deltaTime * (fValue > 0 ? -1.f : 1.f);
		fValue *=m_deltaTime;	
	m_CameraController.ProcessMouseMovement(0, -fValue);
}

void CGame::TriggerQuickLoad(float fValue, XActivationEvent ae)
{
	if (this->IsQuicksaveAllowed())
		this->SendMessage("LoadGame");
}

void CGame::TriggerQuickSave(float fValue, XActivationEvent ae)
{
	ICVar *g_LevelStated = GetISystem()->GetIConsole()->GetCVar("g_LevelStated");
	if (!g_LevelStated->GetIVal())
	{
		if (this->IsQuicksaveAllowed())
			this->SendMessage("SaveGame");
	}
}

void CGame::TriggerMessageMode(float fValue, XActivationEvent ae)
{
	//m_pSystem->GetIConsole()->ExecuteString("@messagemode");
}

void CGame::TriggerMessageMode2(float fValue, XActivationEvent ae)
{
	//m_pSystem->GetIConsole()->ExecuteString("@messagemode2");
}

void CGame::TriggerScreenshot(float fValue, XActivationEvent ae)
{
	this->m_pSystem->GetIConsole()->ExecuteString("screenshot");
}

void CGame::TriggerFullscreen(float fValue, XActivationEvent ae)
{
	auto fs = gEnv->pConsole->GetCVar("r_Fullscreen");
	auto val = !fs->GetIVal();

	gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_TOGGLE_FULLSCREEN, val, 0);
}

void CGame::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:
		m_isActive = bool(wparam);
		break;
	default:
		break;
	}
}
