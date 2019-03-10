#pragma once
#include "Renderer.hpp"

class IndexBuffer
{
private:
  GLuint VBO;
  GLuint VAO;
  const void *m_Data;
  size_t m_Size;
public:
  IndexBuffer(const void *data, size_t size);
  ~IndexBuffer();

  void bind();
  void unbind();
  void draw();
};
