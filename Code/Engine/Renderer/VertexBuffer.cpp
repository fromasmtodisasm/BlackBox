#include <BlackBox/Renderer/VertexBuffer.hpp>
#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>

#include <iostream>

using namespace std;
#pragma warning(push)
#pragma warning(disable : 4312)

GLenum toGlPrimitive(RenderPrimitive rp)
{
	switch (rp)
	{
	case RenderPrimitive::LINES: return GL_LINES;
	case RenderPrimitive::LINE_STRIP: return GL_LINE_STRIP;
	case RenderPrimitive::TRIANGLES: return GL_TRIANGLES;
	case RenderPrimitive::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
	default: return -1;
	}
}
/*
VertexArrayObject::VertexArrayObject(const void* data, int count, RenderPrimitive primitive, const Attributes &attributes) :
  m_Data(data), m_Count(count), m_Mode(toGlPrimitive(primitive))
{
	assert(0);
  if (attributes.attributes.empty())
    init();
  else
    m_attributes = attributes;
  glCheck(glGenVertexArrays(1, &id));

  glCheck(glBindVertexArray(id));
  glCheck(glGenBuffers(1, &VBO));

  glCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
  glCheck(glBufferData(GL_ARRAY_BUFFER, count * m_attributes.stride, data, GL_STATIC_DRAW));
  // 3. Устанавливаем указатели на вершинные атрибуты
  setup_attributes();
  glCheck(glBindVertexArray(0));
}

VertexArrayObject::~VertexArrayObject()
{
  glCheck(glDeleteBuffers(1, &VBO));
}

void VertexArrayObject::draw() const
{
  glCheck(glBindVertexArray(id));
  glCheck(glDrawArrays(m_Mode, 0, m_Count));
  glCheck(glBindVertexArray(0));
}

void VertexArrayObject::draw_indexed() const
{
  glCheck(glBindVertexArray(id));
  glCheck(glDrawElements(m_Mode, m_Count, GL_UNSIGNED_INT, nullptr));
  glCheck(glBindVertexArray(0));
}

void VertexArrayObject::setAttributes(Attributes& attributes)
{
  m_attributes = attributes;
}

VertexArrayObject::Attributes& VertexArrayObject::getAttributes()
{
  // TODO: insert return statement here
  return m_attributes;
}

bool VertexArrayObject::init()
{
  m_attributes.stride = sizeof(Vertex);
  m_attributes.attributes[POSITION] = offsetof(Vertex, pos);
  m_attributes.attributes[NORMAL] = offsetof(Vertex, normal);
  m_attributes.attributes[UV] = offsetof(Vertex, uv);
  m_attributes.attributes[TANGENT] = offsetof(Vertex, tangent);
  m_attributes.attributes[BTANGENT] = offsetof(Vertex, btangent);
  return true;
}

void VertexArrayObject::setup_attributes()
{
  GLint position = 0, uv = 2, normal = 1, tangent = 3, btangent = 4;
  if (m_attributes.attributes.find(POSITION) != m_attributes.attributes.end())
  {
    gl::EnableVertexAttribArray(position);
    //																										 stride
    gl::VertexAttribPointer(
        position, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, 
        reinterpret_cast<GLvoid*>(m_attributes.attributes[POSITION])
        );
  }

  if (m_attributes.attributes.find(NORMAL) != m_attributes.attributes.end())
  {
    gl::EnableVertexAttribArray(normal);
    gl::VertexAttribPointer(
        normal, 3, GL_FLOAT, GL_FALSE, m_attributes.stride,
        reinterpret_cast<GLvoid*>(m_attributes.attributes[NORMAL])
        );
  }

  if (m_attributes.attributes.find(UV) != m_attributes.attributes.end())
  {
    gl::EnableVertexAttribArray(uv);
    gl::VertexAttribPointer(
        uv, 2, GL_FLOAT, GL_FALSE, m_attributes.stride,
        reinterpret_cast<GLvoid*>(m_attributes.attributes[UV])
        );
  }

  if (m_attributes.attributes.find(TANGENT) != m_attributes.attributes.end())
  {
    gl::EnableVertexAttribArray(tangent);
    gl::VertexAttribPointer(
        tangent, 3, GL_FLOAT, GL_FALSE, m_attributes.stride,
        reinterpret_cast<GLvoid*>(m_attributes.attributes[TANGENT])
        );
  }

  if (m_attributes.attributes.find(BTANGENT) != m_attributes.attributes.end())
  {
    gl::EnableVertexAttribArray(btangent);
    gl::VertexAttribPointer(
        btangent, 3, GL_FLOAT, GL_FALSE, m_attributes.stride,
        reinterpret_cast<GLvoid*>(m_attributes.attributes[BTANGENT])
        );
  }
}
*/

#pragma warning(pop)

