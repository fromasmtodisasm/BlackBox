#include <BlackBox/VertexBuffer.hpp>
#include <BlackBox/IGeometry.hpp>

#include <glm/glm.hpp>
#include <iostream>

using namespace std;

VertexBuffer::VertexBuffer(const void *data, GLint count) :
  m_Data(data), m_Count(count)
{
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO); 
    glGenBuffers(1, &VBO);  

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(Vertex), data, GL_STATIC_DRAW);
    // 3. Устанавливаем указатели на вершинные атрибуты
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
    glEnableVertexAttribArray(2);
  glBindVertexArray(0);
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &VBO);
}

void VertexBuffer::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
}

void VertexBuffer::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::draw()
{
  glBindVertexArray(VAO); 
  glDrawArrays(GL_TRIANGLES, 0, m_Count);
  glBindVertexArray(0);
}
