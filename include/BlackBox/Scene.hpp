#ifndef SCENE_H
#define SCENE_H
#include <tinyxml2.h>
#include <BlackBox/Object.hpp>

#include <map>

class World;
class CCamera;
class Object;

class Scene
{
  friend class GameGUI;
private:
  std::string name;
  World *world;
  std::map<std::string,Object*> m_Objs;
  CCamera *m_Camera;
  bool lighting;
private:
  void loadObject(tinyxml2::XMLElement *object);
  void loadMesh(tinyxml2::XMLElement *mesh);

public:
  Scene(std::string name);
  void draw(float dt);
  void addObject(std::string name, Object *object);
  Object *getObject(std::string name);
  void setCamera(CCamera *camera);
  void update(float dt);
  bool save();
  tinyxml2::XMLElement *saveTransform(tinyxml2::XMLDocument &xmlDoc, Object *object);
  tinyxml2::XMLElement *saveMaterial(tinyxml2::XMLDocument &xmlDoc, Object *object);
  Transform loadTransform(tinyxml2::XMLElement &object);
  bool load(std::string name);
};

#endif // SCENE_H
