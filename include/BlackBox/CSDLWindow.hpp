#pragma once

#include <BlackBox/IWindow.hpp>
#include <SDL2/SDL.h>

class CSDLWindow : public IWindow {
private:
 SDL_Window* m_Win = nullptr;
 SDL_Surface* m_Ssurface = nullptr;
 SDL_GLContext m_GLcontext;

 int m_Xpos,m_Ypos;
 int m_Width,m_Height;

public:
 CSDLWindow();
 CSDLWindow(int x,int y,int w = 640,int h = 480);
 ~CSDLWindow() = default;
 bool create();
 bool init();
 void update();
 void clear();
 bool closed();
 void swap();
 void setTitle(const char *);
 void show();
 int getWidth();
 int getHeight();
 void *getHandle();
 void setFlags(int flags);
};
