#pragma once
#include <BlackBox/Input/IInput.hpp>

class CInputActionMap : public IInputActionMap
{
  CInputActionMap();
  ~CInputActionMap();
  // Inherited via IInputActionMap
  virtual void SetSink(IInputActionTriggerSink* pIActionTrigger) override;
  virtual void Release() override;
  virtual void Update() override;
  virtual void CallActionTrigger(INPUTACTIONID nActionID, float fValue) override;
  virtual float CheckAction(const INPUTACTIONID nActionID) override;
  virtual bool SetAction(const INPUTACTIONID nActionID, bool bCheckPressed, const char* szCodes, const char* szMods = NULL) override;
  virtual void ClearAction(const INPUTACTIONID nActionID) override;
};
