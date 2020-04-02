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

MeshRef CreatePlane(int vSegment, int hSegments)
{
    std::vector<glm::vec3> plane( (vSegment) * (hSegments) * 6);
	VertexArrayObject* vb = nullptr;

    for (int z = 0, i = 0; z < hSegments; z++)
    {
        for (int x = 0; x < hSegments; x++)
        {
            plane[i] = glm::vec3(x, 0, z) / glm::vec3(vSegment, 1, hSegments); i++;
            plane[i] = glm::vec3(x + 1, 0, z) / glm::vec3(vSegment, 1, hSegments); i++;
            plane[i] = glm::vec3(x, 0, z + 1) / glm::vec3(vSegment, 1, hSegments); i++;

            plane[i] = glm::vec3(x + 1, 0, z) / glm::vec3(vSegment, 1, hSegments); i++;
            plane[i] = glm::vec3(x + 1, 0, z + 1) / glm::vec3(vSegment, 1, hSegments); i++;
            plane[i] = glm::vec3(x, 0, z + 1) / glm::vec3(vSegment, 1, hSegments); i++;
        	
        }
    }

    VertexArrayObject::Attributes attributes;
    attributes.stride = sizeof(glm::vec3);
    attributes.attributes[VertexArrayObject::POSITION] = 0;

	vb = new VertexArrayObject(
        plane.data(), static_cast<unsigned int>(plane.size()), RenderPrimitive::TRIANGLES, attributes
    );
    return  std::make_shared<Mesh>(vb, nullptr);
}
