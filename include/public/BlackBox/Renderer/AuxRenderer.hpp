#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>

class CRenderAuxGeom : public IRenderAuxGeom
{
public:
  CRenderAuxGeom();
  ~CRenderAuxGeom();
  void DrawAABB(Vec3 min, Vec3 max) override;

private:
  CVertexBuffer* m_BoundingBox = nullptr;
  SVertexStream* m_BB_IndexBuffer = nullptr;
};