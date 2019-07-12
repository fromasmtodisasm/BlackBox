#pragma once
#include <BlackBox/common.h>
#include <BlackBox/Render/VertexArrayObject.hpp>
#include <cassert>
#include <glm/glm.hpp>

#include <memory>
#include <string>

class VertexArrayObject;
class IndexBuffer;


struct Vertex
{
  glm::vec3 pos;
  glm::vec2 uv;
  glm::vec3 normal;
  glm::vec3 tangent;
  glm::vec3 btangent;

  Vertex() = default;
  Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 n) : pos(pos), uv(uv), normal(n)
  {}
};


struct Mesh {
protected:
  VertexArrayObject *m_Verts;  
  IndexBuffer *m_Indexes;
public:
  std::shared_ptr<std::string> m_Path;
  Mesh(VertexArrayObject *verts, IndexBuffer *indxs);

  VertexArrayObject *getVertexBuffer();
  IndexBuffer *getIndexBuffer();
};

