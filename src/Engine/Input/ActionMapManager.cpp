#include <BlackBox/Input/ActionMapManager.hpp>
#include <BlackBox/Input/ActionMap.hpp>

CActionMapManager::CActionMapManager(IInput* pInput)
{
	pInput->AddEventListener(this);
}

CActionMapManager::~CActionMapManager()
{
}

void CActionMapManager::SetInvertedMouse(bool bEnable)
{
}

bool CActionMapManager::GetInvertedMouse()
{
  return false;
}

void CActionMapManager::RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam)
{
}

void CActionMapManager::SetSink(IActionMapSink* pSink)
{
}

void CActionMapManager::CreateAction(XACTIONID nActionID, const char* sActionName, XActionActivationMode aam /* = aamOnPress*/)
{
	m_ActionList.emplace_back(ActionInfo(nActionID, sActionName, aam));
}

IActionMap* CActionMapManager::CreateActionMap(const char* s)
{
  return (m_ActionMaps[string(s)] = new CActionMap(this));
}

IActionMap* CActionMapManager::GetActionMap(const char* s)
{
  return nullptr;
}

void CActionMapManager::ResetAllBindings()
{
}

void CActionMapManager::GetActionMaps(IActionMapDumpSink* pCallback)
{
}

void CActionMapManager::SetActionMap(const char* s)
{
}

bool CActionMapManager::CheckActionMap(XACTIONID nActionID)
{
  return false;
}

bool CActionMapManager::CheckActionMap(const char* sActionName)
{
  return false;
}

void CActionMapManager::Reset()
{
}

void CActionMapManager::Update(unsigned int nTimeMSec)
{
}

void CActionMapManager::Release()
{
}

void CActionMapManager::Enable()
{
}

void CActionMapManager::Disable()
{
}

bool CActionMapManager::IsEnabled()
{
  return false;
}

bool CActionMapManager::OnInputEvent(const SInputEvent& event)
{
	return false;
}
