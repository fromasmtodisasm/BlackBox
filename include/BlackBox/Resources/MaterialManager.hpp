#pragma once

#include <BlackBox/Material.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/ILog.hpp>

#include <map>
#include <string>
#include <tinyxml2.h>

extern Material *defaultMaterial;

class MaterialManager
{
  static MaterialManager *manager;
  std::map<std::string, Material*> cache;
  ILog *m_pLog;
	bool alpha_shakaled = false;
	bool isSkyBox = false;
	std::map<std::string, CBaseShaderProgram*> shaders_map;
public:
  static MaterialManager *instance();
  Material *getProgram(std::string vMaterial, std::string fMaterial);
  Material *getMaterial(std::string name);
  static bool init(std::string materialLib);

private:
  MaterialManager();
  bool loadLib(std::string name);
  bool loadMaterial(tinyxml2::XMLElement *material);
  BaseTexture *loadTexture(tinyxml2::XMLElement *texture);
  tinyxml2::XMLElement *saveTexture(tinyxml2::XMLDocument &xmlDoc, Texture *texture);
  CShader *loadShader(tinyxml2::XMLElement *shader);
  tinyxml2::XMLElement *saveShader(tinyxml2::XMLDocument &xmlDoc, CShader *shader);
};
