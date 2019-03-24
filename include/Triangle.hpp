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
  std::vector<Vertex> m_Vertices;
  std::vector<int> m_Indeces;
public:
  Triangle(CShaderProgram *program);
  ~Triangle();
  virtual void draw() override;
  virtual void move(glm::vec3 v) override;
  virtual void rotate(float angle, glm::vec3 v) override;
  virtual void scale(glm::vec3 v) override;

};
