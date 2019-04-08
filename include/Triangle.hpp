#pragma once
#include "Object.hpp"
#include <Opengl.hpp>
#include "CShader.hpp"
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>



class Plane : public Object
{
private:
  CShaderProgram *m_Shader;  
  std::vector<Vertex> m_Vertices;
  std::vector<int> m_Indeces;
public:
  Plane(CShaderProgram *program);
  ~Plane();
 };
