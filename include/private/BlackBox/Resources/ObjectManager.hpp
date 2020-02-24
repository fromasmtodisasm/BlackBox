#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

class Object;
struct Mesh;

struct LoadObjectSink
{
  virtual Object* OnLoad(Object* object, std::string type) = 0;
};

class ObjectManager
{
  static ObjectManager* manager;
  std::map<std::string, std::shared_ptr<std::vector<Mesh>>> cache;
  //ObjectManager();
public:
  static ObjectManager* instance();
  Object* getObject(std::string Object, std::string type, LoadObjectSink* callback);
  std::string getPathByPointer(Object* object);
  Object* objectFactory(Object* object, std::string type, LoadObjectSink* callback);
  //Object *getPrimitive(Primitive::Type type, CShaderProgram *program);
};