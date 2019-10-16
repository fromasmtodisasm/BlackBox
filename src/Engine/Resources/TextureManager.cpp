#include <BlackBox/ISystem.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/Resources/TextureManager.hpp>

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

BaseTexture *TextureManager::getTexture(std::string name, bool isSkyBox)
{
  std::string prefix = "res/images/";
  bool usPrefix = true;
  if (name.find("/") != name.npos)
    usPrefix = false;

  BaseTexture *texture;
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
			texture->load(name.c_str());
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
      GetISystem()->GetILog()->AddLog("[EROR] Error of load texture: %s\n", Path.c_str());
    }
    else
    {
      GetISystem()->GetILog()->AddLog("[INFO] Texture [%s] loaded\n",  Path.c_str());
    }
  }
  return texture;
}
