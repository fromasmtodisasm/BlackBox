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
  Scene *activeScene;
public:
  static float gravity;
  World();
  void draw(float dt);

  void setCamera(CCamera *camera);
  void setScene(Scene* scene);
  void update(float deltatime);

};
