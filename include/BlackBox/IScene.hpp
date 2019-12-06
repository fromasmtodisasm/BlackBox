#pragma once
#include <BlackBox/Render/Light.hpp>

class Object;
struct LoadObjectSink;

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

};
