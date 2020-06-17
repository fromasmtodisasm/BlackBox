#pragma once
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/System/IConsole.hpp>
//#include <BlackBox/Game/ScriptObjects/ScriptObjectClient.hpp>
//#include "XClientSnapshot.h"
#include <Network/XEntityProcessingCmd.hpp>
#include <Network/XNetwork.hpp>	
//#include "XNetworkStats.h"						// CXNetworkStats
//#include <BlackBox/ScriptObjectVector.hpp>						// CXNetworkStats
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>						// CXNetworkStats
#include <BlackBox/System/ITimer.hpp>

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


class CClient : 
	public IClientSink,
	public IActionMapSink
{
public:
	CClient(CGame *pGame);
	void Update();
	bool Init();

  // Inherited via IClientSink
  virtual void OnXConnect() override;
	virtual void OnXClientDisconnect(const char* szCause) override;
  virtual void OnXContextSetup(CStream& stmContext) override;
	virtual void OnXData(CStream& stm) override;
  virtual void OnXServerTimeout() override;
	virtual void OnXServerRessurect() override;
  virtual unsigned int GetTimeoutCompensation() override;
	virtual void MarkForDestruct() override;
  virtual bool DestructIfMarked() override;

	// Triggers function
	void TriggerMoveLeft(float fValue,XActivationEvent ae);
	void TriggerMoveRight(float fValue,XActivationEvent ae);
	void TriggerMoveForward(float fValue,XActivationEvent ae);
	void TriggerMoveBackward(float fValue,XActivationEvent ae);
	void TriggerJump(float fValue,XActivationEvent ae);

	void TriggerFire0(float fValue,XActivationEvent ae);

	void TriggerUse(float fValue,XActivationEvent ae);
	void TriggerTurnLR(float fValue,XActivationEvent ae);
	void TriggerTurnUD(float fValue,XActivationEvent ae);

	void TriggerQuickLoad(float fValue,XActivationEvent ae);
	void TriggerQuickSave(float fValue,XActivationEvent ae);
	void TriggerMessageMode(float fValue,XActivationEvent ae);
	void TriggerMessageMode2(float fValue,XActivationEvent ae);
	void TriggerScreenshot(float fValue, XActivationEvent ae);
	void TriggerFullscreen(float fValue, XActivationEvent ae);

	void TriggerChangeCameraMode(float fValue, XActivationEvent ae);

	void OnLoadScene();

	BEGIN_INPUTACTIONMAP()
		REGISTER_INPUTACTIONMAP(ACTION_MOVE_LEFT, TriggerMoveLeft)
		REGISTER_INPUTACTIONMAP(ACTION_MOVE_RIGHT, TriggerMoveRight)
		REGISTER_INPUTACTIONMAP(ACTION_MOVE_FORWARD, TriggerMoveForward)
		REGISTER_INPUTACTIONMAP(ACTION_MOVE_BACKWARD, TriggerMoveBackward)
		REGISTER_INPUTACTIONMAP(ACTION_JUMP, TriggerJump)
		REGISTER_INPUTACTIONMAP(ACTION_FIRE0, TriggerFire0)

		REGISTER_INPUTACTIONMAP(ACTION_CAMERA_MODE, TriggerChangeCameraMode)


		REGISTER_INPUTACTIONMAP(ACTION_USE, TriggerUse)
		REGISTER_INPUTACTIONMAP(ACTION_TURNLR, TriggerTurnLR)
		REGISTER_INPUTACTIONMAP(ACTION_TURNUD, TriggerTurnUD)

		REGISTER_INPUTACTIONMAP(ACTION_QUICKLOAD,TriggerQuickLoad);
		REGISTER_INPUTACTIONMAP(ACTION_QUICKSAVE,TriggerQuickSave);


		REGISTER_INPUTACTIONMAP(ACTION_MESSAGEMODE, TriggerMessageMode)
		REGISTER_INPUTACTIONMAP(ACTION_MESSAGEMODE2, TriggerMessageMode2)
		REGISTER_INPUTACTIONMAP(ACTION_TAKESCREENSHOT, TriggerScreenshot)
		//REGISTER_INPUTACTIONMAP(ACTION_FULLSCRN_TOOGLE, TriggerFullscreen);
		END_INPUTACTIONMAP()

	void Render();
private:
	void DrawAux();
	void DrawAxis(IRenderAuxGeom* render, Vec3 axis);
	void CClient::IntersectionTest();
	void CClient::IntersectionByRayCasting();

private:
	CGame* m_pGame{};

	// Player
	CXEntityProcessingCmd m_PlayerProcessingCmd;
	CXEntityProcessingCmd m_PrevPlayerProcessingCmd;
	CCameraController m_CameraController;
	IActionMapManager* m_pIActionMapManager{};

	IClient* m_pClient;

	std::vector<TestObject> m_testObjects;
	size_t m_SelectedBox = 0;
	struct SIntersectionState
	{
		SIntersectionState() = default;
		bool m_NeedIntersect = false;
		Vec3 m_LastPickedPos = Vec3(0);
		struct Ray
		{
			Ray() = default;
			Vec3 start;		
			Vec3 end;		
			Vec3 origin;
			Vec3 direction;
		}ray;
		std::vector<TestObject>::iterator picked;
		float m_CurrentDistant = 0.f;
		float mx, my;
	}m_IntersectionState;

	std::vector<Vec3> m_LineList;
	bool m_InsertLines = false;

	ITexture* m_CrossHair = nullptr;

};
