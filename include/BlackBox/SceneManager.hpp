#pragma once

class Scene;

#include <map>
#include <string>

class SceneManager
{
  static SceneManager *manager;
  std::map<std::string, Scene*> cache;
  //SceneManager();
public:
  static SceneManager *instance();
  Scene *getScene(std::string Scene);
  //Scene *getPrimitive(Primitive::Type type, CShaderProgram *program);
};

