#ifndef SCENE_H
#define SCENE_H
#include <tinyxml2.h>
#include <BlackBox/Object.hpp>
#include <BlackBox/Render/Light.hpp>
#include <BlackBox/Quad.hpp>
#include <BlackBox/Render/ScreenShader.hpp>
#include <BlackBox/Render/ITechnique.hpp>
#include <BlackBox/IPostProcessor.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/Terrain.hpp>
#include <BlackBox/Render/BaseShader.hpp>

#include <map>
#include <string>

class World;
class CCamera;
class Object;
class FrameBufferObject;
class Scene;
class SkyBox;

extern Scene *defaultScene;

//////////////////////////////////////////////////////////////////
using ObjecstList = std::multimap<std::string, Object*>;
using ObjecstListIt = ObjecstList::iterator;

using DirectionLightList = std::map<std::string, DirectionLight*>;
using DirectionLightListIt = DirectionLightList::iterator;

using PointLightList = std::map<std::string, PointLight*>;
using PointLightListIt = PointLightList::iterator;

using SpotLightList = std::map<std::string, SpotLight*>;
using SpotLightListIt = SpotLightList::iterator;

using CameraList = std::map<std::string, CCamera*>;
using CameraListIt = std::map<std::string, CCamera*>::iterator;
//////////////////////////////////////////////////////////////////

struct ForEachObjectSink
{
  virtual bool OnObjectFound(Object* object) = 0;
};

struct ForEachDirectionLightSink
{
  virtual bool OnLightFound(DirectionLight* light) = 0;
};

struct ForEachPointLightSink
{
  virtual bool OnLightFound(PointLight* light) = 0;
};

struct ForEachSpotLightSink
{
  virtual bool OnLightFound(SpotLight* light) = 0;
};

struct PointObject
{
public:
	PointObject();
	~PointObject();

	void draw();

public:
	GLuint VAO;
	size_t point_cnt = 0;
	BaseShaderProgramRef shader;
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
	BaseShaderProgramRef m_ScreenShader;
	CShaderProgram *m_TextShader;
	IPostProcessor* postProcessor = nullptr;
  ITechnique* m_Technique;

  ObjecstList m_Objects;
  DirectionLightList m_DirectionLight;
  PointLightList m_PointLights;
  SpotLightList m_SpotLights;
	PointObject* m_Points;

	Terrain terrain;

  CameraList m_Camera;
  CameraListIt m_CurrentCamera;
  bool lighting;
	bool inverse_visibility = true;
	decltype(m_Objects)::iterator selected_object_it;
private:
  void loadTerrain(tinyxml2::XMLElement *terrain);
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
  size_t numObjects();
  void setCamera(std::string name, CCamera *camera);
  CCamera *getCurrentCamera();
  void update(float dt);
  void saveObject(tinyxml2::XMLDocument& xmlDoc, ObjectManager* objectManager, std::pair<const std::string, Object*>& obj, tinyxml2::XMLNode* pScene);
  bool save(std::string as ="");
  tinyxml2::XMLElement *saveTransform(tinyxml2::XMLDocument &xmlDoc, Transform *transform);
  //tinyxml2::XMLElement *saveVec3(tinyxml2::XMLDocument &xmlDoc, glm::vec3 &);
  tinyxml2::XMLElement *saveLight(tinyxml2::XMLDocument &xmlDoc, BaseLight * light);
  tinyxml2::XMLElement *saveCamera(tinyxml2::XMLDocument &xmlDoc, CCamera * camera);
  tinyxml2::XMLElement* saveVec3(tinyxml2::XMLDocument& xmlDoc, glm::vec3& color, const char* name);;
  tinyxml2::XMLElement* saveFloat(tinyxml2::XMLDocument& xmlDoc, float value, const char* name);;
  tinyxml2::XMLElement *saveMaterial(tinyxml2::XMLDocument &xmlDoc, Object *object);
  Transform loadTransform(tinyxml2::XMLElement &object);
  glm::vec3 loadVec3(tinyxml2::XMLElement &element, const char* name);
  void loadCamera(tinyxml2::XMLElement* element);
  void loadTagPoint(tinyxml2::XMLElement* element);
  bool load(std::string name);
  GLint getRenderTarget();

  Material* shadowMapMat;

  void begin();
  void end();
	void present(int width, int height);
  SkyBox* GetSkyBox();

	void setPostProcessor(IPostProcessor* postProcessor);
  void ForEachObject(ForEachObjectSink* callback);
  void ForEachDirectionLight(ForEachDirectionLightSink* callback);
  void ForEachPointLight(ForEachPointLightSink* callback);
  void ForEachSpotLight(ForEachSpotLightSink* callback);
  void setTechnique(ITechnique* technique);

  const PointLightList &GetPointLights();

	Terrain *getTerrain();

	PointObject* createPointObject(tinyxml2::XMLElement* object);
	PointObject* getPoints();

};

#endif // SCENE_H
