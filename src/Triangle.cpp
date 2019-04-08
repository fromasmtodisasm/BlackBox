#include "Triangle.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include <iostream>

#include <GLFW/glfw3.h>
#include <Opengl.hpp>

using namespace std;

Plane::Plane(CShaderProgram *program) :
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

Plane::~Plane() 
{
  
}
