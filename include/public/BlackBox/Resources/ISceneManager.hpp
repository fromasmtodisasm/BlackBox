#pragma once

class Scene;
struct IScene;
struct LoadObjectSink;
struct ICVar;

#include <map>
#include <string>

class ISceneManager
{
public:
  static bool init(const char* scene, LoadObjectSink* callback);
  IScene* getScene(std::string scene, LoadObjectSink* callback);
  void removeScene(std::string scene);
  IScene* currentScene();
  void nextScene();
  void prevScene();
  bool exist(std::string scene);
};