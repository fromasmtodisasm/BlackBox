#pragma once

//#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Renderer/Object.hpp>
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