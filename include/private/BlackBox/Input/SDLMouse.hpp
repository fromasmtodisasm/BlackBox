#ifndef __SDLMOUSE_H__
#define __SDLMOUSE_H__

#include "SDLInput.hpp"

struct IRenderer;
class CSDLMouse : public CLinuxInputDevice
{
public:
  CSDLMouse(CLinuxInput& input);

  virtual ~CSDLMouse();

  virtual int  GetDeviceIndex() const { return 0; }   //Assume only one mouse

  virtual bool Init();
  virtual void PostInit();

  virtual void Update(bool focus);

  void         GrabInput();

  void         UngrabInput();

protected:

  void CapDeltas(float cap);

  void SmoothDeltas(float accel, float decel = 0.0f);

private:
  IRenderer* m_pRenderer;
  int        m_posX;
  int        m_posY;
  Vec2       m_deltas;
  Vec2       m_oldDeltas;
  Vec2       m_deltasInertia;
  bool       m_bGrabInput;
};

#endif // __SDLMOUSE_H__