#pragma once
#include <BlackBox/Renderer.hpp>

class VertexBuffer
{
private:
  GLuint VBO;
  GLuint VAO;
  const void *m_Data;
  GLint m_Count;
public:
  VertexBuffer(const void *data, GLint size);
  ~VertexBuffer();

  void bind();
  void unbind();
  void draw();
};
