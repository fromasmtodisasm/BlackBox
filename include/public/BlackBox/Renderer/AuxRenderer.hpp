#pragma once
#include <BlackBox/Renderer/IRender.hpp>

class IRenderAuxGeom
{
public:
  IRenderAuxGeom();
  void DrawAABB(Vec3 min, Vec3 max);

private:
  CVertexBuffer* m_BoundingBox = nullptr;
  SVertexStream* m_BB_IndexBuffer = nullptr;
};