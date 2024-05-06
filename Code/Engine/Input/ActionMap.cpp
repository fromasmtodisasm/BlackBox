#include "pch.hpp"
#include "ActionMap.hpp"
#include "ActionMapManager.hpp"

CActionMap::CActionMap(CActionMapManager* pActionMapManager)
    : m_ActionMapManager(pActionMapManager)
{
}

CActionMap::~CActionMap()
{
}

void CActionMap::ResetAllBindings()
{
}

void CActionMap::ResetBinding(XACTIONID nActionID)
{
}

void CActionMap::RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam)
{
	auto& map = m_ActionMapManager->m_ActionBindingMap;
	if (const auto& binding = map.find(this); binding != map.end())
	{
		for (const auto& [id, bind] : binding->second)
		{
			if (id == nActionID)
			{
				if (bind.nKey == NewBind.nKey && bind.nModifier == NewBind.nModifier)
				{
					map.erase(binding);
					break;
				}
			}
		}
	}
}

void CActionMap::BindAction(XACTIONID nActionID, XBind& NewBind, int iKeyPos /* = -1*/)
{
}

void CActionMap::BindAction(XACTIONID nActionID, uint32 nKey, EModifierMask nModifier /* = eKI_Unknown*/, int iKeyPos /* = -1*/)
{
	XBind bind;
	bind.nKey      = nKey;
	bind.nModifier = nModifier;
	m_ActionMapManager->AddBind(this, ActionBinding{nActionID, bind});
}

void CActionMap::BindAction(XACTIONID nActionID, const char* sKey, const char* sModifier /* = NULL*/, int iKeyPos /* = -1*/)
{
	XBind bind;
	if (auto symbol = Env::Input()->GetSymbolByName(sKey); symbol)
	{
		bind.nKey = symbol->keyId;
		//bind.nModifier = ;
		m_ActionMapManager->AddBind(this, ActionBinding{nActionID, bind});
	}
}

void CActionMap::GetBinding(XACTIONID nActionID, int nKeyPos, XBind& Bind)
{
}

void CActionMap::GetBinding(XACTIONID nActionID, int nKeyPos, int& nKey, int& nModifier)
{
	if (auto binding = m_ActionMapManager->m_ActionBindingMap.find(this); binding != m_ActionMapManager->m_ActionBindingMap.end())
	{
		for (auto& bind : binding->second)
		{
			if (bind.id == nActionID)
			{
				nKey     = bind.bind.nKey;
				nModifier = bind.bind.nModifier;
				break;
			}
		}
	}
}

void CActionMap::GetBinding(XACTIONID nActionID, int nKeyPos, char* pszKey, char* pszModifier)
{
	*pszKey      = '\0';
	*pszModifier = '\0';
	if (auto binding = m_ActionMapManager->m_ActionBindingMap.find(this); binding != m_ActionMapManager->m_ActionBindingMap.end())
	{
		for (auto& bind : binding->second)
		{
			if (bind.id == nActionID)
			{
				if (auto key = Env::Input()->GetKeyName(EKeyId(bind.bind.nKey)); key)
				{
					auto len = strlen(key);
					strncpy(pszKey, key, len);
					pszKey[len] = '\0';
					break;
				}
			}
		}
	}
}

void CActionMap::GetBindDifferences(IActionMap* pActionMap, std::vector<int>& keys)
{
}
