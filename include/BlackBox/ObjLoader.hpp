#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <BlackBox/IGeometry.hpp>

class ObjLoader
{

public:
  static bool load(
    const char * path,
    std::vector <Vertex> & out_vertices
  );
};
