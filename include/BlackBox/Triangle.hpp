#pragma once
#include <BlackBox/Object.hpp>
#include <BlackBox/Opengl.hpp>
#include <BlackBox/CShader.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>



class Plane : public Object
{
private:
  std::vector<Vertex> m_Vertices;
  std::vector<int> m_Indeces;
public:
  Plane(CShaderProgram *program);
  ~Plane();
 };
