#include <Client\Client.hpp>

#include <BlackBox/Renderer/IRenderAuxGeom.hpp>

#define YAW		(0)  
#define PITCH	(1)    
#define ROLL	(0)   

std::vector<Vec3> lineBuffer;


CClient::CClient(CGame *pGame)
	: 
	m_pGame(pGame),
	m_CameraController(),
	m_IntersectionState()
{
}

void CClient::Update()
{
	m_PlayerProcessingCmd.SetDeltaAngles(Vec3(0));
	if (!gEnv->pConsole->IsOpened())
		m_pGame->GetActionMapManager()->Update(16);
	if (m_PlayerProcessingCmd.CheckAction(ACTION_MOVE_LEFT))
	{
		m_CameraController.ProcessKeyboard(Movement::LEFT, m_pGame->m_deltaTime);	
	}
	if (m_PlayerProcessingCmd.CheckAction(ACTION_MOVE_RIGHT))
	{
		m_CameraController.ProcessKeyboard(Movement::RIGHT, m_pGame->m_deltaTime);	
	}
	if (m_PlayerProcessingCmd.CheckAction(ACTION_MOVE_FORWARD))
	{
		m_CameraController.ProcessKeyboard(Movement::FORWARD, m_pGame->m_deltaTime);	
	}
	if (m_PlayerProcessingCmd.CheckAction(ACTION_MOVE_BACKWARD))
	{
		m_CameraController.ProcessKeyboard(Movement::BACKWARD, m_pGame->m_deltaTime);	
	}
	if (m_PlayerProcessingCmd.CheckAction(ACTION_TURNLR))
	{
		auto ang = m_PlayerProcessingCmd.GetDeltaAngles()[YAW];
		//ang *= 0.01;
		m_CameraController.ProcessMouseMovement(-ang, 0);
		//m_CameraController.ProcessKeyboard(Movement::BACKWARD, m_pGame->m_deltaTime);	
	}
	if (m_PlayerProcessingCmd.CheckAction(ACTION_TURNUD))
	{
		auto ang = m_PlayerProcessingCmd.GetDeltaAngles()[PITCH];
		//ang *= 0.01;
		m_CameraController.ProcessMouseMovement(0, -ang);
		//m_CameraController.ProcessKeyboard(Movement::BACKWARD, m_pGame->m_deltaTime);	
	}
	if (m_PlayerProcessingCmd.CheckAction(ACTION_FIRE0))
	{
		auto& lpp = m_IntersectionState.m_LastPickedPos;
		m_IntersectionState.m_NeedIntersect = true;
		gEnv->pHardwareMouse->GetHardwareMousePosition(&m_IntersectionState.mx, &m_IntersectionState.my);
		//if (m_Mode != MENU)
		{
			m_IntersectionState.mx= (float)gEnv->pRenderer->GetWidth() / 2;
			m_IntersectionState.my= (float)gEnv->pRenderer->GetHeight() / 2;
		}
	}

	auto pos = m_CameraController.CurrentCamera()->GetPos();
	auto cam = m_CameraController.CurrentCamera();
	gEnv->pRenderer->GetIRenderAuxGeom()->DrawAABB(pos + cam->Front*Vec3(2, -0.5, -1), pos + cam->Front*Vec3(3, 0, 1), UCol(0,0,1,1));

	m_CameraController.SetRenderCamera(0);
	m_pGame->m_pSystem->SetViewCamera(*m_CameraController.RenderCamera());
	m_PlayerProcessingCmd.Reset();
}

bool CClient::Init()
{
	m_pIActionMapManager = m_pGame->GetActionMapManager();
	if(m_pIActionMapManager)
		m_pIActionMapManager->SetSink(this);
	m_pGame->m_pSystem->SetViewCamera(*m_CameraController.RenderCamera());

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

	//auto CameraBox = TestObject(AABB({16, 0, 0}, {21, 5, 5}), Vec4(4, 10, 40, 255));
	//CameraBox.m_AABB.Translate(m_CameraController.RenderCamera()->transform.position);
	//m_testObjects.emplace_back(CameraBox);
	m_IntersectionState.picked = m_testObjects.begin();

	m_CrossHair = gEnv->pRenderer->LoadTexture("crosshair.png", 0, false);
	return true;
}

void CClient::OnXConnect()
{
}

void CClient::OnXClientDisconnect(const char* szCause)
{
}


void CClient::OnXContextSetup(CStream& stmContext)
{
}

void CClient::OnXData(CStream& stm)
{
}

void CClient::OnXServerTimeout()
{
}

void CClient::OnXServerRessurect()
{
}

unsigned int CClient::GetTimeoutCompensation()
{
	return 0;
}

void CClient::MarkForDestruct()
{
}

bool CClient::DestructIfMarked()
{
	return false;
}


//////////////////////////////////////////////////////////////////////
void CClient::TriggerMoveLeft(float fValue,XActivationEvent ae)
{
	m_PlayerProcessingCmd.SetMoveLeft(1);
	m_PlayerProcessingCmd.AddAction(ACTION_MOVE_LEFT);
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerMoveRight(float fValue,XActivationEvent ae)
{
	m_PlayerProcessingCmd.SetMoveRight(1);
	m_PlayerProcessingCmd.AddAction(ACTION_MOVE_RIGHT);
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerMoveForward(float fValue,XActivationEvent ae)
{
	m_PlayerProcessingCmd.SetMoveFwd(1);
	m_PlayerProcessingCmd.AddAction(ACTION_MOVE_FORWARD);
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerMoveBackward(float fValue,XActivationEvent ae)
{
	m_PlayerProcessingCmd.SetMoveBack(1);
	m_PlayerProcessingCmd.AddAction(ACTION_MOVE_BACKWARD);
}

///////////////////////////////////////////////
void CClient::TriggerTurnLR(float fValue,XActivationEvent ae)
{ 
	{
		float fFovMul = 1.0f;
		#if 0
		IEntity *pPlayerEnt = m_pISystem->GetEntity( m_wPlayerID );
		if (pPlayerEnt)
		{

			IEntityCamera *pCam = pPlayerEnt->GetCamera();
			if (pCam)
			{
				fFovMul = (float) (pCam->GetFov() / 1.5707963267948966192313216916398);
			}
		}
		#endif
		m_PlayerProcessingCmd.GetDeltaAngles()[YAW] -= fValue*fFovMul;
		m_PlayerProcessingCmd.AddAction(ACTION_TURNLR);
	}
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerTurnUD(float fValue,XActivationEvent ae)
{
	{
		float fFovMul = 1.0f;
		#if 0
		IEntity *pPlayerEnt = m_pISystem->GetEntity( m_wPlayerID );
		if (pPlayerEnt)
		{

			IEntityCamera *pCam = pPlayerEnt->GetCamera();
			if (pCam)
			{
				fFovMul = float(pCam->GetFov() / 1.5707963267948966192313216916398);
			}
			//RESET RECOIL RETURN
			IEntityContainer *pCnt=pPlayerEnt->GetContainer();
		}
		#endif
		m_PlayerProcessingCmd.GetDeltaAngles()[PITCH] += fValue*fFovMul;
		m_PlayerProcessingCmd.AddAction(ACTION_TURNUD);
	}
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerJump(float fValue,XActivationEvent ae)
{
	m_PlayerProcessingCmd.AddAction(ACTION_JUMP);
}

void CClient::TriggerFire0(float fValue, XActivationEvent ae)
{
	m_PlayerProcessingCmd.AddAction(ACTION_FIRE0);
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerUse(float fValue,XActivationEvent ae)
{
	m_PlayerProcessingCmd.AddAction(ACTION_USE);
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerQuickLoad(float fValue,XActivationEvent ae)
{
	if (m_pGame->IsQuicksaveAllowed())
		m_pGame->SendMessage("LoadGame");
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerQuickSave(float fValue,XActivationEvent ae)
{
	ICVar *g_LevelStated = GetISystem()->GetIConsole()->GetCVar("g_LevelStated");
	if (!g_LevelStated->GetIVal())
	{
		if (m_pGame->IsQuicksaveAllowed())
			m_pGame->SendMessage("SaveGame");
	}
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerMessageMode(float fValue,XActivationEvent ae)
{
	m_pGame->m_pSystem->GetIConsole()->ExecuteString("messagemode");
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerMessageMode2(float fValue,XActivationEvent ae)
{
	m_pGame->m_pSystem->GetIConsole()->ExecuteString("messagemode2");
}

//////////////////////////////////////////////////////////////////////
void CClient::TriggerScreenshot(float fValue, XActivationEvent ae)
{
	m_pGame->m_pSystem->GetIConsole()->ExecuteString("r_GetScreenShot 1");
}

void CClient::TriggerChangeCameraMode(float fValue, XActivationEvent ae)
{
	auto mode = m_CameraController.CurrentCamera()->mode;
	m_CameraController.CurrentCamera()->mode = mode == CCamera::Mode::FLY ? CCamera::Mode::FPS : CCamera::Mode::FLY;
}

void CClient::OnLoadScene()
{
	m_CameraController.AddCamera(new CCamera(Vec3(0,0,0)));
	m_CameraController.AddCamera(new CCamera(Vec3(10,10,10)));
	m_CameraController.SetRenderCamera(1);
	m_CameraController.InitCVars();
}

void CClient::DrawAux()
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
	gEnv->pRenderer->DrawImage(static_cast<float>(gEnv->pRenderer->GetWidth()) / 2, static_cast<float>(gEnv->pRenderer->GetHeight()) / 2, 20,20, m_CrossHair->getId(), 0, 0, 1, 1, 0, 1, 0, 0.5);
}

void CClient::DrawAxis(IRenderAuxGeom* render, Vec3 axis)
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

void CClient::IntersectionTest()
{
	if (m_IntersectionState.m_NeedIntersect)
	{
		IntersectionByRayCasting();
		m_IntersectionState.m_NeedIntersect = false;
	}
}

void CClient::IntersectionByRayCasting()
{
	auto& start = m_IntersectionState.ray.start;
	gEnv->pRenderer->UnProjectFromScreen(
		m_IntersectionState.mx, m_IntersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = m_IntersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
		m_IntersectionState.mx, m_IntersectionState.my, 1, &end.x, &end.y, &end.z);

	float tMin = std::numeric_limits<float>::max();
	Ray eyeRay;

	//m_CameraController.RenderCamera()->type = CCamera::Type::Ortho;
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

void CClient::Render()
{
	DrawAux();
}
