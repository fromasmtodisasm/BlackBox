#include <BlackBox/Renderer/Particles.hpp>
#include <BlackBox/Renderer/BaseShader.hpp>
#include <BlackBox/Core/MathHelper.hpp>

#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

inline Vec3 randomVector(int min, int max)
{
  Vec3 vec(0);
  vec.x = static_cast<float>((std::rand() + min) % (int)max);
  vec.y = static_cast<float>((std::rand() + min) % (int)max);
  vec.z = static_cast<float>((std::rand() + min) % (int)max);
  return vec;
}

Particles::Particles(int cnt, float radius, float pos, float minV, float maxV, BaseShaderProgramRef shader)
{
}

Particles::~Particles()
{
}

bool Particles::Create()
{
#pragma warning(push)
#pragma warning(disable : 4244)
  std::srand(time(0));
#pragma warning(pop)
  initPos();
  initVel();

  shader->Use();
  shader->Uniform(randomVector(10, 40), "BlackHolePos1");
  shader->Uniform(randomVector(10, 40), "BlackHolePos2");
  shader->Unuse();
  return true;
}

void Particles::Draw(float dt)
{
  shader->Use();
  shader->Uniform(dt, "DeltaT");

  shader->Unuse();
}

void Particles::initPos()
{
  vector<Vec4> initPos;
  initPos.resize(posCnt);
  for (int i = 0; i < posCnt; i++)
  {
    initPos[i].x = (std::rand() % (int)radius) - radius;
    initPos[i].y = (std::rand() % (int)radius) - radius;
    initPos[i].z = (std::rand() % (int)radius) - radius;
  }
  GLuint buffSize = posCnt * sizeof(glm::vec4);
  glGenBuffers(1, &positions);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, positions);
  glBufferData(GL_SHADER_STORAGE_BUFFER, buffSize, &initPos[0], GL_DYNAMIC_DRAW);
}

void Particles::initVel()
{
  vector<Vec4> initVel;
  initVel.resize(posCnt);
  for (int i = 0; i < posCnt; i++)
  {
    initVel[i].x = (std::rand() % (int)radius) - radius;
    initVel[i].y = (std::rand() % (int)radius) - radius;
    initVel[i].z = (std::rand() % (int)radius) - radius;
  }
  GLuint buffSize = posCnt * sizeof(glm::vec4);
  glGenBuffers(1, &velocities);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, velocities);
  glBufferData(GL_SHADER_STORAGE_BUFFER, buffSize, &initVel[0], GL_DYNAMIC_DRAW);
}