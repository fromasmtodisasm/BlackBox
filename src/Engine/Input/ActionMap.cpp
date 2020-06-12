#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Input/ActionMap.hpp>
#include <BlackBox/Input/ActionMapManager.hpp>

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
}

void CActionMap::BindAction(XACTIONID nActionID, XBind& NewBind, int iKeyPos/* = -1*/)
{
}

void CActionMap::BindAction(XACTIONID nActionID, uint32 nKey, EModifierMask nModifier/* = eKI_Unknown*/, int iKeyPos/* = -1*/)
{
	XBind bind;
	bind.nKey = nKey;
	bind.nModifier = nModifier;
	m_ActionMapManager->AddBind(this, ActionBinding{nActionID, bind});
}

void CActionMap::BindAction(XACTIONID nActionID, const char* sKey, const char* sModifier/* = NULL*/, int iKeyPos/* = -1*/)
{
	XBind bind;
	if (auto symbol = gEnv->pInput->GetSymbolByName(sKey); symbol)
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
}

void CActionMap::GetBinding(XACTIONID nActionID, int nKeyPos, char* pszKey, char* pszModifier)
{
	*pszKey = '\0';
	*pszModifier = '\0';
	if (auto binding = m_ActionMapManager->m_ActionBindingMap.find(this); binding != m_ActionMapManager->m_ActionBindingMap.end())
	{
		for (auto &bind : binding->second)
		{
			if (bind.id == nActionID)
			{
				if (auto key = gEnv->pInput->GetKeyName(EKeyId(bind.bind.nKey)); key)
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
