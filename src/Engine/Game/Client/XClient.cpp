#include <BlackBox/Game/Client/XClient.hpp>

//////////////////////////////////////////////////////////////////////
CXClient::CXClient()
{
	m_bConnected = 0;
	m_CameraParams = 0;
	m_pScriptObjectClient = 0;
	m_pIActionMapManager = 0;
	//m_pEntitySystem = 0;
	m_pISystem = 0;
	m_pIClient = 0;
	m_pISystem = 0;
	//m_wPlayerID = INVALID_WID;
	m_pGame = 0;

	cl_explShakeDCoef = 0.07f;
	cl_explShakeAmplH = 0.001f;
	cl_explShakeAmplV = 0.001f;
	cl_explShakeFreq = 11.73f;
	cl_explShakeTime = 1.73f;

	m_fFrontSound = 0;
	m_fBackSound = 0;
	m_fLeftSound = 0;
	m_fRightSound = 0;
	m_pClientStuff = 0;
	bDoSwitch = false;
	m_pTimer = 0;
	m_bSelfDestruct = false;			//  to make sure the client is only released in one place
	m_pSavedConsoleVars = 0;
	m_bLazyChannelState = false;	// start with false on client and serverslot side
}
