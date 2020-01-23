#ifndef VBOINDEXER_HPP
#define VBOINDEXER_HPP
#include <BlackBox/Renderer/IGeometry.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>

void indexVBO(
  std::vector<Vertex>& vb,
  std::vector<GLuint>& ib,
  std::vector<Vertex>& ivb
);

void indexVBO_TBN(
  std::vector<glm::vec3>& in_vertices,
  std::vector<glm::vec2>& in_uvs,
  std::vector<glm::vec3>& in_normals,
  std::vector<glm::vec3>& in_tangents,
  std::vector<glm::vec3>& in_bitangents,

  std::vector<unsigned short>& out_indices,
  std::vector<glm::vec3>& out_vertices,
  std::vector<glm::vec2>& out_uvs,
  std::vector<glm::vec3>& out_normals,
  std::vector<glm::vec3>& out_tangents,
  std::vector<glm::vec3>& out_bitangents
);

#endif