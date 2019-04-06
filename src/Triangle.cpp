#include "Triangle.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <iostream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

using namespace std;

Triangle::Triangle(CShaderProgram *program) :
  Object(), m_Shader(program)
{
  m_type = OBJType::TPRIMITIVE;
  m_Vertices = {
    {-0.5f, -0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    {0.5f, -0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {0.5f,  0.5f, -0.5f},
    /*
    {-0.5f, -0.5f, 0.0f},
    {0.5f/2,  0.5f/2, 0.0f},
    {0.5f, -0.5f, 0.0f}
    */
  };

  m_Indeces = {
    0,1,2
  };

  VertexBuffer *vb = new VertexBuffer(m_Vertices.data(),static_cast<GLint>(m_Vertices.size()));
  IndexBuffer *ib;// = new IndexBuffer(m_Indeces.data(),m_Indeces.size());
  m_Mesh = new Mesh(vb, nullptr);
}

Triangle::~Triangle() 
{
  
}

void Triangle::draw() {
  VertexBuffer *vb = m_Mesh->getVertexBuffer();
  glm::mat4x4 rotate(1.0f);
  m_Shader->use();
  float time = static_cast<float>(glfwGetTime()/4);
  //rotate = glm::rotate(rotate, time, glm::vec3(0.0f, 1.0f, 0.0f));
  rotate = glm::rotate(rotate, angle.x, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
  rotate = glm::rotate(rotate, angle.y, glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  rotate = glm::rotate(rotate, angle.z, glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
  m_Shader->setUniformValue("rotate", rotate);
  vb->draw();
}

void Triangle::move(glm::vec3 v)
{

}

void Triangle::rotate(float angle, glm::vec3 v)
{
  if (v.x) this->angle.x+=angle;
  if (v.y) this->angle.y+=angle;
  if (v.z) this->angle.z+=angle;
}

void Triangle::scale(glm::vec3 v)
{

}

