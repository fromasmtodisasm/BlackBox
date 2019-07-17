#include <BlackBox/Render/VertexBuffer.hpp>
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/Render/Renderer.hpp>

#include <glm/glm.hpp>
#include <iostream>

using namespace std;

VertexArrayObject::VertexArrayObject(const void *data, GLint count, GLenum type, Attributes attributes) :
  m_Data(data), m_Count(count), m_Type(type)
{
  GLint position = 0, uv = 2, normal = 1, tangent = 3, btangent = 4;
	if (attributes.empty())
		init();
	else
		m_attributes = attributes;
	//m_attributes = attributes;
  glGenVertexArrays(1, &id);

  glBindVertexArray(id);
    glGenBuffers(1, &VBO);  

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count*sizeof(Vertex), data, GL_STATIC_DRAW);
    // 3. Устанавливаем указатели на вершинные атрибуты
		if (m_attributes.find(POSITION) != m_attributes.end())
		{
			glEnableVertexAttribArray(position);
			glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
		}

		if (m_attributes.find(NORMAL) != m_attributes.end())
		{
			glEnableVertexAttribArray(normal);
			glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		}

		if (m_attributes.find(UV) != m_attributes.end())
		{
			glEnableVertexAttribArray(uv);
			glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
		}

		if (m_attributes.find(TANGENT) != m_attributes.end())
		{
			glEnableVertexAttribArray(tangent);
			glVertexAttribPointer(tangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));
		}

		if (m_attributes.find(BTANGENT) != m_attributes.end())
		{
			glEnableVertexAttribArray(btangent);
			glVertexAttribPointer(btangent, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, btangent));
		}
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

void VertexArrayObject::setAttributes(Attributes& attributes)
{
	m_attributes = attributes;
}

bool VertexArrayObject::init()
{
	m_attributes.insert(POSITION);
	m_attributes.insert(NORMAL);
	m_attributes.insert(UV);
	m_attributes.insert(TANGENT);
	m_attributes.insert(BTANGENT);
	return true;
}
