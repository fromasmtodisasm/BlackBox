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
  Object *obj;
  {
    auto oPath = "res/geom/" + object;
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

