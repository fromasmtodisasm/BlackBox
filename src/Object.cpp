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

Object::Object() : m_transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f))
{
}

Object::Object(const Object & obj):
  m_transform(obj.m_transform.position, obj.m_transform.rotation, obj.m_transform.scale),
  m_Mesh(obj.m_Mesh), m_Shader(obj.m_Shader),
  m_type(obj.m_type)
{
}



void Object::parse(std::string filename, std::vector<Vertex> &vs, CShaderProgram **shader)
{
 
}

void Object::draw() {
  if (m_texture != nullptr)
  {
    int id = m_texture->id;
    glBindTexture(GL_TEXTURE_2D, m_texture->id);
  }
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

}

void Object::setTexture(Texture *texture)
{
  m_texture = texture;
}

void Object::move(glm::vec3 v) {
  m_transform.position += v;
}

void Object::moveTo(glm::vec3 v)
{
  m_transform.position = v;
}

void Object::rotate(float angle, glm::vec3 v) {
  m_transform.rotation.x = angle;
  m_transform.rotation.y = angle;
  m_transform.rotation.z = angle;
}

void Object::scale(glm::vec3 v)
{
  m_transform.scale = v;
}

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
