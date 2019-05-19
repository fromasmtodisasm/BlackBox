#include <BlackBox/VertexBuffer.hpp>
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/Renderer.hpp>

#include <glm/glm.hpp>
#include <iostream>

using namespace std;

VertexArrayObject::VertexArrayObject(const void *data, GLint count, GLenum type) :
  m_Data(data), m_Count(count), m_Type(type)
{
  glGenVertexArrays(1, &id);

  glBindVertexArray(id);
    glGenBuffers(1, &VBO);  

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(Vertex), data, GL_STATIC_DRAW);
    // 3. Устанавливаем указатели на вершинные атрибуты
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, n));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
  glBindVertexArray(0);
}

VertexArrayObject::~VertexArrayObject()
{
  glDeleteBuffers(1, &VBO);
}

void VertexArrayObject::draw()
{
  glBindVertexArray(id);
  glDrawArrays(m_Type, 0, m_Count);
  glBindVertexArray(0);
}