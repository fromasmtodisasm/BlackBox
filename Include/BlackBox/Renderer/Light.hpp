#pragma once
#include <BlackBox/Core/MathHelper.hpp>

struct DirectionLight;
struct SpotLight;

struct BaseLight
{
  Legacy::Vec3 ambient;
  Legacy::Vec3 diffuse;
  Legacy::Vec3 specular;
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
  Legacy::Vec3 direction;
};

struct PointLight : public BaseLight
{
  Legacy::Vec3 position;

  float constant;
  float linear;
  float quadratic;
  const char* type;
};

struct SpotLight : public PointLight
{
  Legacy::Vec3 direction;
  float cutOff;
  float outerCutOff;
};