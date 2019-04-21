#include <BlackBox/IGeometry.hpp>

Mesh::Mesh(VertexBuffer *verts, IndexBuffer *indxs) :
   m_Verts(verts), m_Indexes(indxs) 
{
  
}

VertexBuffer *Mesh::getVertexBuffer()
{
  return m_Verts;
}

IndexBuffer *Mesh::getIndexBuffer()
{
  return m_Indexes;
}
