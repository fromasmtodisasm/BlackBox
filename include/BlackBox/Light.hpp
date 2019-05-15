#pragma once
#include <glm/glm.hpp>

struct Light
{
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  float constant;
  float linear;
  float quadratic;

  const char* type;

  //Light() = default;
  Light() :
    position(position),
    ambient(0.2f),
    diffuse(0.5f),
    specular(1.0f),
    constant(1.0f),
    linear(0.022f),
    quadratic(0.0019)
  {

  }

};