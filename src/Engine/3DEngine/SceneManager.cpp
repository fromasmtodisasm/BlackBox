#include <BlackBox/3DEngine/SceneManager.hpp>
#include <BlackBox/3DEngine/StatObject.hpp>
#include <BlackBox/Renderer/ObjectManager.hpp>
#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <iostream>
using	namespace std;

Scene* defaultScene = nullptr;
ICVar* SceneManager::scene_path = nullptr;

#if 0
SceneManager* SceneManager::instance()
{
  if (manager == nullptr)
  {
    manager = new SceneManager();
    manager->current_scene_it = manager->cache.begin();
    scene_path = GetISystem()->GetIConsole()->GetCVar("scenes_path");
  }
  return manager;
}

bool SceneManager::init(const char* scene, LoadObjectSink* callback)
{
  //defaultScene = SceneManager::instance()->getScene(scene == nullptr ? "default" : scene, callback);
  /*SceneManager::instance()->current_scene_it = SceneManager::instance()->cache.find(scene == nullptr ?
    std::string(scene_path->GetString()) + "default" :
    std::string(scene_path->GetString()) + scene
  );*/
  return true;
}
#endif

IScene* SceneManager::getScene(string scene, LoadObjectSink* callback)
{
  std::string prefix = "res/scenes/";
  bool usPrefix = true;
  if (scene.find("/") != scene.npos)
    usPrefix = false;

  Scene* result;
  {
    std::string scenePath;
    if (usPrefix)
      scenePath = prefix + scene;
    else scenePath = scene;
    const auto v = cache.find(scenePath);
    if (v != cache.end())
    {
      result = v->second;
      GetISystem()->GetILog()->Log("[INFO] Scene [%s] already cached\n", scenePath.c_str());
    }
    else {
      result = new Scene(scene);
      if (!result->load(std::string(scenePath + ".xml").c_str(), callback))
      {
        GetISystem()->GetILog()->Log("[ERROR] Error or load scene: %s\n", scenePath.c_str());
        delete result;
        return nullptr;
      }
      else
      {
				GetISystem()->Log("Cached");
        cache[scenePath] = result;
        GetISystem()->GetILog()->Log("[INFO] Scene [%s] loaded\n", scenePath.c_str());
      }
    }
  }
  return result;
}

#if 0
void SceneManager::removeScene(std::string scene)
{
  cache.erase(scene_path->GetString() + scene);
}

IScene* SceneManager::currentScene()
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

bool SceneManager::exist(std::string scene)
{
  return cache.find("res/scenes/" + scene) != cache.end();
}
#endif
