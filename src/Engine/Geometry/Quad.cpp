#include <BlackBox/Quad.hpp>
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>

Quad::Quad() 
{
	float verts[] = {
		// positions   // texCoords
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				 1.0f, -1.0f,  1.0f, 0.0f,
				 1.0f,  1.0f,  1.0f, 1.0f
	};
 glGenVertexArrays(1, &id);

  glBindVertexArray(id);
    glGenBuffers(1, &VBO);  

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), &verts, GL_STATIC_DRAW);
    // 3. Устанавливаем указатели на вершинные атрибуты
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (GLvoid*)(2*sizeof(float)));

			debuger::vertex_array_label(id, "Quad");
  glBindVertexArray(0);
}

Quad::~Quad()
{
	glDeleteBuffers(1, &VBO);
}

void Quad::draw() { 
  glCheck(glBindVertexArray(id));
  glCheck(glDrawArrays(GL_TRIANGLES, 0, 6));
  glCheck(glBindVertexArray(0));
 }

bool Quad::init()
{
	return true;
}

