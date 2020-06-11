#pragma once
#include <BlackBox/Input/IInput.hpp>
class CActionMapManager;
class CActionMap : public IActionMap
{
public:
  CActionMap(CActionMapManager* pActionMapManager);
  ~CActionMap();

  // Inherited via IActionMap
  virtual void ResetAllBindings() override;

  virtual void ResetBinding(XACTIONID nActionID) override;

  virtual void RemoveBind(XACTIONID nActionID, XBind& NewBind, XActionActivationMode aam) override;

  virtual void BindAction(XACTIONID nActionID, XBind& NewBind, int iKeyPos = -1) override;

  virtual void BindAction(XACTIONID nActionID, uint32 nKey, EModifierMask nModifier, int iKeyPos = -1) override;

  virtual void BindAction(XACTIONID nActionID, const char* sKey, const char* sModifier = NULL, int iKeyPos = -1) override;

  virtual void GetBinding(XACTIONID nActionID, int nKeyPos, XBind& Bind) override;

  virtual void GetBinding(XACTIONID nActionID, int nKeyPos, int& nKey, int& nModifier) override;

  virtual void GetBinding(XACTIONID nActionID, int nKeyPos, char* pszKey, char* pszModifier) override;

  virtual void GetBindDifferences(IActionMap* pActionMap, std::vector<int>& keys) override;

private:
  class CActionMapManager* m_ActionMapManager;

};