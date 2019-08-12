#pragma once
#include <BlackBox/Render/Opengl.hpp>
#include <glm/glm.hpp>

struct IFont
{
	virtual bool Init(const char* font, unsigned int w, unsigned int h) = 0;
	virtual void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, float color[4]) = 0;
	virtual float GetXPos() = 0;
	virtual float GetYPos() = 0;
	virtual void SetXPos(float x) = 0;
	virtual void SetYPos(float y) = 0;
};
