#pragma once
#include <BlackBox/Render/Opengl.hpp>
#include <glm/glm.hpp>

struct IFont
{
	virtual bool Init(const char* font, int w, int h) = 0;
	virtual void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) = 0;
};

//"arial.ttf", 16, 18