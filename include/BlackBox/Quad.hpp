#pragma once
#include <BlackBox/Render/VertexBuffer.hpp>
#include <BlackBox/IGeometry.hpp>

#include <vector>

class Quad
{
	GLuint id;
  GLuint VBO;
	std::vector<float> m_vertecies;
public:
	Quad();
	~Quad();
	void draw();
private:
	virtual bool init();
};