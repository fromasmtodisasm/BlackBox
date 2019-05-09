#pragma once
#include <BlackBox/Texture.hpp>

#include <map>
#include <string>

extern Texture *defaultTexture;

class TextureManager
{
  static TextureManager *manager;
  std::map<std::string, Texture*> cache;
  //TextureManager();
public:
  static TextureManager *instance();
  Texture *getProgram(std::string vTexture, std::string fTexture);
  static bool init();
  Texture *getTexture(std::string name);
};
