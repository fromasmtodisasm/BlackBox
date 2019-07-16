#pragma once
#include <BlackBox/Render/VertexBuffer.hpp>

#include <vector>

class Quad : protected VertexArrayObject
{
	static std::vector<Vertex> m_vertecies;
public:
	Quad();
	~Quad();
private:
	virtual bool init();
};