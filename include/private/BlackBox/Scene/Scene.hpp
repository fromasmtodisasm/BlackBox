#ifndef SCENE_H
#define SCENE_H

#include <BlackBox/Scene/IScene.hpp>
#include <BlackBox/Renderer/Light.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/Renderer/Terrain.hpp>
#include <BlackBox/Renderer/BaseShader.hpp>

#include <tinyxml2.h>

#include <map>
#include <string>


class CCamera;
class FrameBufferObject;
class Object;
class ObjectManager;
class Scene;
class SkyBox;
class World;
struct IPostProcessor;
struct Material;
struct Transform;

extern Scene* defaultScene;

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

class Scene : public IScene
{
  friend class GameGUI;
  friend class CGame;
private:
  class Serializator
  {
  public:
    Serializator(Scene* scene);
    bool save(const char* as = "");
    bool load(const char* name, LoadObjectSink* callback);

    void loadTerrain(tinyxml2::XMLElement* terrain);
    void loadObject(tinyxml2::XMLElement* object, LoadObjectSink* callback);
    void loadMesh(tinyxml2::XMLElement* mesh);
    void loadLight(tinyxml2::XMLElement* light);
    Transform loadTransform(tinyxml2::XMLElement& object);
    void loadCamera(tinyxml2::XMLElement* element);
    void loadTagPoint(tinyxml2::XMLElement* element);
    
    void saveLights(tinyxml2::XMLDocument& xmlDoc, tinyxml2::XMLNode* pScene);
    void saveObject(tinyxml2::XMLDocument& xmlDoc, ObjectManager* objectManager, std::pair<const std::string, Object*>& obj, tinyxml2::XMLNode* pScene);
    tinyxml2::XMLElement* saveTransform(tinyxml2::XMLDocument& xmlDoc, Transform* transform);
    tinyxml2::XMLElement* saveLight(tinyxml2::XMLDocument& xmlDoc, BaseLight* light);
    tinyxml2::XMLElement* saveCamera(tinyxml2::XMLDocument& xmlDoc, CCamera* camera);
    tinyxml2::XMLElement* saveMaterial(tinyxml2::XMLDocument& xmlDoc, Object* object);
  private:
    Scene *m_Scene = nullptr;
  };

public:
  Scene(std::string name);
  void selectPrevObject();
  void selectNextObject();
  bool selectObject(std::string name);
  void addObject(std::string name, Object* object);
  Object* getObject(std::string name);
  ObjectMapItr selectedObject();
  size_t numObjects();
  void setCamera(std::string name, CCamera* camera);
  CCamera* getCurrentCamera();
  SkyBox* GetSkyBox();
  Terrain* getTerrain();
  GLint getRenderTarget();

  virtual bool save(const char* as = "") override;
  virtual bool load(const char* name, LoadObjectSink* callback) override;

  void begin();
  void update(float dt);
  void end();
  void draw(float dt);
  void present(int width, int height);

  void setPostProcessor(IPostProcessor* postProcessor);
  void ForEachObject(ForEachObjectSink* callback);
  void ForEachDirectionLight(ForEachDirectionLightSink* callback);
  void ForEachPointLight(ForEachPointLightSink* callback);
  void ForEachSpotLight(ForEachSpotLightSink* callback);

  const DirectionLightList& GetDirectionLights();
  const PointLightList& GetPointLights();
  const SpotLightList& GetSpotLights();

  void setTechnique(ITechnique* technique);


  PointObject* createPointObject(tinyxml2::XMLElement* object);
  PointObject* getPoints();
private:
#pragma region Fields
  std::string name;
  GLint m_RenderedScene;
  SkyBox* skyBox;
  BaseShaderProgramRef m_ScreenShader;
  CShaderProgram* m_TextShader;
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
  Material* shadowMapMat;

  unsigned int quadVAO;
  ICVar* texture_speed = nullptr;
#pragma endregion Fields
};

#endif // SCENE_H
