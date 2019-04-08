#pragma once

#include "Triangle.hpp"
#include "Cube.hpp"
#include "Tetrahedron.hpp"
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
  static Object *create(Type type, std::string vsh = "", std::string fsh = "");
  ~Primitive();
};