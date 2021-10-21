#include <BlackBox/Input/InputActionMap.hpp>

CInputActionMap::CInputActionMap()
{
}

CInputActionMap::~CInputActionMap()
{
}

void CInputActionMap::SetSink(IInputActionTriggerSink* pIActionTrigger)
{
}

void CInputActionMap::Release()
{
}

void CInputActionMap::Update()
{
}

void CInputActionMap::CallActionTrigger(INPUTACTIONID nActionID, float fValue)
{
}

float CInputActionMap::CheckAction(const INPUTACTIONID nActionID)
{
  return 0.0f;
}

bool CInputActionMap::SetAction(const INPUTACTIONID nActionID, bool bCheckPressed, const char* szCodes, const char* szMods/* = NULL*/)
{
  return false;
}

void CInputActionMap::ClearAction(const INPUTACTIONID nActionID)
{
}
