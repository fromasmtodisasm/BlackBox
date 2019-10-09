#pragma once
#include <glm/glm.hpp>
#include <BlackBox/Render/BaseTexture.hpp>
#include <BlackBox/Render/Shader.hpp>

class Object;

class Pipeline
{
public:
	static Pipeline* instance();
	void bindProgram(std::shared_ptr<CShaderProgram> program);
	void bindProgram(const char *name);
private:
	Pipeline() = default;
public:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 transform;
	glm::vec3 view_pos;
	BaseTexture* skyBox;
	BaseShaderProgramRef shader;
	Object* object;

private:
  static Pipeline *m_instance;

};