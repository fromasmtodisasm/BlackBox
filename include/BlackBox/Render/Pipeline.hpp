#pragma once
#include <glm/glm.hpp>
#include <BlackBox/Render/BaseTexture.hpp>

class Pipeline
{
public:
	static Pipeline* instance();
private:
	Pipeline() = default;
public:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 transform;
	glm::vec3 view_pos;
	BaseTexture* skyBox;

private:
  static Pipeline *m_instance;

};