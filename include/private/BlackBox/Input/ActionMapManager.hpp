#pragma once
#include <BlackBox/Input/IInput.hpp>

class CActionMap;
struct ActionInfo
{
	XACTIONID nActionID;
	const char* sActionName;
	XActionActivationMode aam;

  ActionInfo(XACTIONID nActionID, const char* sActionName, XActionActivationMode aam)
		: nActionID(nActionID), sActionName(sActionName), aam(aam)
	{

	}
};

struct ActionBinding
{
	XACTIONID id;
	XBind bind;
};


class CActionMapManager : public IActionMapManager, public IInputEventListener
{
	friend CActionMap;
	using ActionList = std::vector<ActionInfo>;
  using ActionMaps = std::map<string, CActionMap*>;
  using ActionMapsToString = std::map<CActionMap*, string>;
  using ActionMapIt = std::map<string, CActionMap*>::iterator;
  using ActionBindingMap = std::map<CActionMap*, std::vector<ActionBinding>>;
public:
  CActionMapManager(IInput* pInput);
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

  void BindAction();

  // Унаследовано через IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) override;


public:
  void AddBind(CActionMap* mpa, const ActionBinding& actionBinding);

private:
  ActionMaps m_ActionMaps;
  ActionMapIt m_CurrentActionMap;
  ActionList m_ActionList;
  ActionBindingMap m_ActionBindingMap;
  ActionMapsToString m_ActionMapsToString;

  bool m_Enabled = true;

	IActionMapSink* m_ActionMapSink{};
  std::set<EKeyId> m_keys;

};
