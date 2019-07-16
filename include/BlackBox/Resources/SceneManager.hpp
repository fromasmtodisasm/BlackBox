#pragma once

class Scene;

#include <map>
#include <string>

class SceneManager
{
  friend class GameGUI;
  static SceneManager *manager;
  std::map<std::string, Scene*> cache;
  //SceneManager();
public:
  static SceneManager *instance();
  static bool init();
  Scene *getScene(std::string scene);
  //Scene *getPrimitive(Primitive::Type type, CShaderProgram *program);
};

