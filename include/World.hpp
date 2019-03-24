#pragma once


#include "Object.hpp"
#include <MatrixTranform.hpp>
#include "VertexBuffer.hpp"
#include <map>
#include <string>

using std::string;
// using std::map;

class World {
private:
  std::map<string,Object*> m_Objs;
  std::map<string, Object*> m_Cams;
public:

  void draw() {
    for (const auto &object : m_Objs) {
      object.second->draw();
    }
    // Camera ...
  }

  void add(string name, Object* o) { 
    if (o->getType() == OBJType::TCAM)
      m_Cams[name] = o;
    else if (o->getType() == OBJType::TPRIMITIVE)
      m_Objs[name] = o;
  }
  void del(string name) { m_Objs.erase(name); }
  void delCam(string name) { m_Cams.erase(name); }

  Object*& operator[] (string name) { return m_Objs[name]; }

};
