#include <BlackBox/Triangle.hpp>
#include <BlackBox/VertexBuffer.hpp>
#include <BlackBox/IndexBuffer.hpp>
#include <BlackBox/Opengl.hpp>
#include <iostream>

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

  VertexArrayObject *vb = new VertexArrayObject(m_Vertices.data(),static_cast<GLint>(m_Vertices.size()), GL_TRIANGLES);
  IndexBuffer *ib;// = new IndexBuffer(m_Indeces.data(),m_Indeces.size());
  m_Mesh = std::make_shared<Mesh>(vb, nullptr);
}

Plane::~Plane() 
{
  
}
