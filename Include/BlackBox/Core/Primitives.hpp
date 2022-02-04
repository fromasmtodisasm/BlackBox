#pragma once

//#include <BlackBox/Renderer/IObjectManager.hpp>
#include <string>

class Primitive
{
public:
  enum Type
  {
    TRIANGLE,
    PLANE,
    CUBE,
    TETRAHEDRON
  };
private:
  Primitive(Type type);
public:
  ~Primitive();
};