#pragma once
#include <BlackBox/CCamera.hpp>
#include <BlackBox/Object.hpp>
#include <BlackBox/VertexBuffer.hpp>

#include <map>
#include <string>

using std::string;

class World {
private:
  std::map<string,Object*> m_Objs;
  std::map<string, Object*> m_Cams;
  CCamera *m_Camera;
public:
  World();
  void draw(float dt);

  void setCamera(CCamera *camera);
  void add(string name, Object* o);
  inline void del(string name) { m_Objs.erase(name); }
  inline void delCam(string name) { m_Cams.erase(name); }
  void update(float deltatime);

  Object*& operator[] (string name) { return m_Objs[name]; }

};
