#pragma once
#include <BlackBox/Input/IInput.hpp>

class CActiontMap : public IActionMap
{
  public:
	CActiontMap();
	~CActiontMap();

	// Inherited via IActionMap
	virtual void ResetAllBindings() override;

	virtual void ResetBinding(XACTIONID nActionID) override;

	virtual void RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam) override;

	virtual void BindAction(XACTIONID nActionID, XBind& NewBind, int iKeyPos = -1) override;

	virtual void BindAction(XACTIONID nActionID, int nKey, int nModifier = eKI_Unknown, int iKeyPos = -1) override;

	virtual void BindAction(XACTIONID nActionID, const char* sKey, const char* sModifier = NULL, int iKeyPos = -1) override;

	virtual void GetBinding(XACTIONID nActionID, int nKeyPos, XBind& Bind) override;

	virtual void GetBinding(XACTIONID nActionID, int nKeyPos, int& nKey, int& nModifier) override;

	virtual void GetBinding(XACTIONID nActionID, int nKeyPos, char* pszKey, char* pszModifier) override;

	virtual void GetBindDifferences(IActionMap* pActionMap, std::vector<int>& keys) override;
};