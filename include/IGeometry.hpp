#pragma once
#include <common.h>

class VertexBuffer;
class IndexBuffer;

struct Geometry {
protected:
  VertexBuffer *m_Verts;  
  IndexBuffer *m_Indexes;
public:
  Geometry(VertexBuffer *verts, IndexBuffer *indxs);

  VertexBuffer *getVertexBuffer();
  IndexBuffer *getIndexBuffer();
};

