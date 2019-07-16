#pragma once
#include <glm/glm.hpp>

struct DirectionLight;
struct SpotLight;

struct BaseLight
{
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  bool enabled;
  enum Type
  {
    DIRECTIONAL,
    POINT,
    SPOT
  }type;
  const char* toStr;
};

struct DirectionLight : BaseLight
{
  glm::vec3 direction;
};

struct PointLight : public BaseLight
{
  glm::vec3 position;

  float constant;
  float linear;
  float quadratic;
  const char* type;

};

struct SpotLight : public PointLight 
{
  glm::vec3 direction;
  float cutOff;
  float outerCutOff;
};