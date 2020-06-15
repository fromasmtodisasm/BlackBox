#ifndef __LINUXINPUT_H__
#define __LINUXINPUT_H__
#pragma once

#include "BaseInput.hpp"
#include "InputDevice.hpp"

class CSDLPadManager;
class CSDLMouse;
struct ILog;

#if !BB_PLATFORM_ANDROID && !BB_PLATFORM_IOS
#define SDL_USE_HAPTIC_FEEDBACK
#endif

class CLinuxInput : public CBaseInput
{
public:
  CLinuxInput(ISystem* pSystem);

  virtual bool Init() override;
  virtual void ShutDown() override;
  virtual void Update(bool focus) override;
  virtual bool GrabInput(bool bGrab) override;
  virtual int  ShowCursor(const bool bShow) override;

private:
  ISystem* m_pSystem;
  ILog* m_pLog;
  CSDLPadManager* m_pPadManager;
  CSDLMouse* m_pMouse;

  virtual IActionMapManager* CreateActionMapManager() override;
};

class CLinuxInputDevice : public CInputDevice
{
public:
  CLinuxInputDevice(CLinuxInput& input, const char* deviceName);
  virtual ~CLinuxInputDevice();

  CLinuxInput& GetLinuxInput() const;
protected:
  void         PostEvent(SInputSymbol* pSymbol, unsigned keyMod =  0);
private:
  CLinuxInput& m_linuxInput;
};

struct ILog;
struct ICVar;

#endif