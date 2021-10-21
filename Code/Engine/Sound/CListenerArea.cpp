#include "CListenerArea.hpp"

int CListenerArea::GetVisFrameId()
{
	return 0;
}

int CListenerArea::GetVisAreaConnections(IVisArea** pAreas, int nMaxConnNum, bool bSkipDisabledPortals/* = false*/)
{
	return 0;
}

bool CListenerArea::IsConnectedToOutdoor()
{
	return false;
}

const char* CListenerArea::GetName()
{
	return m_Name.data();
}

bool CListenerArea::IsPortal()
{
	return false;
}

bool CListenerArea::FindVisArea(IVisArea* pAnotherArea, int nMaxRecursion, bool bSkipDisabledPortals)
{
	return false;
}

bool CListenerArea::IsAfectedByOutLights()
{
	return false;
}
