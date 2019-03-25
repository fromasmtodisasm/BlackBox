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

  void draw();

  void add(string name, Object* o);
  inline void del(string name) { m_Objs.erase(name); }
  inline void delCam(string name) { m_Cams.erase(name); }

  Object*& operator[] (string name) { return m_Objs[name]; }

};
