#include <BlackBox/Renderer/TechniqueManager.hpp>
#include <BlackBox/Renderer/HdrTechnique.hpp>

std::map<std::string, ITechnique*> TechniqueManager::techs;
TechniqueManager* TechniqueManager::manager = nullptr;

ITechnique* TechniqueManager::get(std::string name)
{
  auto it = techs.find(name);
  if (it != techs.end())
    return it->second;
  else
    return nullptr;
}

TechniqueManager* TechniqueManager::instance()
{
	return nullptr;
  if (manager == nullptr)
  {
    //manager = new TechniqueManager();
    //manager->init();
  }
  return manager;
}


bool TechniqueManager::init()
{
  //techs["hdr"] = new HdrTechnique();
  return true;
}

TechniqueManager::TechniqueManager()
{
}

TechniqueManager::~TechniqueManager()
{
}