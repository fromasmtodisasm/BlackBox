#pragma once
#include <BlackBox/Renderer/VertexBuffer.hpp>
#include <BlackBox/Renderer/IGeometry.hpp>

#include <vector>

class Quad
{
  uint id;
  uint VBO;
  std::vector<float> m_vertecies;
public:
  Quad();
  ~Quad();
  void draw();
private:
  virtual bool init();
};