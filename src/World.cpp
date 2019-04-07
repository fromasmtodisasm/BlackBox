#include "World.hpp"


void World::draw() {
  for (const auto &object : m_Objs) {
    object.second->rotate(0.001f, {0,1,0});
    //object.second->getShaderProgram()->use();
    //object.second->getShaderProgram()->setUniformValue("rotate", 0.001f);
    object.second->draw();
  }
  // Camera ...
}

void World::add(string name, Object * o) {
  if (o->getType() == OBJType::TCAM)
    m_Cams[name] = o;
  else if (o->getType() == OBJType::TPRIMITIVE)
    m_Objs[name] = o;
}
