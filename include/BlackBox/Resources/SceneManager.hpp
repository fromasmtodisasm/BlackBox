#pragma once

class Scene;

#include <map>
#include <string>

class SceneManager
{
  friend class GameGUI;
  static SceneManager *manager;
  std::map<std::string, Scene*> cache;
	decltype(cache)::iterator current_scene_it;
  //SceneManager();
public:
  static SceneManager *instance();
  static bool init(const char *scene);
  Scene *getScene(std::string scene);
  Scene *currentScene();
	void nextScene();
	void prevScene();
  bool exist(std::string scene);
};

