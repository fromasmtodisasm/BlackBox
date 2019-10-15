#include <BlackBox/Render/VertexBuffer.hpp>
#include <BlackBox/IGeometry.hpp>
#include <BlackBox/Render/Renderer.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>

#include <glm/glm.hpp>
#include <iostream>

using namespace std;
#pragma warning(push)
#pragma warning(disable : 4312)

VertexArrayObject::VertexArrayObject(const void *data, GLint count, GLenum type, Attributes attributes) :
  m_Data(data), m_Count(count), m_Type(type)
{
  GLint position = 0, uv = 2, normal = 1, tangent = 3, btangent = 4;
	if (attributes.attributes.empty())
		init();
	else
		m_attributes = attributes;
	//m_attributes = attributes;
  glCheck(glGenVertexArrays(1, &id));

  glCheck(glBindVertexArray(id));
    glCheck(glGenBuffers(1, &VBO));  

    glCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    glCheck(glBufferData(GL_ARRAY_BUFFER, count*m_attributes.stride, data, GL_STATIC_DRAW));
    // 3. Устанавливаем указатели на вершинные атрибуты
		if (m_attributes.attributes.find(POSITION) != m_attributes.attributes.end())
		{
			glCheck(glEnableVertexAttribArray(position));
			//																										 stride
			glCheck(glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, reinterpret_cast<GLvoid*>(m_attributes.attributes[POSITION])));
		}

		if (m_attributes.attributes.find(NORMAL) != m_attributes.attributes.end())
		{
			glCheck(glEnableVertexAttribArray(normal));
			glCheck(glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, reinterpret_cast<GLvoid*>(m_attributes.attributes[NORMAL])));
		}

		if (m_attributes.attributes.find(UV) != m_attributes.attributes.end())
		{
			glCheck(glEnableVertexAttribArray(uv));
			glCheck(glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, m_attributes.stride, reinterpret_cast<GLvoid*>(m_attributes.attributes[UV])));
		}

		if (m_attributes.attributes.find(TANGENT) != m_attributes.attributes.end())
		{
			glCheck(glEnableVertexAttribArray(tangent));
			glCheck(glVertexAttribPointer(tangent, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, reinterpret_cast<GLvoid*>(m_attributes.attributes[TANGENT])));
		}

		if (m_attributes.attributes.find(BTANGENT) != m_attributes.attributes.end())
		{
			glCheck(glEnableVertexAttribArray(btangent));
			glCheck(glVertexAttribPointer(btangent, 3, GL_FLOAT, GL_FALSE, m_attributes.stride, reinterpret_cast<GLvoid*>(m_attributes.attributes[BTANGENT])));
		}
  glCheck(glBindVertexArray(0));
}

VertexArrayObject::~VertexArrayObject()
{
  glCheck(glDeleteBuffers(1, &VBO));
}

void VertexArrayObject::draw()
{
  glCheck(glBindVertexArray(id));
  glCheck(glDrawArrays(m_Type, 0, m_Count));
  glCheck(glBindVertexArray(0));
}

void VertexArrayObject::setAttributes(Attributes& attributes)
{
	m_attributes = attributes;
}

VertexArrayObject::Attributes& VertexArrayObject::getAttributes()
{
	// TODO: insert return statement here
	return m_attributes;
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

#pragma warning(pop)
