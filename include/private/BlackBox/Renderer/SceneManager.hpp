#pragma once
#include <BlackBox/Renderer/ISceneManager.hpp>

class Scene;
struct IScene;
struct LoadObjectSink;
struct ICVar;

#include <map>
#include <string>

class SceneManager : public ISceneManager
{
  friend class GameGUI;
  static SceneManager* manager;
  std::map<std::string, Scene*> cache;
  decltype(cache)::iterator current_scene_it;
  static ICVar* scene_path;
public:
  static SceneManager* instance();
  static bool init(const char* scene, LoadObjectSink* callback);
  IScene* getScene(std::string scene, LoadObjectSink* callback);
  void removeScene(std::string scene);
  IScene* currentScene();
  void nextScene();
  void prevScene();
  bool exist(std::string scene);
};