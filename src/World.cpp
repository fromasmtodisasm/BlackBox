#include "World.hpp"


void World::draw() {
  for (const auto &object : m_Objs) {
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
