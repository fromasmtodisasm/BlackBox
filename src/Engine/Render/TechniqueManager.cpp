#include "TechniqueManager.hpp"
#include <BlackBox/Render/HdrTechnique.hpp>

std::map<std::string, ITechnique*> TechniqueManager::techs;

ITechnique* TechniqueManager::get(std::string name)
{
  auto it = techs.find(name);
  if (it != techs.end())
    return it->second;
  else
    return nullptr;
}

bool TechniqueManager::init()
{
  techs["hdr"] = new HdrTechnique();
  return true;
}

TechniqueManager::TechniqueManager()
{
}

TechniqueManager::~TechniqueManager()
{
}
