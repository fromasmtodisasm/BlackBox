#pragma once

class Object;
struct Mesh;

#include <map>
#include <string>
#include <memory>
#include <vector>

class ObjectManager
{
  static ObjectManager *manager;
  std::map<std::string, std::shared_ptr<std::vector<Mesh>>> cache;
  //ObjectManager();
public:
  static ObjectManager *instance();
  Object *getObject(std::string Object, std::string type="object");
  std::string getPathByPointer(Object *object);
  Object *objectFactory(Object *object, std::string type);
  //Object *getPrimitive(Primitive::Type type, CShaderProgram *program);
};
