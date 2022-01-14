#pragma once
#include <BlackBox/Renderer/ISceneManager.hpp>

class Scene;
struct IScene;
struct LoadObjectSink;
struct ICVar;

#include <map>
#include <string>

class SceneManager
{
  std::map<std::string, Scene*> cache;
  decltype(cache)::iterator current_scene_it;
  static ICVar* scene_path;
public:
  IScene* getScene(string scene, LoadObjectSink* callback);
  void removeScene(string scene);
  IScene* currentScene();
  void nextScene();
  void prevScene();
  bool exist(std::string scene);
};