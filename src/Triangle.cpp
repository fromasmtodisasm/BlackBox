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
