#ifndef __SDLKEYBOARD_H__
#define __SDLKEYBOARD_H__

#include "SDLInput.hpp"

class CSDLMouse;

class CSDLKeyboard : public CLinuxInputDevice
{
  friend class CLinuxInputDevice;
public:

  CSDLKeyboard(CLinuxInput& input);

  virtual ~CSDLKeyboard();

  virtual int  GetDeviceIndex() const { return 0; }   //Assume only one keyboard

  virtual bool Init();

  virtual void Update(bool focus);

  virtual char GetInputCharAscii(const SInputEvent& event);

protected:
  static int ConvertModifiers(unsigned);

private:
  unsigned char Event2ASCII(const SInputEvent& event);
  void          SetupKeyNames();

private:
  unsigned m_lastKeySym;
  int      m_lastMod;
  //unsigned m_lastUNICODE;
};

#endif