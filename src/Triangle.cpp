#include "Triangle.hpp"
#include "VertexBuffer.hpp"
#include <iostream>

#include <MatrixTranform.hpp>

using namespace std;

Triangle::Triangle(CShaderProgram *program,float rotAngle) :
  m_Shader(program)
{
  ty = OBJType::TPRIMITIVE;
  m_Vertices = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f,  0.5f, 0.0f
  };  

  // glm::mat4 rot = glm::rotate(glm::mat4(1.0f),rotAngle,glm::vec3(1.0, 0.0, 0.0)); 
  glm::mat4 rot = glm::scale(glm::vec3(1.0f, 2.0f, 1.0f));

  m_Vertices = m_Vertices * rot;
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
