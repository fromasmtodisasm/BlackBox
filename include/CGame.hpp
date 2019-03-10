#pragma once
#include "IGame.hpp"
#include "IWindow.hpp"
#include "CShader.hpp"
#include "Triangle.hpp"
#include "World.hpp"

#include <common.h>
#include <map>
#include <string>
#include <vector>
#include <memory>

using string = std::string;

class CGame : public IGame {
private:
  IWindow *m_Window;
  char *m_Title;
  CShaderProgram *m_ShaderProgram; 
  std::vector<Object*> m_Objects;
  World world;
public:
  CGame(char *);
  ~CGame() = default;
  bool init(bool debug);
  bool update();
  bool run();
};
