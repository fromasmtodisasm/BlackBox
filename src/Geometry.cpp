#include "IGeometry.hpp"

Geometry::Geometry(VertexBuffer *verts, IndexBuffer *indxs) :
   m_Verts(verts), m_Indexes(indxs) 
{
  
}

VertexBuffer *Geometry::getVertexBuffer()
{
  return m_Verts;
}

IndexBuffer *Geometry::getIndexBuffer()
{
  return m_Indexes;
}
