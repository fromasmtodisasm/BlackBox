#pragma once
#include <BlackBox/CCamera.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/VertexBuffer.hpp>
#include <BlackBox/Scene.hpp>

#include <map>
#include <string>

using std::string;

class World {
  friend class GameGUI;
private:
  std::map<string, Scene*> m_Scenes;
  Scene *activeScene;
  float gravity = 0;
public:
  World();
  void draw(float dt);

  void setCamera(CCamera *camera);
  void addScene(string name, Scene* scene);
  void update(float deltatime);

};
