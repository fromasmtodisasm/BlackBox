#include <BlackBox/Quad.hpp>
#include <BlackBox/IGeometry.hpp>

Quad::Quad() : VertexArrayObject(m_vertecies.data(), m_vertecies.size(), GL_TRIANGLES)
{

}

Quad::~Quad()
{
}

bool Quad::init()
{
	m_attributes.insert(POSITION);
	m_attributes.insert(UV);

	m_vertecies.push_back(Vertex({-1.f, -1.f, 0.f}, { 0.f,0.f }));
	m_vertecies.push_back(Vertex({-1.f,  1.f, 0.f}, { 0.f,1.f }));
	m_vertecies.push_back(Vertex({ 1.f,  1.f, 0.f}, { 1.f,1.f }));
	m_vertecies.push_back(Vertex({ 1.f, -1.f, 0.f}, { 1.f,0.f }));
}
