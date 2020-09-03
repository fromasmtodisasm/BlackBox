#include <BlackBox/Renderer/TextureManager.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

using	namespace std;

TextureManager* TextureManager::manager = nullptr;

TextureManager* TextureManager::instance()
{
  if (manager == nullptr)
  {
    manager = new TextureManager();
  }
  return manager;
}

BaseTexture* TextureManager::getTexture(std::string name, bool isSkyBox)
{
  std::string prefix = "res/images/";
  bool usPrefix = true;
  //FIXME: !!!
  #if 0
  if (name.find("/") != name.npos)
    usPrefix = false;
  #endif

  BaseTexture* texture;
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
      if (isSkyBox)
        texture = new TextureCube();
      else
        texture = new Texture();
      texture->path = std::make_shared<std::string>(Path);
      texture->load(Path.data());
      if (t != cache.end())
      {
        cache[Path + "alphaDist"] = texture;
      }
      else
      {
        cache[Path] = texture;
      }
    }
    if (texture == nullptr)
    {
      GetISystem()->GetILog()->LogError("Error of load texture: %s", Path.c_str());
    }
    else
    {
      GetISystem()->GetILog()->Log("Texture [%s] loaded", Path.c_str());
    }
  }
  return texture;
}