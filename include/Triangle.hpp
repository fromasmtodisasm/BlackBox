#pragma once
#include "Object.hpp"
#include <glad/glad.h>
#include "CShader.hpp"
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>



class Triangle : public Object
{
private:
  CShaderProgram *m_Shader;  
  std::vector<float> m_Vertices;
public:
  Triangle(CShaderProgram *program);
  ~Triangle();
  void draw(); 
  void move(); 
  void rotate(); 
};
