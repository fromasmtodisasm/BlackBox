#pragma once
#include <BlackBox/3DEngine/IObjectManager.hpp>

#include <map>
#include <string>
#include <memory>
#include <vector>

class CStatObj;

class ObjectManager : public IObjectManager
{
  static ObjectManager* manager;
  std::map<std::string, std::shared_ptr<std::vector<Mesh>>> cache;
  //ObjectManager();
public:
  static ObjectManager* instance();
  IStatObj* getObject(std::string Object, std::string type, LoadObjectSink* callback);
  IStatObj* objectFactory(IStatObj* object, std::string type, LoadObjectSink* callback);
  //Object *getPrimitive(Primitive::Type type, CShaderProgram *program);
};