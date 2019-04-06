#pragma once
#include <common.h>
#include <cassert>
#include "VertexArrayObject.hpp"
#include <glm/glm.hpp>

class VertexBuffer;
class IndexBuffer;

typedef glm::vec3 Vertex;

struct Mesh {
protected:
  VertexBuffer *m_Verts;  
  IndexBuffer *m_Indexes;
public:
  Mesh(VertexBuffer *verts, IndexBuffer *indxs);

  VertexBuffer *getVertexBuffer();
  IndexBuffer *getIndexBuffer();
};

