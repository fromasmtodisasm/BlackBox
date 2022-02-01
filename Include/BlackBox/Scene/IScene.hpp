#pragma once
#include <BlackBox/Renderer/Light.hpp>

#include <map>
#include <string>

class CStatObj;
class SkyBox;
class CCamera;
struct LoadObjectSink;
struct IPostProcessor;
struct ITechnique;

using ObjectMapItr = std::map<std::string, CStatObj*>::iterator;
//////////////////////////////////////////////////////////////////

struct ForEachObjectSink
{
  virtual bool OnObjectFound(CStatObj* object) = 0;
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
  virtual CStatObj* getObject(std::string name) = 0;
  virtual void addObject(std::string name, CStatObj* object) = 0;
  virtual CCamera* getCurrentCamera() = 0;
  virtual size_t numObjects() = 0;
  virtual SkyBox* GetSkyBox() = 0;

};
