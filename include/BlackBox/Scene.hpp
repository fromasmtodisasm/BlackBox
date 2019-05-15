#ifndef SCENE_H
#define SCENE_H
#include <tinyxml2.h>
#include <BlackBox/Object.hpp>

#include <map>
#include <string>

class World;
class CCamera;
class Object;
struct Light;

class Scene
{
  friend class GameGUI;
private:
  std::string name;
  World *world;
  std::map<std::string, Object*> m_Objects;
  std::map<std::string, Light*> m_Lights;
  CCamera *m_Camera;
  bool lighting;
private:
  void loadObject(tinyxml2::XMLElement *object);
  void loadMesh(tinyxml2::XMLElement *mesh);
  void loadLight(tinyxml2::XMLElement* light);
  glm::vec3 loadColorAttribute(tinyxml2::XMLElement* element);

public:
  Scene(std::string name);
  void draw(float dt);
  void addObject(std::string name, Object *object);
  Object *getObject(std::string name);
  void setCamera(CCamera *camera);
  void update(float dt);
  bool save();
  tinyxml2::XMLElement *saveTransform(tinyxml2::XMLDocument &xmlDoc, Object *object);
  tinyxml2::XMLElement *saveLight(tinyxml2::XMLDocument &xmlDoc, Light *light);
  tinyxml2::XMLElement* saveColorAttribute(tinyxml2::XMLDocument& xmlDoc, glm::vec3& color, const char* name);;
  tinyxml2::XMLElement *saveMaterial(tinyxml2::XMLDocument &xmlDoc, Object *object);
  Transform loadTransform(tinyxml2::XMLElement &object);
  bool load(std::string name);
};

#endif // SCENE_H
