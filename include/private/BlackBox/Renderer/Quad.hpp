#pragma once
#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <vector>

class Quad
{
	CVertexBuffer* m_VertexBuffer = nullptr;
public:
  Quad();
  ~Quad();
  void draw();
private:
  virtual bool init();
};