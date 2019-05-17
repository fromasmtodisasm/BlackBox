#include <BlackBox/IndexBuffer.hpp>
#include <iostream>

using namespace std;

IndexBuffer::IndexBuffer(const void *data, size_t size) :
  m_Data(data), m_Size(size)
{
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO); 
  glGenBuffers(1, &VBO);  

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  // 3. Устанавливаем указатели на вершинные атрибуты
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glIndexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray(0);
}

IndexBuffer::~IndexBuffer()
{
  glDeleteBuffers(1, &VBO);
}

void IndexBuffer::bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
}

void IndexBuffer::unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::draw()
{

  glBindVertexArray(VAO); 
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Size, m_Data, GL_STATIC_DRAW);
  // 1. Затем установим указатели на вершинные атрибуты
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  // 2. Используем нашу шейдерную программу
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
