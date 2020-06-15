#include <BlackBox/Input/ActionMap.hpp>
#include <BlackBox/Input/ActionMapManager.hpp>

CActionMapManager::CActionMapManager(IInput* pInput)
{
	pInput->SetExclusiveListener(static_cast<IInputEventListener*>(this));
}

CActionMapManager::~CActionMapManager()
{
	for (auto am : m_ActionMaps)
	{
		delete am.second;
	}
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
	for (std::size_t i = 0; i < m_ActionList.size(); i++)
	{
		if (m_ActionList[i].nActionID == nActionID)
		{
			m_ActionList.erase(m_ActionList.begin() + i);
			break;
		}
	}
}

void CActionMapManager::SetSink(IActionMapSink* pSink)
{
	m_ActionMapSink = pSink;
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
	if (auto it = m_ActionMaps.find(s); it != m_ActionMaps.end())
	{
		return it->second;
	}
	return nullptr;
}

void CActionMapManager::ResetAllBindings()
{
}

void CActionMapManager::GetActionMaps(IActionMapDumpSink* pCallback)
{
	for (auto& am : m_ActionMaps)
	{
		pCallback->OnElementFound(am.first.c_str(), am.second);
	}
}

void CActionMapManager::SetActionMap(const char* s)
{
	if (auto it = m_ActionMaps.find(s); it != m_ActionMaps.end())
	{
		m_CurrentActionMap = it;
	}
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
	delete this;
}

void CActionMapManager::Enable()
{
	m_Enabled = true;
}

void CActionMapManager::Disable()
{
	m_Enabled = false;
}

bool CActionMapManager::IsEnabled()
{
	return m_Enabled;
}

bool CActionMapManager::OnInputEvent(const SInputEvent& event)
{
	if (event.keyId == eKI_SYS_Commit)
		return false;
	bool keyPressed	 = event.state == eIS_Pressed;
	bool keyReleased = event.state == eIS_Released;

	auto binding = m_ActionBindingMap.find(m_CurrentActionMap->second);

	for (auto& bind : binding->second)
	{
		if (bind.bind.nKey == event.keyId && ((bind.bind.nModifier == event.modifiers) || (bind.bind.nModifier == eMM_None)))
		{
			if ((event.modifiers != eMM_None) && (bind.bind.nModifier == eMM_None))
				continue;
			XActionActivationMode aam;
			switch (event.state)
			{
			case EInputState::eIS_Pressed:
			case EInputState::eIS_Changed:
			case EInputState::eIS_Down:
				if (auto it = m_keys.find(event.keyId); it == m_keys.end())
				{
					m_keys.insert(event.keyId);
					aam = XActionActivationMode::aamOnHold;
				}
				else
				{
					aam = XActionActivationMode::aamOnHold;
				}
				break;
			case EInputState::eIS_Released:
				m_keys.erase(event.keyId);
				aam = XActionActivationMode::aamOnRelease;
				break;
			default:
				break;
			}
			for (std::size_t i = 0; i < m_ActionList.size(); i++)
			{
				if (m_ActionList[i].aam == aam)
				{
					m_ActionMapSink->OnAction(bind.id, event.value, etHolding);
					break;
				}
			}
		}
	}

	return false;
}

void CActionMapManager::AddBind(CActionMap* mpa, const ActionBinding& actionBinding)
{
	m_ActionBindingMap[mpa].push_back(actionBinding);
}
