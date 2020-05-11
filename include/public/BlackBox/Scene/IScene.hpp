#pragma once
#include <BlackBox/Renderer/Light.hpp>

#include <map>
#include <string>

class Object;
class SkyBox;
class CCamera;
struct LoadObjectSink;
struct IPostProcessor;
struct ITechnique;

using ObjectMapItr = std::map<std::string, Object*>::iterator;
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


struct IScene
{
  virtual bool save(const char* as = "") = 0;
  virtual bool load(const char* name, LoadObjectSink* callback) = 0;
  virtual Object* getObject(std::string name) = 0;
  virtual void addObject(std::string name, Object* object) = 0;
  virtual CCamera* getCurrentCamera() = 0;
  virtual void present(int width, int height) = 0;
  virtual size_t numObjects() = 0;
  virtual SkyBox* GetSkyBox() = 0;

};
