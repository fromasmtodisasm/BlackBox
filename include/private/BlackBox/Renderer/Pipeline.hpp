#pragma once
#include <BlackBox/Renderer/BaseTexture.hpp>
#include <BlackBox/Renderer/Shader.hpp>
#include <glm/glm.hpp>

class Object;

class Pipeline
{
  public:
	static Pipeline* instance();
	void bindProgram(ShaderProgramRef program);
	void bindProgram(const char* name);

  private:
	Pipeline() = default;

  public:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 transform;
	glm::vec3 view_pos;
	BaseTexture* skyBox;
	IShaderProgram* shader;
	Object* object;

  private:
	static Pipeline* m_instance;
};