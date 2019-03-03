#pragma once

#include "IWindow.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef __cplusplus >= 2001103L

#include "common.h"
#include <map>

#else

#error "not corrected c++ standart ( c++11 )"

#endif

typedef std::function<void(GLFWwindow *window, int key, int scancode, int action, int mode)> kcbfunc;

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
	bool setCallback(int key,kcbfunc);
  HWND get();

private:
	std::map<int, kcbfunc> kcbfuncs;
  static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
};
