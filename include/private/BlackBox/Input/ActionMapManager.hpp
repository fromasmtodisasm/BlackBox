#pragma once
#include <BlackBox/Input/IInput.hpp>

class CActionMapManager : public IActionMapManager
{
  public:
	CActionMapManager();
	~CActionMapManager();
	// Inherited via IActionMapManager
	virtual void SetInvertedMouse(bool bEnable) override;
	virtual bool GetInvertedMouse() override;
	virtual void RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam) override;
	virtual void SetSink(IActionMapSink* pSink) override;
	virtual void CreateAction(XACTIONID nActionID, const char* sActionName, XActionActivationMode aam = aamOnPress) override;
	virtual IActionMap* CreateActionMap(const char* s) override;
	virtual IActionMap* GetActionMap(const char* s) override;
	virtual void ResetAllBindings() override;
	virtual void GetActionMaps(IActionMapDumpSink* pCallback) override;
	virtual void SetActionMap(const char* s) override;
	virtual bool CheckActionMap(XACTIONID nActionID) override;
	virtual bool CheckActionMap(const char* sActionName) override;
	virtual void Reset() override;
	virtual void Update(unsigned int nTimeMSec) override;
	virtual void Release() override;
	virtual void Enable() override;
	virtual void Disable() override;
	virtual bool IsEnabled() override;
};
