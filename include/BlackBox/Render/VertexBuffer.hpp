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
	using Attributes = std::set<Attribute>;

  VertexArrayObject(const void *data, GLint size, GLenum type/*, Attributes attributes*/);
  ~VertexArrayObject();
  void draw();
	void setAttributes(Attributes &attributes);
protected:
	virtual bool init();
protected:
  GLuint VBO;
  GLuint id;
  const void *m_Data;
  GLint m_Count;
  GLenum m_Type;
	Attributes m_attributes;
};


