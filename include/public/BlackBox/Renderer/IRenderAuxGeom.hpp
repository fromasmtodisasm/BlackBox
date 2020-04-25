#pragma once

struct IRenderAuxGeom
{
  virtual void DrawAABB(Vec3 min, Vec3 max) = 0;
};