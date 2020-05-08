#pragma once

#include <BlackBox/Renderer/ITechniqueManager.hpp>

class TechniqueManager : public ITechniqueManager
{
private:
  TechniqueManager();
  ~TechniqueManager();
public:
  ITechnique* get(std::string name) override;
  ITechnique* add(std::string name, ITechnique* tech) override { return nullptr; };
  static TechniqueManager* instance();
  static bool init();

private:
  static std::map<std::string, ITechnique*> techs;
  static TechniqueManager* manager;
};