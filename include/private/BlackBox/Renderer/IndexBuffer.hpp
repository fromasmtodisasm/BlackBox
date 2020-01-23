#pragma once
#include <BlackBox/Renderer/Renderer.hpp>

#if 0
class IndexBuffer
{
private:
  GLuint VBO;
  GLuint VAO;
  const void* m_Data;
  std::size_t m_Size;
public:
  IndexBuffer(const void* data, std::size_t size);
  ~IndexBuffer();

  void bind();
  void unbind();
  void draw();
};
#endif