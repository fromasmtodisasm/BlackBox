#pragma once

#include <map>
#include <string>
#include <memory>
#include <vector>

class Object;
class Mesh;

struct LoadObjectSink
{
  virtual Object* OnLoad(Object* object, std::string type) = 0;
};

class IObjectManager
{
public:
  Object* getObject(std::string Object, std::string type, LoadObjectSink* callback);
  std::string getPathByPointer(Object* object);
  Object* objectFactory(Object* object, std::string type, LoadObjectSink* callback);
};