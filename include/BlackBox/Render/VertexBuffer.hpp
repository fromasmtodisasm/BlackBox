#pragma once
#include <BlackBox/Render/Renderer.hpp>

#include <set>

class VertexArrayObject
{
public:
	enum Attribute
	{
		POSITION,
		NORMAL,
		UV, 
		TANGENT,
		BTANGENT
	};

  VertexArrayObject(const void *data, GLint size, GLenum type);
  ~VertexArrayObject();
  void draw();
protected:
	virtual bool init();
protected:
  GLuint VBO;
  GLuint id;
  const void *m_Data;
  GLint m_Count;
  GLenum m_Type;
	std::set<Attribute> m_attributes;
};


