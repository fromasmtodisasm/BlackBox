#include <BlackBox/Input/ActionMap.hpp>
#include <BlackBox/Input/ActionMapManager.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ILog.hpp>

CActionMapManager::CActionMapManager(IInput* pInput)
{
	pInput->SetExclusiveListener(static_cast<IInputEventListener*>(this));
	for (auto k : m_Keys)
	{
		k.empty = true;
	}
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
	auto binding = m_ActionBindingMap.find(m_CurrentActionMap->second);

	uint32 i = 0;
	for (auto &event : m_Queue)
	{
		for (auto& bind : binding->second)
		{
			if (!m_ActionMapSink)
				return;
			if (bind.bind.nKey ==  event && ((bind.bind.nModifier == m_Modifires) || (bind.bind.nModifier == eMM_None)))
			{
				if ((m_Modifires != eMM_None) && (bind.bind.nModifier == eMM_None))
					;
				//continue;
				for (std::size_t i = 0; i < m_ActionList.size(); i++)
				{
					if (m_ActionList[i].aam == m_Keys[event].aam)
					{
						m_ActionMapSink->OnAction(bind.id, m_Keys[event].value, m_Keys[event].ae);
						break;
					}
				}
			}
			else if (bind.bind.nKey ==  event)
			{
				gEnv->pLog->Log("fffffffffffffff");
			}
		}
		i++;
	}
	m_Queue.erase(eKI_MouseX);
	m_Queue.erase(eKI_MouseY);
	m_Queue.erase(eKI_MouseZ);
	#if 0
	for (auto it : m_EventRelease)
	{
		m_EventBuffer.erase(it);	
	}

	m_EventRelease.clear();
	#endif
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
	m_Modifires = event.modifiers;

	switch (event.state)
	{
	case EInputState::eIS_Pressed:
	case EInputState::eIS_Changed:
	case EInputState::eIS_Down:
	{
		EvnetBufferEntry ebe;
		ebe.ae = XActivationEvent::etPressing;
		ebe.aam = XActionActivationMode::aamOnPress;
		ebe.modifires = event.modifiers;
		ebe.value	  = event.value;
		ebe.empty	  = false;
		if (m_Keys[event.keyId].empty)
		{
			m_Keys[event.keyId] = ebe;
		}
		else
		{
			auto &e = m_Keys[event.keyId];
			e.ae = XActivationEvent::etHolding;
			e.aam = XActionActivationMode::aamOnHold;
			e.value = event.value;
		}
		m_Queue.insert(event.keyId);
	}
	break;
	case EInputState::eIS_Released:
	{
		m_Keys[event.keyId].empty = true;
		m_Queue.erase(event.keyId);
		/*
		EvnetBufferEntry ebe;
		ebe.ae = XActivationEvent::etPressing;
		ebe.aam = XActionActivationMode::aamOnRelease;
		ebe.bindInfo.nKey = event.keyId;
		ebe.bindInfo.nModifier = event.modifiers;
		if (auto &it = m_EventBuffer.find(ebe); it != m_EventBuffer.end())
		{
			assert(it != m_EventBuffer.end());
			m_EventRelease.push_back(it);
			//m_EventQueue.push_back(it);
		}
		*/
	}
	break;
	default:
		break;
	}
	return false;
}

void CActionMapManager::AddBind(CActionMap* mpa, const ActionBinding& actionBinding)
{
	m_ActionBindingMap[mpa].push_back(actionBinding);
}
