#pragma once

#include <string>
#include <map>
#include <BlackBox/Renderer/ITechnique.hpp>

class TechniqueManager
{
private:
  TechniqueManager();
  ~TechniqueManager();
public:
  static ITechnique* get(std::string name);
  static ITechnique* add(std::string name, ITechnique* tech);
  static bool init();

private:
  static std::map<std::string, ITechnique*> techs;
};