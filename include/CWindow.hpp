#pragma once

#include "IWindow.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window : public IWindow {
private:
  GLFWwindow* m_handle; 
public: 
  Window();
  ~Window();
  bool create();
  bool init();
  void run();
};
