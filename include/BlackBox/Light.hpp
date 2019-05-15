#pragma once
#include <glm/glm.hpp>

struct Light
{
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  
  const char* type;

  Light() = default;
  Light(glm::vec3 position) :
    position(position),
    ambient(0.2f),
    diffuse(0.5f),
    specular(1.0f)
  {

  }

};