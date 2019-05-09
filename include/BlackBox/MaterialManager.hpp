#pragma once

#include <BlackBox/Material.hpp>

#include <map>
#include <string>
#include <tinyxml2.h>

extern Material *defaultMaterial;

class MaterialManager
{
  static MaterialManager *manager;
  std::map<std::string, Material*> cache;
  //MaterialManager();
public:
  static MaterialManager *instance();
  Material *getProgram(std::string vMaterial, std::string fMaterial);
  static bool init();
  Material *getMaterial(std::string name);
  static bool init(std::string materialLib);

private:
  bool loadLib(std::string name);
  bool loadMaterial(tinyxml2::XMLElement *material);
  Texture *loadTexture(tinyxml2::XMLElement *texture);
  CShader *loadShader(tinyxml2::XMLElement *shader);
};
