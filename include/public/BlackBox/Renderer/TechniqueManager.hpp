#pragma once

#include <BlackBox/Renderer/ITechnique.hpp>
#include <map>
#include <string>

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