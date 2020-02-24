#pragma once
#include <BlackBox/Core/MathHelper.hpp>

struct DirectionLight;
struct SpotLight;

struct BaseLight
{
  Vec3 ambient;
  Vec3 diffuse;
  Vec3 specular;
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
  Vec3 direction;
};

struct PointLight : public BaseLight
{
  Vec3 position;

  float constant;
  float linear;
  float quadratic;
  const char* type;
};

struct SpotLight : public PointLight
{
  Vec3 direction;
  float cutOff;
  float outerCutOff;
};