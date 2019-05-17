#include <BlackBox/TextureManager.hpp>

#include <iostream>
using	namespace std;

TextureManager *TextureManager::manager = nullptr;

TextureManager *TextureManager::instance()
{
  if (manager == nullptr)
  {
    manager = new TextureManager();
  }
  return manager;

}

Texture *TextureManager::getTexture(std::string name)
{
  std::string prefix = "res/images/";
  bool usPrefix = true;
  if (name.find("/") != name.npos)
    usPrefix = false;

  Texture *texture;
  {
    std::string Path;
    if (usPrefix)
      Path = prefix + name;
    else Path = name;
    const auto t = cache.find(Path);
    if (t != cache.end())
    {
      texture = t->second;
    }
    else {
      texture = new Texture(name);
      texture->path = std::make_shared<std::string>(Path);
      cache[Path] = texture;
    }
    if (texture == nullptr)
    {
      cout << "Error of load texture: " << Path << endl;
    }
    else
    {
      cout << "Texture [" << Path <<"] loaded" << endl;
    }
  }
  return texture;
}
