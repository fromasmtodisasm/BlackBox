#include "Primitives.hpp"

Object * Primitive::create(Type type)
{
  Object *obj;
  switch (type)
  {
  case Primitive::TRIANGLE:
    //obj = new Triangle(shader);
    break;
  case Primitive::CUBE:
    obj = Object::load("cube.obj");
    obj->setType(OBJType::TPRIMITIVE);
    break;
  case Primitive::TETRAHEDRON:
    break;
  default:
    break;
  }
  return obj;
}
