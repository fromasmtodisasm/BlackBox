#pragma once
#include "Renderer.hpp"

class VertexBuffer
{
private:
  GLuint VBO;
  GLuint VAO;
  const void *m_Data;
  size_t m_Size;
public:
  VertexBuffer(const void *data, size_t size);
  ~VertexBuffer();

  void bind();
  void unbind();
  void draw();
};
