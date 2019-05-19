#include <BlackBox/IEngine.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/TextureManager.hpp>

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
      GetIEngine()->getILog()->AddLog("[EROR] Error of load texture: %s\n", Path.c_str());
    }
    else
    {
      GetIEngine()->getILog()->AddLog("[INFO] Texture [%s] loaded\n",  Path.c_str());
    }
  }
  return texture;
}