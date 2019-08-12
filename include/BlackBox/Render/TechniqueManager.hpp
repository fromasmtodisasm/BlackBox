#pragma once

#include <string>
#include <map>
#include <BlackBox/Render/ITechnique.hpp>

class TechniqueManager
{
private:
  TechniqueManager();
  ~TechniqueManager();
public:
  static ITechnique *get(std::string name);
  static bool init();

private:
  static std::map<std::string, ITechnique*> techs;
};
