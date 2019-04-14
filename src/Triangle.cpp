#include "Triangle.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <iostream>

#include <Opengl.hpp>

using namespace std;

Plane::Plane(CShaderProgram *program) :
  Object()
{
  m_Shader = program;
  m_type = OBJType::TPRIMITIVE;
  m_Vertices = {
    {{-1.f,0,  1.f},   {0, 0},{0,1,0 }},
    {{-1.f,0, -1.f},   {0, 0},{0,1,0 }},
    {{1.f, 0, -1.f},   {0, 0},{0,1,0 }},
    {{1.f, 0, -1.f},   {0, 0},{0,1,0 }},
    {{1.f,0,   1.f},   {0, 0},{0,1,0 }},
    {{-1.f,0,  1.f},   {0, 0},{0,1,0 }}

  };

  m_Indeces = {
    0,1,2
  };

  VertexBuffer *vb = new VertexBuffer(m_Vertices.data(),static_cast<GLint>(m_Vertices.size()));
  IndexBuffer *ib;// = new IndexBuffer(m_Indeces.data(),m_Indeces.size());
  m_Mesh = new Mesh(vb, nullptr);
}

Plane::~Plane() 
{
  
}
