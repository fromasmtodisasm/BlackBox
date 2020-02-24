#include <BlackBox/Input/ActionMap.hpp>

CActiontMap::CActiontMap()
{
}

CActiontMap::~CActiontMap()
{
}

void CActiontMap::ResetAllBindings()
{
}

void CActiontMap::ResetBinding(XACTIONID nActionID)
{
}

void CActiontMap::RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam)
{
}

void CActiontMap::BindAction(XACTIONID nActionID, XBind& NewBind, int iKeyPos /* = -1*/)
{
}

void CActiontMap::BindAction(XACTIONID nActionID, int nKey, int nModifier /* = eKI_Unknown*/, int iKeyPos /* = -1*/)
{
}

void CActiontMap::BindAction(XACTIONID nActionID, const char* sKey, const char* sModifier /* = NULL*/, int iKeyPos /* = -1*/)
{
}

void CActiontMap::GetBinding(XACTIONID nActionID, int nKeyPos, XBind& Bind)
{
}

void CActiontMap::GetBinding(XACTIONID nActionID, int nKeyPos, int& nKey, int& nModifier)
{
}

void CActiontMap::GetBinding(XACTIONID nActionID, int nKeyPos, char* pszKey, char* pszModifier)
{
}

void CActiontMap::GetBindDifferences(IActionMap* pActionMap, std::vector<int>& keys)
{
}
