#pragma once
#include <BlackBox/Renderer/Texture.hpp>
#include <BlackBox/Renderer/TextureCube.hpp>

#include <map>
#include <string>

extern Texture* defaultTexture;

class TextureManager
{
  static TextureManager* manager;
  std::map<std::string, BaseTexture*> cache;
  //TextureManager();
public:
  static TextureManager* instance();
  Texture* getProgram(std::string vTexture, std::string fTexture);
  static bool init();
  BaseTexture* getTexture(std::string name, bool isSkyBox);
};