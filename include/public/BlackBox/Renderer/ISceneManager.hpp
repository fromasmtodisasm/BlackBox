#pragma once

class Scene;
struct IScene;
struct LoadObjectSink;
struct ICVar;

#include <map>
#include <string>

struct ISceneManager
{
  virtual IScene* getScene(std::string_view scene, LoadObjectSink* callback) = 0;
  virtual void removeScene(std::string_view scene) = 0;
  virtual IScene* currentScene() = 0;
  virtual void nextScene() = 0;
  virtual void prevScene() = 0;
  virtual bool exist(std::string scene) = 0;
};