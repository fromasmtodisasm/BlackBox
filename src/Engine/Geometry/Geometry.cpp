#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/VertexBuffer.hpp>

Mesh::Mesh(VertexArrayObject* verts, IndexBuffer* indxs) :
  m_Verts(verts), m_Indexes(indxs)
{
}

VertexArrayObject* Mesh::getVertexBuffer()
{
  return m_Verts;
}

IndexBuffer* Mesh::getIndexBuffer()
{
  return m_Indexes;
}

MeshRef SubdivisionShpereTessellatorSimple::Compute(int numberOfSubdivisions)
{
    MeshRef mesh = std::make_shared<Mesh>();
    VertexArrayObject::Attributes attributes;
    //attributes.attributes[VertexArrayObject::Attribute::POSITION]

    return mesh;
}
