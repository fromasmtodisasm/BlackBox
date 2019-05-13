#pragma once
#include <BlackBox/common.h>
#include <BlackBox/VertexArrayObject.hpp>
#include <cassert>
#include <glm/glm.hpp>
#include <memory>

class VertexArrayObject;
class IndexBuffer;

//typedef glm::vec3 Vertex;
struct Vertex
{
  glm::vec3 pos;
  glm::vec2 uv;
  glm::vec3 n;
  Vertex() = default;
  Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 n) : pos(pos), uv(uv), n(n)
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

