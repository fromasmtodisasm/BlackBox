#include "Triangle.hpp"
#include "VertexBuffer.hpp"
#include <iostream>

using namespace std;

Triangle::Triangle(CShaderProgram *program) :
  m_Shader(program)
{
   m_Vertices = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f,  0.5f, 0.0f
    };

  VertexBuffer *vb = new VertexBuffer(m_Vertices.data(),m_Vertices.size()*sizeof(GLfloat));
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);

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
