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
	if (attributes.attributes.empty())
		init();
	else
		m_attributes = attributes;
	//m_attributes = attributes;
  glGenVertexArrays(1, &id);

  glBindVertexArray(id);
    glGenBuffers(1, &VBO);  

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, count*m_attributes.stride, data, GL_STATIC_DRAW);
    // 3. Устанавливаем указатели на вершинные атрибуты
		if (m_attributes.attributes.find(POSITION) != m_attributes.attributes.end())
		{
			glEnableVertexAttribArray(position);
			//																										 stride
			glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, (GLvoid*)m_attributes.attributes[POSITION]);
		}

		if (m_attributes.attributes.find(NORMAL) != m_attributes.attributes.end())
		{
			glEnableVertexAttribArray(normal);
			glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, (GLvoid*)m_attributes.attributes[NORMAL]);
		}

		if (m_attributes.attributes.find(UV) != m_attributes.attributes.end())
		{
			glEnableVertexAttribArray(uv);
			glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, m_attributes.stride, (GLvoid*)m_attributes.attributes[UV]);
		}

		if (m_attributes.attributes.find(TANGENT) != m_attributes.attributes.end())
		{
			glEnableVertexAttribArray(tangent);
			glVertexAttribPointer(tangent, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, (GLvoid*)m_attributes.attributes[TANGENT]);
		}

		if (m_attributes.attributes.find(BTANGENT) != m_attributes.attributes.end())
		{
			glEnableVertexAttribArray(btangent);
			glVertexAttribPointer(btangent, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, (GLvoid*)m_attributes.attributes[BTANGENT]);
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
	m_attributes.stride = sizeof(Vertex);
	m_attributes.attributes[POSITION] = offsetof(Vertex, pos);
	m_attributes.attributes[NORMAL] = offsetof(Vertex, normal);
	m_attributes.attributes[UV] = offsetof(Vertex, uv);
	m_attributes.attributes[TANGENT] = offsetof(Vertex, tangent);
	m_attributes.attributes[BTANGENT] = offsetof(Vertex, btangent);
	return true;
}
