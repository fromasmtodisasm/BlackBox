#ifndef SCENE_H
#define SCENE_H
#include <tinyxml2.h>
#include <BlackBox/Object.hpp>
#include <BlackBox/Render/Light.hpp>
#include <BlackBox/Quad.hpp>
#include <BlackBox/Render/ScreenShader.hpp>
#include <BlackBox/Render/Technique.hpp>
#include <BlackBox/IPostProcessor.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/IConsole.hpp>

#include <map>
#include <string>

class World;
class CCamera;
class Object;
class FrameBufferObject;
class Scene;
class SkyBox;

extern Scene *defaultScene;

using ObjecstList = std::multimap<std::string, Object*>;
using DirectionLightList = std::map<std::string, DirectionLight*>;
using PointLightList = std::map<std::string, PointLight*>;
using SpotLightList = std::map<std::string, SpotLight*>;

struct ForEachObjectSink
{
  virtual bool OnObjectFound(Object* object) = 0;
};

class Scene
{
  friend class GameGUI;
  friend class CGame;
private:
	FreeTypeFont *m_Font;
  std::string name;
  World *m_World;
  GLint m_RenderedScene;
	Quad m_ScreenQuad;
	SkyBox* skyBox;
	CBaseShaderProgram *m_ScreenShader;
	CShaderProgram *m_TextShader;
	IPostProcessor* postProcessor = nullptr;
  Technique* m_Technique;

  ObjecstList m_Objects;
  DirectionLightList m_DirectionLight;
  PointLightList m_PointLights;
  SpotLightList m_SpotLights;

  CCamera *m_Camera;
  bool lighting;
	bool inverse_visibility = true;
	decltype(m_Objects)::iterator selected_object_it;
private:
  void loadObject(tinyxml2::XMLElement *object);
  void loadMesh(tinyxml2::XMLElement *mesh);
  void loadLight(tinyxml2::XMLElement* light);
  glm::vec3 loadColorAttribute(tinyxml2::XMLElement* element);
	unsigned int quadVAO;
	ICVar* texture_speed = nullptr;

public:
  Scene(std::string name);
	void selectPrevObject();
	void selectNextObject();
	std::map<std::string,Object*>::iterator selectedObject();
	bool selectObject(std::string name);
  void draw(float dt);
  void addObject(std::string name, Object *object);
  Object *getObject(std::string name);
  int numObjects();
  void setCamera(CCamera *camera);
  CCamera *getCamera();
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
  GLint getRenderTarget();
  void setupLights(Object* object);

  Material* shadowMapMat;

  void begin();
  void end();
	void present(int width, int height);

	void setPostProcessor(IPostProcessor* postProcessor);
  void ForEachObject(ForEachObjectSink* callback);
  void setTechnique(Technique* technique);

};

#endif // SCENE_H
