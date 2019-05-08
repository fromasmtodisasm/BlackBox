#include <BlackBox/ObjectManager.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/Primitives.hpp>

#include <iostream>
using	namespace std;

ObjectManager *ObjectManager::manager = nullptr;

ObjectManager *ObjectManager::instance()
{
  if (manager == nullptr)
  {
    manager = new ObjectManager();
  }
  return manager;
}

Object *ObjectManager::getObject(std::string object)
{
  std::string prefix = "res/geom/";
  bool usPrefix = true;
  if (object.find("/") != object.npos)
    usPrefix = false;

  Object *obj;
  {
    std::string oPath;
    if (usPrefix)
      oPath = prefix + object;
    else oPath = object;
    const auto v = cache.find(oPath);
    if (v != cache.end())
    {
      //obj = v->second->clone();
      obj = new Object(*v->second);
    }
    else {
      obj = Object::load(oPath);
      cache[oPath] = obj;
    }
    if (obj == nullptr)
    {
      cout << "Error of load object: " << oPath << endl;
    }
    else
    {
      cout << "Object [" << oPath <<"] loaded" << endl;
    }
  }
  return obj;
}

string ObjectManager::getPathByPointer(Object *object)
{
  for (auto &obj : cache)
  {
    if (obj.second == object)
      return obj.first;
  }

  //TODO: handle missing pointer
  return string("");
}

