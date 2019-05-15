#include "BlackBox/CSDLWindow.hpp"

bool sdl_init = false;

CSDLWindow::CSDLWindow() {}

CSDLWindow::CSDLWindow(int x,int y,int w,int h) {
  m_Xpos = SDL_WINDOWPOS_UNDEFINED;
  m_Ypos = SDL_WINDOWPOS_UNDEFINED;
  m_Width = SCREEN_WIDTH;
  m_Height = SCREEN_HEIGHT;
}

bool CSDLWindow::create() {
  return true;
}

bool CSDLWindow::init() {
  if(!sdl_init) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
      return false;
    sdl_init = true;
  }

  m_Win = SDL_CreateWindow(m_Xpos,m_Ypos,m_Width,m_Height, SDL_WINDOW_OPENGL);
  
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);    // использовать двойной буфер
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);    
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);  
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);   

  m_GLcontext = SDL_GL_CreateContext(m_Win);

  return true;
}

bool CSDLWindow::closed() {
  SDL_GL_DeleteContext(m_GLcontext);
  return true;
}

bool CSDLWindow::swap() {
  SDL_GL_SwapWindow(window);
  return true;
}


















