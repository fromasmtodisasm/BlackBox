/*
#include "VertexArrayObject.hpp"
#include <glad/glad.h>

VertexArrayObject::VertexArrayObject()
{

}

VertexArrayObject::VertexArrayObject(std::vector<Vertex> &vb, std::vector<int> &ib)
{

  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(vb.size()), vb.data(), GL_STATIC_DRAW);
    // 3. Устанавливаем указатели на вершинные атрибуты
    glVertexAttribPointer(0, static_cast<int>(vb.size()), GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
  glBindVertexArray(0);
}
*/
