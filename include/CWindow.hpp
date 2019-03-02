#pragma once

#include "IWindow.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class CWindow : public IWindow {
private:
  GLFWwindow* m_handle; 
  bool m_bClose;
public: 
  CWindow();
  ~CWindow();
  bool create();
  bool init();
  bool update();
  bool shouldCose();
  HWND get();

private:
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
};
