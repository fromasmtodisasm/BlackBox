#ifndef SCENE_H
#define SCENE_H
#include <tinyxml2.h>
#include <BlackBox/Object.hpp>
#include <BlackBox/Render/Light.hpp>
#include <BlackBox/Quad.hpp>
#include <BlackBox/Render/ScreenShader.hpp>
#include <BlackBox/IPostProcessor.hpp>

#include <map>
#include <string>

class World;
class CCamera;
class Object;
class FrameBufferObject;
class Scene;
class SkyBox;

extern Scene *defaultScene;

class Scene
{
  friend class GameGUI;
  friend class CGame;
private:
  std::string name;
  World *world;
  FrameBufferObject* m_RenderedScene;
	Quad m_ScreenQuad;
	SkyBox* skyBox;
	CShaderProgram *m_ScreenShader;
	IPostProcessor* postProcessor = nullptr;
  std::multimap<std::string, Object*> m_Objects;
  std::map<std::string, DirectionLight*> m_DirectionLight;
  std::map<std::string, PointLight*> m_PointLights;
  std::map<std::string, SpotLight*> m_SpotLights;
  CCamera *m_Camera;
  bool lighting;
	bool inverse_visibility = true;
	decltype(m_Objects)::iterator selected_object_it;
private:
  void loadObject(tinyxml2::XMLElement *object);
  void loadMesh(tinyxml2::XMLElement *mesh);
  void loadLight(tinyxml2::XMLElement* light);
  glm::vec3 loadColorAttribute(tinyxml2::XMLElement* element);
  void setupLights(Object* object);
	unsigned int quadVAO;

public:
  Scene(std::string name);
	void selectPrevObject();
	void selectNextObject();
	Object* selectedObject();
  void draw(float dt);
  void addObject(std::string name, Object *object);
  Object *getObject(std::string name);
  void setCamera(CCamera *camera);
  void update(float dt);
  bool save(std::string as ="");
  tinyxml2::XMLElement *saveTransform(tinyxml2::XMLDocument &xmlDoc, Object *object);
  //tinyxml2::XMLElement *saveVec3(tinyxml2::XMLDocument &xmlDoc, glm::vec3 &);
  tinyxml2::XMLElement *saveLight(tinyxml2::XMLDocument &xmlDoc, BaseLight * light);
  tinyxml2::XMLElement* saveVec3(tinyxml2::XMLDocument& xmlDoc, glm::vec3& color, const char* name);;
  tinyxml2::XMLElement* saveFloat(tinyxml2::XMLDocument& xmlDoc, float value, const char* name);;
  tinyxml2::XMLElement *saveMaterial(tinyxml2::XMLDocument &xmlDoc, Object *object);
  Transform loadTransform(tinyxml2::XMLElement &object);
  bool load(std::string name);
  void setRenderTarget(FrameBufferObject *renderedScene);
  FrameBufferObject *getRenderTarget();

  void begin();
  void end();
	void present();

	void setPostProcessor(IPostProcessor* postProcessor);

};

#endif // SCENE_H
