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

    //note every face of the cube is on a single line
    std::vector<unsigned int> indices = {
    0,  1,  2,  0,  2,  3,   //front
    4,  5,  6,  4,  6,  7,   //right
    8,  9,  10, 8,  10, 11,  //back
    12, 13, 14, 12, 14, 15,  //left
    16, 17, 18, 16, 18, 19,  //upper
    20, 21, 22, 20, 22, 23 }; //bottom


    return mesh;
}
