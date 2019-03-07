#pragma once
#include "IGame.hpp"
#include "IWindow.hpp"
#include "CShader.hpp"

#include <common.h>
#include <map>
#include <string>
#include <vector>

using string = std::string;

class CGame : public IGame {
private:
  IWindow *m_Window;
  char *m_Title;
  CShaderProgram *m_ShaderProgram; 
public:
  CGame(char *);
  bool init();
  bool update();
  bool run();
};
