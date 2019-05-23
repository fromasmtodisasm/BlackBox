#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <BlackBox/IGeometry.hpp>

struct indexed_vertex
{
  unsigned int v;
  unsigned int vt;
  unsigned int n;
};

struct face
{
  indexed_vertex v[3];
};


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
    std::vector<int>& indexData
  );
private:
  bool buildVertexData(
    std::vector <Vertex> & vertex_data, 
    std::vector<face> &faces 
  );
  void calcNormal(std::vector<Vertex> &face);
  void calcTangentSpace(std::vector<Vertex> &face);
};
