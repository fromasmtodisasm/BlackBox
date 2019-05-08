#pragma once
#include <memory>

class CShader;
class CShaderProgram;
class Texture;

struct Material
{
  std::shared_ptr<Texture*>
  difuse,
  specular,
  bump,
  normal,
  mask;

  CShaderProgram *program;
};
