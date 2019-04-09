#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "IGeometry.hpp"

bool loadOBJ(
  const char * path,
  std::vector <Vertex> & out_vertices
);