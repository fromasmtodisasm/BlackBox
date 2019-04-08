#include "Primitives.hpp"

Object * Primitive::create(Type type, std::string vsh, std::string fsh)
{
  Object *obj;
  switch (type)
  {
  case Primitive::PLANE: {
    CShaderProgram *shader = new CShaderProgram("res\\" + vsh, "res\\" + fsh);
    shader->create();
    obj = new Plane(shader);
    obj->setShaderProgram(shader);
    break;
  }
  case Primitive::CUBE:
    obj = Object::load("cube.obj");
    obj->setType(OBJType::TPRIMITIVE);
    obj->getShaderProgram()->use();
    obj->getShaderProgram()->setUniformValue("color", {1.0f, 0.0f,0.0f});
    break;
  case Primitive::TETRAHEDRON:
    break;
  default:
    break;
  }
  return obj;
}
