#pragma once
#include <BlackBox/IConsole.hpp>

class Scene;
struct LoadObjectSink;

#include <map>
#include <string>

class SceneManager
{
  friend class GameGUI;
  static SceneManager* manager;
  std::map<std::string, Scene*> cache;
  decltype(cache)::iterator current_scene_it;
  static ICVar* scene_path;
  //SceneManager();
public:
  static SceneManager* instance();
  static bool init(const char* scene, LoadObjectSink* callback);
  Scene* getScene(std::string scene, LoadObjectSink* callback);
  void removeScene(std::string scene);
  Scene* currentScene();
  void nextScene();
  void prevScene();
  bool exist(std::string scene);
};