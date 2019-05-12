#pragma once

class Object;

#include <map>
#include <string>

class ObjectManager
{
  static ObjectManager *manager;
  std::map<std::string, Object*> cache;
  //ObjectManager();
public:
  static ObjectManager *instance();
  Object *getObject(std::string Object, std::string type="object");
  std::string getPathByPointer(Object *object);
  Object *objectFactory(Object *object, std::string type);
  //Object *getPrimitive(Primitive::Type type, CShaderProgram *program);
};
