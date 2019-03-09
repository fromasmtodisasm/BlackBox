#pragma once
#include "Renderer.hpp"

class IndexBuffer
{
private:
  GLuint m_RendererID;
  size_t m_Count;
public:
  IndexBuffer(const void *data, size_t count);
  ~IndexBuffer();

  void bind();
  void unbinnd();

  inline size_t getCount() { return m_Count; }
};
