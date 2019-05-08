#ifndef SCENE_H
#define SCENE_H

#include <map>

class World;
class CCamera;
class Object;

class Scene
{
  World *world;
  std::map<std::string,Object*> m_Objs;
  CCamera *m_Camera;

public:
  Scene();
  void draw(float dt);
  void addObject(std::string name, Object *object);
};

#endif // SCENE_H
