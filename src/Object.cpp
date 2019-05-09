#include <BlackBox/Object.hpp>
#include <BlackBox/ObjLoader.hpp>
#include <BlackBox/VertexBuffer.hpp>
#include <BlackBox/Renderer.hpp>
#include <BlackBox/Opengl.hpp>

#include <fstream>
#include <iostream>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Object::Object() : m_transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),velocity(glm::vec3(0))
{
}

Object::Object(const Object *obj):
  m_transform(obj->m_transform.position, obj->m_transform.rotation, obj->m_transform.scale),
  m_Mesh(obj->m_Mesh), m_Shader(obj->m_Shader),
  m_type(obj->m_type),velocity(glm::vec3(0)),
  m_path(obj->m_path)
{
}



void Object::parse(std::string filename, std::vector<Vertex> &vs, CShaderProgram **shader)
{
 
}

void Object::draw(CCamera *camera) {
  m_Material->apply(this, camera);
  VertexArrayObject *vb = m_Mesh->getVertexBuffer();
  vb->draw();
}

void Object::setType(OBJType type)
{
  m_type = type;
}

CShaderProgram * Object::getShaderProgram()
{
  return m_Shader;
}

glm::mat4 Object::getTransform()
{
  glm::mat4x4 translate(1.0f), rotate(1.0f), scale(1.0f);
  scale = glm::scale(scale, m_transform.scale);
  translate = glm::translate(translate, m_transform.position);
  rotate = glm::rotate(rotate, m_transform.rotation.x, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
  rotate = glm::rotate(rotate, m_transform.rotation.y, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  rotate = glm::rotate(rotate, m_transform.rotation.z, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
  return translate * rotate * scale;
}

void Object::setShaderProgram(CShaderProgram* shader)
{
  m_Shader = shader;
}

void Object::update(float deltatime)
{
  if (m_transform.position.y < 0)
    velocity.y = - velocity.y*friction;
  m_transform.position += velocity * deltatime;
}

void Object::setTexture(Texture *texture, const char *type)
{
  m_Material->setTexture(texture, type);
}

Object Object::operator=(Object &that)
{
  Object obj;
  obj.m_Mesh = that.m_Mesh;
  obj.m_type = that.m_type;

}

Object *Object::clone()
{
  Object *obj = new Object;
  obj->m_Mesh = this->m_Mesh;
  obj->m_type = this->m_type;
  return obj;
}

Material *Object::getMaterial()
{
  return m_Material;
}

void Object::setMaterial(Material *material)
{
  if (m_Material != nullptr)
      ;//delete m_Material;
  m_Material = material;
}

void Object::move(glm::vec3 v) {
  m_transform.position += v;
}

void Object::moveTo(glm::vec3 v)
{
  m_transform.position = v;
}

void Object::rotate(float angle, glm::vec3 v) {
  m_transform.rotation.x += angle;
  m_transform.rotation.y += angle;
  m_transform.rotation.z += angle;
}

void Object::scale(glm::vec3 v)
{
  m_transform.scale = v;
}

Object * Object::load(string path)
{
  Object *obj = nullptr;
  std::shared_ptr<Mesh> mesh;
  VertexArrayObject *vb;
  std::vector<Vertex> p;

  if (!loadOBJ(path.c_str(), p))
    return nullptr;
  
  vb = new VertexArrayObject(p.data(), static_cast<GLint>(p.size()), GL_TRIANGLES);
  mesh = std::make_shared<Mesh>(vb, nullptr);
  obj = new Object();
  obj->m_Mesh = mesh;
  obj->m_path = std::make_shared<std::string>(path);
	return obj;
}
