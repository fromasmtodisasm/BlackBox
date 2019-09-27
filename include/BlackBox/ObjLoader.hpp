#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <BlackBox/IGeometry.hpp>



class ObjLoader
{
private:
  const unsigned int NUMBER_OF_VERTEX = 3;
  const unsigned int NUMBER_OF_ATTRIBUTES = 3;
  
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector<face> faces;
  std::vector< glm::vec3 > vertex_buffer;
  std::vector< glm::vec2 > uv_buffer;
  std::vector< glm::vec3 > normal_buffer;
  bool has_uv = false;
  bool has_normal = false;
public:
  bool load(
    const char * path,
    VerteciesInfo &verteciesInfo,
		BoundingBox &bb
  );
	bool loadBin(
    const char * path,
    VerteciesInfo &verteciesInfo,
		BoundingBox &bb
  );
private:
  BoundingBox buildVertexData(
    VertexData& vertex_data, 
    std::vector<face> &faces 
  );
  void calcNormal(std::vector<Vertex> &face);
  void calcTangentSpace(std::vector<Vertex> &face);
};
