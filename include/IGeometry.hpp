#pragma once
#include <common.h>
#include <cassert>
#include "VertexArrayObject.hpp"

class VertexBuffer;
class IndexBuffer;

struct Vertex
{
  float x,y,z;
};

struct Mesh {
protected:
  VertexBuffer *m_Verts;  
  IndexBuffer *m_Indexes;
public:
  Mesh(VertexBuffer *verts, IndexBuffer *indxs);

  VertexBuffer *getVertexBuffer();
  IndexBuffer *getIndexBuffer();
};

