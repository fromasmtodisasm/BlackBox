 
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
// 
//	File: ScriptObjectInput.cpp
//  Description: 
//		ScriptObjectInput.cpp: implementation of the CScriptObjectInput class.
//
//	History: 
//	- File Created by Marco Corbetta
//	- February 2005: Modified by Marco Corbetta for SDK release
//	- October 2006: Modified by Marco Corbetta for SDK 1.4 release
//
//////////////////////////////////////////////////////////////////////

#include <BlackBox/Core/Platform/Platform.hpp> 
#include "Game.hpp"
#include "ScriptObjects/ScriptObjectInput.hpp"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectInput)

//////////////////////////////////////////////////////////////////////
CScriptObjectInput::CScriptObjectInput()
{
}

//////////////////////////////////////////////////////////////////////
CScriptObjectInput::~CScriptObjectInput()
{
}

//////////////////////////////////////////////////////////////////////
void CScriptObjectInput::InitializeTemplate(IScriptSystem *pSS)
{
	_ScriptableEx<CScriptObjectInput>::InitializeTemplate(pSS);
	REG_FUNC(CScriptObjectInput,BindCommandToKey);
	REG_FUNC(CScriptObjectInput,BindAction);
	REG_FUNC(CScriptObjectInput,BindActionMultipleMaps);
	REG_FUNC(CScriptObjectInput,ClearAction);
	REG_FUNC(CScriptObjectInput,SetActionMap);
	REG_FUNC(CScriptObjectInput,GetActionMaps);
	REG_FUNC(CScriptObjectInput,ResetToDefaults);
	REG_FUNC(CScriptObjectInput,ResetAllBindings);
	REG_FUNC(CScriptObjectInput,ResetBinding);
	REG_FUNC(CScriptObjectInput,GetBinding);
	REG_FUNC(CScriptObjectInput,SetMouseSensitivity);
	REG_FUNC(CScriptObjectInput,GetMouseSensitivity);
	REG_FUNC(CScriptObjectInput,SetMouseSensitivityScale);
	REG_FUNC(CScriptObjectInput,GetMouseSensitivityScale);
	REG_FUNC(CScriptObjectInput,SetJoySensitivityHGain);
	REG_FUNC(CScriptObjectInput,SetJoySensitivityHScale);
	REG_FUNC(CScriptObjectInput,SetJoySensitivityVGain);
	REG_FUNC(CScriptObjectInput,SetJoySensitivityVScale);
	REG_FUNC(CScriptObjectInput,GetJoySensitivityHGain);
	REG_FUNC(CScriptObjectInput,GetJoySensitivityHScale);
	REG_FUNC(CScriptObjectInput,GetJoySensitivityVGain);
	REG_FUNC(CScriptObjectInput,GetJoySensitivityVScale);
	REG_FUNC(CScriptObjectInput,GetXKeyPressedName);
	REG_FUNC(CScriptObjectInput,GetXKeyDownName);
	REG_FUNC(CScriptObjectInput,ResetKeyState);
	REG_FUNC(CScriptObjectInput,SetInvertedMouse);
	REG_FUNC(CScriptObjectInput,GetInvertedMouse);
}

//////////////////////////////////////////////////////////////////////
/*! Initializes the script-object and makes it available for the scripts.
		@param pScriptSystem Pointer to the ScriptSystem-interface
		@param pGame Pointer to the Game
		@param pSystem Pointer to the System-interface
*/
void CScriptObjectInput::Init(IScriptSystem *pScriptSystem,CGame *pGame,ISystem *pSystem)
{
	m_pGame=pGame;
	m_pSystem=pSystem;
	m_pInput=pSystem->GetIInput();
	m_pConsole=pSystem->GetIConsole();
	InitGlobal(pScriptSystem,"Input",this);
	
}

//////////////////////////////////////////////////////////////////////
//! Reset all bindings, and set the default ones again
int CScriptObjectInput::ResetToDefaults(IFunctionHandler *pH)
{
	m_pGame->ResetInputMap();

	return pH->EndFunctionNull();
}

//////////////////////////////////////////////////////////////////////
//! Resets all bindings
int CScriptObjectInput::ResetAllBindings(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	m_pGame->GetActionMapManager()->ResetAllBindings();
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*! Binds a key (or other input) to a certain LUA-command
		@param sCmd LUA-command to execute (str)
		@param sRes key which should be bound to that command (str)
		@param nCex if this is 0 the command will executed each frame, only once otherwise (int)
*/
int CScriptObjectInput::BindCommandToKey(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(3);
	const char *sCmd;
	const char *sRes;
	int nCex;
	pH->GetParam(1,sCmd);
	pH->GetParam(2,sRes);
	pH->GetParam(3,nCex);
	
	if (!sRes)
		return pH->EndFunction();
	 
	char sTemp[256];strcpy(sTemp,sRes);
	m_pConsole->CreateKeyBind(sCmd,_strlwr(sTemp));
	
	return pH->EndFunction();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Binds a key (or other input) to a certain action (eg. JUMP)
		@param sAction action to bind (string)
		@param sKeys keys which should be bound to that action (this can be more than one, separated by |, eg LEFTCTRL|RIGHTCTRL) (str)
		@param nCheckPressed if this is 0 the action will triggered each frame, only once otherwise (int) (optional parameter)
*/
int CScriptObjectInput::BindAction(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	int nNumOfParams=0;
	if((nNumOfParams=pH->GetParamCount())<2){
		m_pScriptSystem->RaiseError("Input:BindAction wrong number of arguments");
		return pH->EndFunctionNull();
	}
	const char *sAction;
	const char *sKeys;
	const char *sActionMap=NULL;
	int	iKeyPos = -1;
	int nCheckPressed=0;
	pH->GetParam(1,sAction);
	pH->GetParam(2,sKeys);
	if(nNumOfParams>2)
		pH->GetParam(3,sActionMap);
	if (nNumOfParams>3)
		pH->GetParam(4, iKeyPos);
	//TRACE("BindAction %s %s %s\n",sAction,sKeys,(nCheckPressed)?"true":"false");
	m_pGame->BindAction(sAction,sKeys,sActionMap, iKeyPos);

	return pH->EndFunction();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Binds a key (or other input) to a certain action (eg. JUMP) in all action-maps associated with this action
		@param sAction action to bind (string)
		@param sKeys keys which should be bound to that action (this can be more than one, separated by |, eg LEFTCTRL|RIGHTCTRL) (str)
*/
int CScriptObjectInput::BindActionMultipleMaps(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(3);
	const char *sAction;
	const char *sKeys;
	int iKeyPos = -1;
	pH->GetParam(1,sAction);
	pH->GetParam(2,sKeys);
	pH->GetParam(3,iKeyPos);
	m_pGame->BindActionMultipleMaps(sAction, sKeys, iKeyPos);
	return pH->EndFunction();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! Clears a certain action (eg. JUMP) from the action map.
		@param sAction action to be removed (str)
		@return true if this action was found and deleted, false otherwise
*/
int CScriptObjectInput::ClearAction(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);
	const char *sAction;
	pH->GetParam(1,sAction);
	
	m_pGame->ClearAction(sAction);
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*! Set the current action map 
		@param sMapName action map name("default","zoom","vehicle" etc..)
*/
int CScriptObjectInput::SetActionMap(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);
	const char *sMapName=NULL;
	if(pH->GetParam(1,sMapName))
	{
		m_pGame->m_pIActionMapManager->SetActionMap(sMapName);
	}
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
class CDumpActions : public IActionMapDumpSink
{
public:
	IScriptObject *m_pObj;
	int m_nActions;
	CDumpActions(IScriptObject *pObj)
	{
		m_nActions=0;
		m_pObj=pObj;
	}
	void OnElementFound(const char *pszActionMapName, IActionMap *pActionMap)
	{
		m_pObj->SetAt(m_nActions, pszActionMapName);
		m_nActions++;
	}
};

//////////////////////////////////////////////////////////////////////
/*! Returns a table containg all action names
*/
int CScriptObjectInput::GetActionMaps(IFunctionHandler *pH)
{
	if(!m_pInput) return pH->EndFunctionNull();
	CHECK_PARAMETERS(0);
	SmartScriptObject pObj(m_pScriptSystem);
	CDumpActions Dumper(pObj);
	m_pGame->m_pIActionMapManager->GetActionMaps(&Dumper);
	return pH->EndFunction(Dumper.m_pObj);
}

//////////////////////////////////////////////////////////////////////
/*! Reset an action in a specified action map
	@param pszActionMapName action map
	@param nAction the action to reset
*/
int CScriptObjectInput::ResetBinding(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(2);
	const char *pszActionMapName;
	int nAction;
	pH->GetParam(1, pszActionMapName);
	pH->GetParam(2, nAction);
	IActionMap *pActionMap=m_pGame->m_pIActionMapManager->GetActionMap(pszActionMapName);
	if (pActionMap)
		pActionMap->ResetBinding(nAction);
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*! Get the binding of a specified action of a specified action map
	@param pszActionMapName action map
	@param nAction the action to reset
	@return a table containing 'n' couples key-mod 
		key: is the key bound to this action
		mod: is the modifier(this parameter is currently unused)
*/
int CScriptObjectInput::GetBinding(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(2);
	const char *pszActionMapName;
	int nAction;
	pH->GetParam(1, pszActionMapName);
	pH->GetParam(2, nAction);
	IActionMap *pActionMap=m_pGame->m_pIActionMapManager->GetActionMap(pszActionMapName);
	if (!pActionMap)
		return pH->EndFunctionNull();
	SmartScriptObject pObj(m_pScriptSystem);
	char pszKey[256];
	char pszMod[256];
	int nKey = 0;
	int nMod = 0;
	int n=1;
	for (int i=0;i<MAX_BINDS_PER_ACTION;i++)
	{
		SmartScriptObject pKey(m_pScriptSystem);
		pActionMap->GetBinding(nAction, i, pszKey, pszMod);
		pActionMap->GetBinding(nAction, i, nKey, nMod);
		if (strlen(pszKey))
		{
			pKey->SetValue("key", pszKey);
			if (strlen(pszMod))
				pKey->SetValue("mod", pszMod);
			else
				pKey->SetToNull("mod");
			pKey->SetValue("key_id", nKey);
			if (nMod)
				pKey->SetValue("mod_id", nMod);
			else
				pKey->SetToNull("mod_id");
			pObj->SetAt(n, pKey);
		}
		n++;
	}
	return pH->EndFunction(pObj);
}

//////////////////////////////////////////////////////////////////////
/*! Set the mouse sensitivity
	@param fSensitivity the mouse sensitivity to be set
*/
int CScriptObjectInput::SetMouseSensitivity(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);
	float fSensitivity;
	pH->GetParam(1,fSensitivity);
#if 0
  if (m_pInput->GetDevice(0, eIDT_Mouse))
    m_pInput->GetDevice(0, eIDT_Mouse)->SetSensitvity(fSensitivity);
#endif
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*! Get the mouse sensitivity
	@return the mouse sensitivity
*/
int CScriptObjectInput::GetMouseSensitivity(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunction(0);
	float fSensitivity;
#if 0
  if(m_pInput->GetIMouse())
	  fSensitivity=m_pInput->GetIMouse()->GetSensitvity();
  else
    fSensitivity = 20.0f;
#endif
	return pH->EndFunction(fSensitivity);
}

//////////////////////////////////////////////////////////////////////
/*! Set the mouse sensitivity scale
	@param fSensScale the mouse  sensitivity scale to be set
*/
int CScriptObjectInput::SetMouseSensitivityScale(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);
	float fSensScale;
	pH->GetParam(1,fSensScale);
#if 0
  if(m_pInput->GetIMouse())
	  m_pInput->GetIMouse()->SetSensitvityScale(fSensScale);
#endif
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*! Get the mouse sensitivity scale
	@return the mouse sensitivity scale
*/
int CScriptObjectInput::GetMouseSensitivityScale(IFunctionHandler *pH)
{
  if(!m_pInput)return pH->EndFunction(1);
	float fSensScale;
#if 0
  if(m_pInput->GetIMouse())
	  fSensScale=m_pInput->GetIMouse()->GetSensitvityScale();
  else
    fSensScale = 20.0f;
#endif
	return pH->EndFunction(fSensScale);
}

//////////////////////////////////////////////////////////////////////////
// joypad
int CScriptObjectInput::SetJoySensitivityHGain(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);

	float fHGain;
	pH->GetParam(1,fHGain);

	//m_pInput->SetJoySensitivityHGain(m_pInput->JoyGetDefaultControllerId(),fHGain);

	return pH->EndFunction();
}
int CScriptObjectInput::SetJoySensitivityHScale(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);

	float fHScale;
	pH->GetParam(1,fHScale);

	//m_pInput->SetJoySensitivityHScale(m_pInput->JoyGetDefaultControllerId(),fHScale);

	return pH->EndFunction();
}
int CScriptObjectInput::SetJoySensitivityVGain(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);

	float fVGain;
	pH->GetParam(1,fVGain);

	//m_pInput->SetJoySensitivityVGain(m_pInput->JoyGetDefaultControllerId(),fVGain);

	return pH->EndFunction();
}
int CScriptObjectInput::SetJoySensitivityVScale(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);

	float fVScale;
	pH->GetParam(1,fVScale);

	//m_pInput->SetJoySensitivityVScale(m_pInput->JoyGetDefaultControllerId(),fVScale);

	return pH->EndFunction();
}
int CScriptObjectInput::GetJoySensitivityHGain(IFunctionHandler *pH)
{
	if(!m_pInput)
		return pH->EndFunction(1);

  float fHGain = 0;// m_pInput->GetJoySensitivityHGain(m_pInput->JoyGetDefaultControllerId());

	return pH->EndFunction(fHGain);
}
int CScriptObjectInput::GetJoySensitivityHScale(IFunctionHandler *pH)
{
	if(!m_pInput)
		return pH->EndFunction(1);

  float fHScale = 0;// m_pInput->GetJoySensitivityHScale(m_pInput->JoyGetDefaultControllerId());

	return pH->EndFunction(fHScale);
}
int CScriptObjectInput::GetJoySensitivityVGain(IFunctionHandler *pH)
{
	if(!m_pInput)
		return pH->EndFunction(1);

  float fVGain = 0;// m_pInput->GetJoySensitivityVGain(m_pInput->JoyGetDefaultControllerId());

	return pH->EndFunction(fVGain);
}
int CScriptObjectInput::GetJoySensitivityVScale(IFunctionHandler *pH)
{
	if(!m_pInput)
		return pH->EndFunction(1);

  float fVScale = 0;// m_pInput->GetJoySensitivityVScale(m_pInput->JoyGetDefaultControllerId());

	return pH->EndFunction(fVScale);
}

//////////////////////////////////////////////////////////////////////
/*! return the name of the first xkey(mouse,joystick and mouse) pressed by the user
	(this method is used to capture the input into the input configuration menu)
	@return a string representing the name of the pressed action
*/
int CScriptObjectInput::GetXKeyPressedName(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
  const char* pszKey = "";// m_pInput->GetXKeyPressedName();
	if (!pszKey)
		return pH->EndFunctionNull();
	return pH->EndFunction(pszKey);
}

//////////////////////////////////////////////////////////////////////
/*! return the name of the first xkey(mouse,joystick and mouse) down
(this method is used to check if a key was pressed)
@return a string representing the name of the pressed action
*/
int CScriptObjectInput::GetXKeyDownName(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
  const char* pszKey = "";// m_pInput->GetKeyDownName();
	if (!pszKey)
		return pH->EndFunctionNull();
	return pH->EndFunction(pszKey);
}

//////////////////////////////////////////////////////////////////////
int CScriptObjectInput::ResetKeyState(IFunctionHandler *pH)
{
	if(!m_pInput)
		return pH->EndFunctionNull();

	if (pH->GetParamCount() > 0)
	{
		const char *szKeyName = 0;

		if (pH->GetParam(1, szKeyName))
		{
      m_pInput->GetDevice(0, eIDT_Keyboard)->ClearKeyState();// ClearKey(m_pInput->GetKeyID(szKeyName));

			return pH->EndFunction();
		}
	}

	m_pInput->ClearKeyState();

	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*! set the inverted mouse state
	@param bEnable !=nil(enabled) nil(disabled)
*/
int CScriptObjectInput::SetInvertedMouse(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(1);
	bool bEnable=false;
	pH->GetParam(1,bEnable);
	m_pGame->m_pIActionMapManager->SetInvertedMouse(bEnable);
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*! return the inverted mouse state
	@return !=nil(enabled) nil(disabled)
*/
int CScriptObjectInput::GetInvertedMouse(IFunctionHandler *pH)
{
	if(!m_pInput)return pH->EndFunctionNull();
	CHECK_PARAMETERS(0);
	return pH->EndFunction(m_pGame->m_pIActionMapManager->GetInvertedMouse());
}
