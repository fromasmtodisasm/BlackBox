#include <BlackBox/IGeometry.hpp>

Mesh::Mesh(VertexArrayObject *verts, IndexBuffer *indxs) :
   m_Verts(verts), m_Indexes(indxs) 
{
  
}

VertexArrayObject *Mesh::getVertexBuffer()
{
  return m_Verts;
}

IndexBuffer *Mesh::getIndexBuffer()
{
  return m_Indexes;
}
