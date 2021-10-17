#pragma once
class CListenerArea : public IVisArea
{
  public:
	// Inherited via IVisArea
	virtual int GetVisFrameId() override;
	virtual int GetVisAreaConnections(IVisArea** pAreas, int nMaxConnNum, bool bSkipDisabledPortals = false) override;
	virtual bool IsConnectedToOutdoor() override;
	virtual const char* GetName() override;
	virtual bool		IsPortal() override;
	virtual bool		FindVisArea(IVisArea* pAnotherArea, int nMaxRecursion, bool bSkipDisabledPortals) override;
	virtual bool		IsAfectedByOutLights() override;

	int m_VisFrameId{};
	string m_Name{"UnnamedArea"};
};
