#pragma once

#include "Triangle.hpp"
#include "Cube.hpp"
#include "Tetrahedron.hpp"

class Primitive
{
public:
  enum Type
  {
    TRIANGLE,
    CUBE,
    TETRAHEDRON
  };
private:
  Primitive(Type type);
public:
  static Object *create(Type type, CShaderProgram *shader);
  ~Primitive();
};