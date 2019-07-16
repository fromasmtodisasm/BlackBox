#pragma once
#include <BlackBox/Render/Renderer.hpp>

class VertexArrayObject
{
private:
  GLuint VBO;
  GLuint id;
  const void *m_Data;
  GLint m_Count;
  GLenum m_Type;
public:
  VertexArrayObject(const void *data, GLint size, GLenum type);
  ~VertexArrayObject();

  void draw();
};
