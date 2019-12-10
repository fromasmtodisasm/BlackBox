#pragma once
#include <BlackBox/Object.hpp>
#include <vector>

class Plane : public Object
{
private:
  std::vector<Vertex> m_Vertices;
  std::vector<int> m_Indeces;
public:
  Plane(CBaseShaderProgram* program);
  ~Plane();
};