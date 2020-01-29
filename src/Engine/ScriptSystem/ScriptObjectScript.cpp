#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectScript.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/System/ILog.hpp>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectScript)

CScriptObjectScript::CScriptObjectScript()
{
}

CScriptObjectScript::~CScriptObjectScript()
{
}

void CScriptObjectScript::InitializeTemplate(IScriptSystem* pSS)
{
  _ScriptableEx<CScriptObjectScript>::InitializeTemplate(pSS);
#undef SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_CLASSNAME CScriptObjectScript

  SCRIPT_REG_FUNC(ReloadScripts);
  SCRIPT_REG_FUNC(ReloadScript);
  SCRIPT_REG_FUNC(LoadScript);
  SCRIPT_REG_FUNC(UnloadScript);
  SCRIPT_REG_FUNC(DumpLoadedScripts);
#if FULL_SUPPORT
  SCRIPT_REG_TEMPLFUNC(ReloadEntityScript, "className");
  SCRIPT_REG_TEMPLFUNC(SetTimer, "nMilliseconds,Function");
  SCRIPT_REG_TEMPLFUNC(SetTimerForFunction, "nMilliseconds,Function");
  SCRIPT_REG_TEMPLFUNC(KillTimer, "nTimerId");
#endif
}

void CScriptObjectScript::Init(IScriptSystem* pScriptSystem)
{
  InitGlobal(pScriptSystem, "Script", this);
}

/*!reload all previosly loaded scripts
 */
int CScriptObjectScript::ReloadScripts(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(0);
  m_pSS->ReloadScripts();
  return pH->EndFunction();
}

/*!reload a specified script. If the script wasn't loaded at least once before the function will fail
   @param sFileName path of the script that has to be reloaded
 */
int CScriptObjectScript::ReloadScript(IFunctionHandler* pH)
{
  const char* sFileName;
  if (!pH->GetParam(1, sFileName))
    return pH->EndFunction();

  m_pSS->ExecuteFile(sFileName, true/*, gEnv->IsEditor()*/, true); // Force reload if in editor mode

  return pH->EndFunction();
}

#if FULL_SUPPORT
int CScriptObjectScript::ReloadEntityScript(IFunctionHandler* pH, const char* className)
{
  IEntitySystem* pEntitySystem = gEnv->pEntitySystem;

  IEntityClass* pClass = pEntitySystem->GetClassRegistry()->FindClass(className);

  if (pClass)
  {
    pClass->LoadScript(false);
  }

  return pH->EndFunction();
}
#endif

/*!load a specified script
   @param sFileName path of the script that has to be loaded
 */
int CScriptObjectScript::LoadScript(IFunctionHandler* pH)
{
  bool bReload = false;
  bool bRaiseError = true;

  if (pH->GetParamCount() >= 3)
  {
    pH->GetParam(3, bRaiseError);
  }
  if (pH->GetParamCount() >= 2)
  {
    pH->GetParam(2, bReload);
  }

  const char* sScriptFile;
  pH->GetParam(1, sScriptFile);

  if (m_pSS->ExecuteFile(sScriptFile, bRaiseError, bReload))
    return pH->EndFunction(1);
  else
    return pH->EndFunction();
}

/*!unload script from the "loaded scripts map" so if this script is loaded again
   the Script system will reloadit. this function doesn't
   involve the LUA VM so the resources allocated by the script will not be released
   unloading the script
   @param sFileName path of the script that has to be loaded
 */
int CScriptObjectScript::UnloadScript(IFunctionHandler* pH)
{
  const char* sScriptFile;
  if (!pH->GetParam(1, sScriptFile))
    return pH->EndFunction();

  m_pSS->UnloadScript(sScriptFile);
  return pH->EndFunction();
}

/*!Dump all loaded scripts path calling IScriptSystemSink::OnLoadedScriptDump
   @see IScriptSystemSink
 */
int CScriptObjectScript::DumpLoadedScripts(IFunctionHandler* pH)
{
  m_pSS->DumpLoadedScripts();
  return pH->EndFunction();
}

#if FULL_SUPPORT
//////////////////////////////////////////////////////////////////////////
int CScriptObjectScript::SetTimer(IFunctionHandler* pH, int nMilliseconds, HSCRIPTFUNCTION hFunc)
{
  SmartScriptTable pUserData;
  bool bUpdateDuringPause = false;
  if (pH->GetParamCount() > 2)
  {
    pH->GetParam(3, pUserData);
  }
  if (pH->GetParamCount() > 3)
  {
    pH->GetParam(4, bUpdateDuringPause);
  }
  ScriptTimer timer;
  timer.bUpdateDuringPause = bUpdateDuringPause;
  timer.sFuncName[0] = 0;
  timer.pScriptFunction = hFunc;
  timer.pUserData = pUserData;
  timer.nMillis = nMilliseconds;

  int nTimerId = ((CScriptSystem*)m_pSS)->GetScriptTimerMgr()->AddTimer(timer);
  ScriptHandle timerHandle;
  timerHandle.n = nTimerId;

  return pH->EndFunction(timerHandle);
}

//////////////////////////////////////////////////////////////////////////
int CScriptObjectScript::SetTimerForFunction(IFunctionHandler* pH, int nMilliseconds, const char* sFunctionName)
{
  SmartScriptTable pUserData;
  bool bUpdateDuringPause = false;
  if (pH->GetParamCount() > 2)
  {
    pH->GetParam(3, pUserData);
  }
  if (pH->GetParamCount() > 3)
  {
    pH->GetParam(4, bUpdateDuringPause);
  }
  ScriptTimer timer;
  timer.bUpdateDuringPause = bUpdateDuringPause;
  if (strlen(sFunctionName) > (sizeof(timer.sFuncName) - 1))
  {
    gEnv->pLog->LogError("SetTimerForFunction: Function name too long - %s", sFunctionName);
  }
  else
  {
    cry_strcpy(timer.sFuncName, sFunctionName);
  }
  timer.pScriptFunction = 0;
  timer.pUserData = pUserData;
  timer.nMillis = nMilliseconds;

  int nTimerId = ((CScriptSystem*)m_pSS)->GetScriptTimerMgr()->AddTimer(timer);
  ScriptHandle timerHandle;
  timerHandle.n = nTimerId;

  return pH->EndFunction(timerHandle);
}

//////////////////////////////////////////////////////////////////////////
int CScriptObjectScript::KillTimer(IFunctionHandler* pH, ScriptHandle nTimerId)
{
  int nid = (int)nTimerId.n;
  ((CScriptSystem*)m_pSS)->GetScriptTimerMgr()->RemoveTimer(nid);
  return pH->EndFunction();
}
#endif
