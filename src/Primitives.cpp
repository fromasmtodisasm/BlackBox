#include "Primitives.hpp"

Object * Primitive::create(Type type, std::string vsh, std::string fsh)
{
  Object *obj;
  CShaderProgram *shader = new CShaderProgram("res/" + vsh, "res/" + fsh);
  shader->create();
  switch (type)
  {
  case Primitive::CUBE:
  case Primitive::PLANE:
    if (type == Primitive::CUBE) obj = Object::load("cube.obj");
    if (type == Primitive::PLANE) obj = Object::load("plane.obj");
    obj->setType(OBJType::TPRIMITIVE);
    obj->setShaderProgram(shader);
    obj->getShaderProgram()->use();
    obj->getShaderProgram()->setUniformValue("vColor", glm::vec3(1.0f, 0.0f, 0.0f));
    break;
  case Primitive::TETRAHEDRON:
    break;
  default:
    break;
  }
  return obj;
}
