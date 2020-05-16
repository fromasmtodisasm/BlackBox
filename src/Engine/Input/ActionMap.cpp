#include <BlackBox/Input/ActionMap.hpp>
#include <BlackBox/Input/ActionMapManager.hpp>

CActionMap::CActionMap(IActionMapManager* pActionMapManager) 
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

void CActionMap::BindAction(XACTIONID nActionID, int nKey, int nModifier/* = eKI_Unknown*/, int iKeyPos/* = -1*/)
{

}

void CActionMap::BindAction(XACTIONID nActionID, const char* sKey, const char* sModifier/* = NULL*/, int iKeyPos/* = -1*/)
{
}

void CActionMap::GetBinding(XACTIONID nActionID, int nKeyPos, XBind& Bind)
{
}

void CActionMap::GetBinding(XACTIONID nActionID, int nKeyPos, int& nKey, int& nModifier)
{
}

void CActionMap::GetBinding(XACTIONID nActionID, int nKeyPos, char* pszKey, char* pszModifier)
{
}

void CActionMap::GetBindDifferences(IActionMap* pActionMap, std::vector<int>& keys)
{
}
