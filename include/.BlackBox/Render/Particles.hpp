#pragma once
#include <BlackBox/Renderer/BaseShader.hpp>

#include <memory>

class Particles
{
public:
  Particles(int cnt, float radius, float pos, float minV, float maxV, BaseShaderProgramRef shader);
  ~Particles();

  bool Create();
  void Draw(float dt);
  void initPos();
  void initVel();

private:
  BaseShaderProgramRef shader;
  GLuint positions = 0;
  GLuint velocities = 0;
  int posCnt = 0;
  float radius = 0;
  float pos = 0;
  float minV = 0.f, maxV = 0.f;

  /*
  class Impl;
  std::unique_ptr<Impl> Impl;
  */
};