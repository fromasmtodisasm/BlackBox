#include "Triangle.hpp"
#include "VertexBuffer.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;

Triangle::Triangle(CShaderProgram *program) :
  m_Shader(program)
{
  m_Vertices = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
  };  

  glm::mat4 rot = glm::scale(glm::vec3(0.5, 0.5, 1));

  for (int i = 0; i < m_Vertices.size(); i+=3)
  {
    glm::vec4 v = glm::vec4(m_Vertices[i], m_Vertices[i + 1], m_Vertices[i + 2], 0.0);
    v = v * rot;
    m_Vertices[i] = v.x;
    m_Vertices[i + 1] = v.y;
    m_Vertices[i + 2] = v.z;
  }

  VertexBuffer *vb = new VertexBuffer(m_Vertices.data(),m_Vertices.size()*sizeof(GLfloat));
  m_Geom = new Geometry(vb, nullptr);
}

Triangle::~Triangle() 
{
  
}

void Triangle::draw() {
  VertexBuffer *vb = m_Geom->getVertexBuffer();

  m_Shader->use();
  vb->draw();
}

void Triangle::move()
{

}

void Triangle::rotate()
{

}
