#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/VertexBuffer.hpp>

Mesh::Mesh(CVertexBuffer* vertexBuffer) :
  m_Verts(vertexBuffer)
{
}

CVertexBuffer* Mesh::getVertexBuffer()
{
  return m_Verts;
}

