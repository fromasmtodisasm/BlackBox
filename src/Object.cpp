#include "Object.hpp"
#include "ObjLoader.hpp"
#include "VertexBuffer.hpp"
#include <fstream>
#include <iostream>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <Opengl.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Object::Object() : angle(0.0f), m_Pos(0.0f), m_scale(1.0)
{

}

Object::Object(const Object & obj):
  m_Pos(obj.m_Pos), angle(obj.angle), m_Mesh(obj.m_Mesh), m_Shader(obj.m_Shader),
  m_type(obj.m_type),m_scale(1.0)
{
}



void Object::parse(std::string filename, std::vector<Vertex> &vs, CShaderProgram **shader)
{
 
}

void Object::draw() {
  VertexBuffer *vb = m_Mesh->getVertexBuffer();
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
  scale = glm::scale(scale, m_scale);
  translate = glm::translate(translate, m_Pos);
  rotate = glm::rotate(rotate, angle.x, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
  rotate = glm::rotate(rotate, angle.y, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  rotate = glm::rotate(rotate, angle.z, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
  return translate * rotate * scale;
}

void Object::setShaderProgram(CShaderProgram* shader)
{
  m_Shader = shader;
}

void Object::move(glm::vec3 v) {
  m_Pos = m_Pos + v;
}

void Object::rotate(float angle, glm::vec3 v) {
  if (v.x) this->angle.x += angle;
  if (v.y) this->angle.y += angle;
  if (v.z) this->angle.z += angle;
}

void Object::scale(glm::vec3 v) {m_scale = v;}

Object * Object::load(string path)
{
  Object *obj = nullptr;
  Mesh *mesh;
  VertexBuffer *vb;
  CShaderProgram *shader;
  std::vector<Vertex> p;

  if (!loadOBJ(("res/" + path).c_str(), p))
    return nullptr;
  
  vb = new VertexBuffer(p.data(), static_cast<GLint>(p.size()));
  mesh = new Mesh(vb, nullptr);
  obj = new Object();
  obj->m_Mesh = mesh;
  //obj->m_Shader = shader;
  //obj->m_Shader->create();
	return obj;
}
