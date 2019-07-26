#include <BlackBox/IEngine.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Scene.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Object.hpp>

#include <iostream>
using	namespace std;

SceneManager *SceneManager::manager = nullptr;
Scene *defaultScene = nullptr;

SceneManager *SceneManager::instance()
{
  if (manager == nullptr)
  {
    manager = new SceneManager();
		manager->current_scene_it = manager->cache.begin();
  }
  return manager;
}

bool SceneManager::init()
{
  defaultScene = SceneManager::instance()->getScene("default");
  return true;
}

Scene *SceneManager::getScene(string scene)
{
  std::string prefix = "res/scenes/";
  bool usPrefix = true;
  if (scene.find("/") != scene.npos)
    usPrefix = false;

  Scene *result;
  {
    std::string scenePath;
    if (usPrefix)
      scenePath = prefix + scene;
    else scenePath = scene;
    const auto v = cache.find(scenePath);
    if (v != cache.end())
    {
      result = v->second;
      GetIEngine()->getILog()->AddLog("[INFO] Scene [%s] already cached\n", scenePath.c_str());
    }
    else {
      result = new Scene(scene);
      if (!result->load(scenePath + ".xml"))
      {
        GetIEngine()->getILog()->AddLog("[ERROR] Error or load scene: %s\n",scenePath.c_str());
        delete result;
        return nullptr;
      }
      else
      {
        cache[scenePath] = result;
        GetIEngine()->getILog()->AddLog("[INFO] Scene [%s] loaded\n", scenePath.c_str());
      }
    }
  }
  return result;
}

Scene* SceneManager::currentScene()
{
	return current_scene_it->second;
}

void SceneManager::nextScene()
{
	if (++current_scene_it == cache.end())
	{
		current_scene_it = cache.begin();
	}
}

void SceneManager::prevScene()
{
	if (--current_scene_it == cache.begin())
	{
		current_scene_it = cache.end();
	}
}
