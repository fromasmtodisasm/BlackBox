#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

class CStatObj;
#if 0
class ObjectManager
{
  static ObjectManager* manager;
  std::map<std::string, std::shared_ptr<std::vector<Mesh>>> cache;
public:
  static ObjectManager* instance();
  IStatObj* LoadObject(std::string Object, std::string type, LoadObjectSink* callback);
  IStatObj* objectFactory(IStatObj* object, std::string type, LoadObjectSink* callback);
};
#endif