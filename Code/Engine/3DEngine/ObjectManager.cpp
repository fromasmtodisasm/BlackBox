#include <BlackBox/Renderer/ObjectManager.hpp>
#include <BlackBox/3DEngine/StatObject.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
//#include <Player.h>

#include <iostream>
using	namespace std;
#if 0
ObjectManager* ObjectManager::manager = nullptr;

ObjectManager* ObjectManager::instance()
{
  if (manager == nullptr)
  {
    manager = new ObjectManager();
  }
  return manager;
}

IStatObj* ObjectManager::LoadObject(std::string object, std::string type, LoadObjectSink* callback)
{
  std::string prefix = "res/geom/";
  bool usPrefix = true;
  if (object.find("/") != object.npos)
    usPrefix = false;

  CStatObj* obj;
  {
    std::string oPath;
    if (usPrefix)
      oPath = prefix + object;
    else oPath = object;
    const auto v = cache.find(oPath);
    if (v != cache.end())
    {
      obj = static_cast<CStatObj*>(objectFactory(new CStatObj(v->second), type, callback));
      obj->type = type;
      GetISystem()->GetILog()->Log("[INFO] Object [%s] already cached\n", oPath.c_str());
    }
    else {
      obj = static_cast<CStatObj*>(objectFactory(CStatObj::load(oPath), type, callback));
      if (obj == nullptr) return nullptr;
      obj->type = type;
      cache[oPath] = obj->m_Mesh;
      GetISystem()->GetILog()->Log("[INFO] Object [%s] loaded\n", oPath.c_str());
    }
    if (obj == nullptr)
    {
      GetISystem()->GetILog()->Log("[ERROR] Error or load object: %s\n", oPath.c_str());
    }
    else
    {
    }
  }
  return obj;
}

IStatObj* ObjectManager::objectFactory(IStatObj* object, string type, LoadObjectSink* callback)
{
  return callback->OnLoad(object, type);
#if 0
  if (type == "player")
    return new CPlayer(object);
  else if (type == "gameobject")
    return new GameObject(object);
  else return object;
#endif
}
#endif