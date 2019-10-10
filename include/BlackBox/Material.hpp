#pragma once
#include <memory>
#include <BlackBox/Render/Texture.hpp>
#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/IConsole.hpp>
#include <glm/glm.hpp>
#include <vector>

class CShader;
class CBaseShaderProgram;
class Object;

struct Material
{
  BaseTexture
  /**diffuse,*/
  *specular,
	*emissive,
  *bump,
  *normal,
  *mask;
  float shininess;
	float emissive_factor = 1.0f;
  glm::vec3 diffuseColor;
  float alpha;
	
	std::vector<BaseTexture*> diffuse;

  BaseShaderProgramRef program;
	std::string program_name;
  std::shared_ptr<std::string> name;
  bool hasTexture = false;
  bool enabledNormal = true;
	int current_diffuse = 0;

  Material() : 
  //diffuse(nullptr),
  specular(nullptr),
  bump(nullptr),
  normal(nullptr),
  mask(nullptr),
	emissive(nullptr),
	emissive_factor(1.0f),
  diffuseColor({0,0,0}),
  shininess(64.0f),
  program(nullptr)
  {
  }
  void apply(Object *object);
  void setTexture(Texture *texture, const char *type);
	void nextDiffuse();
	void prevDiffuse();
private:
  void activeTexture(const char *uniform, BaseTexture *texture);
};
