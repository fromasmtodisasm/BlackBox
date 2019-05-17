#include <BlackBox/Primitives.hpp>

Object * Primitive::create(Type type, CShaderProgram *program)
{
  Object *obj = nullptr;
  ObjectManager *manager = ObjectManager::instance();
  program->create();
  switch (type)
  {
  case Primitive::CUBE:
  case Primitive::PLANE:
    if (type == Primitive::CUBE) obj = manager->getObject("cube.obj");
    if (type == Primitive::PLANE) obj = manager->getObject("plane.obj");
    obj->setType(OBJType::TPRIMITIVE);
    obj->setShaderProgram(program);
    obj->getShaderProgram()->use();
    obj->getShaderProgram()->setUniformValue(glm::vec3(1.0f, 0.0f, 0.0f), "vColor");
    break;
  case Primitive::TETRAHEDRON:
    break;
  default:
    break;
  }
  return obj;
}
