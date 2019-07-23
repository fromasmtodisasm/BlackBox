#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <BlackBox/IGeometry.hpp>



class ObjLoader
{
private:
  const int NUMBER_OF_VERTEX = 3;
  const int NUMBER_OF_ATTRIBUTES = 3;
  
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector<face> faces;
  std::vector< glm::vec3 > vertex_buffer;
  std::vector< glm::vec2 > uv_buffer;
  std::vector< glm::vec3 > normal_buffer;
  bool has_uv = false;
public:
  bool load(
    const char * path,
    std::vector <Vertex> & vertex_data, 
    std::vector<int>& indexData,
		BoundingBox &bb
  );
private:
  BoundingBox buildVertexData(
    std::vector <Vertex> & vertex_data, 
    std::vector<face> &faces 
  );
  void calcNormal(std::vector<Vertex> &face);
  void calcTangentSpace(std::vector<Vertex> &face);
};
