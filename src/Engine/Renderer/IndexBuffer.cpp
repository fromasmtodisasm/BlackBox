#if 0
#include <BlackBox/Renderer/IndexBuffer.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <iostream>

using namespace std;

IndexBuffer::IndexBuffer(const void* data, size_t size) :
  m_Data(data), m_Size(size)
{
  glCheck(glGenVertexArrays(1, &VAO));

  glCheck(glBindVertexArray(VAO));
  glCheck(glGenBuffers(1, &VBO));

  glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO));
  glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
  // 3. Устанавливаем указатели на вершинные атрибуты
  glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0));
  glCheck(glEnableVertexAttribArray(0));
  glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // Note that this is allowed, the call to glIndexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glCheck(glBindVertexArray(0));
}

IndexBuffer::~IndexBuffer()
{
  glCheck(glDeleteBuffers(1, &VBO));
}

void IndexBuffer::bind()
{
  glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO));
}

void IndexBuffer::unbind()
{
  glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::draw()
{
  glCheck(glBindVertexArray(VAO));
  glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, m_Data, GL_STATIC_DRAW));
  // 1. Затем установим указатели на вершинные атрибуты
  glCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0));
  // 2. Используем нашу шейдерную программу
  glCheck(glDrawArrays(GL_TRIANGLES, 0, 3));
  glCheck(glBindVertexArray(0));
}
#endif