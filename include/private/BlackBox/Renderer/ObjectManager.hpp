#pragma once
#include <BlackBox/Renderer/IGeometry.hpp>

#include <map>
#include <string>
#include <memory>
#include <vector>

struct IStatObj;
struct LoadObjectSink;
class ObjectManager
{
  std::map<std::string, std::shared_ptr<std::vector<Mesh>>> cache;
public:
  static ObjectManager* instance();
  IStatObj* LoadObject(std::string Object, std::string type, LoadObjectSink* callback);
};