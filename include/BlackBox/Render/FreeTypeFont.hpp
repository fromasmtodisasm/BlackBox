#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <iostream>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/IFont.hpp>
#include <map>
#include <glm/glm.hpp>
#include <Shader.hpp>

class FreeTypeFont : public IFont
{
public:
	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		FT_Pos     Advance;    // Offset to advance to next glyph
	};
	float posX = 0, posY = 0;

	FreeTypeFont():
    EBO(-1),
    VAO(-1),
    VBO(-1),
    face(nullptr),
    ft(nullptr),
    shader(nullptr)
	{

	}
	FreeTypeFont(const char* font, int w, int h):
    EBO(-1),
    VAO(-1),
    VBO(-1),
    face(nullptr),
    ft(nullptr),
    shader(nullptr)
	{
	}
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, float color[4]);
	virtual float TextWidth(const std::string& text) override;
	virtual float CharWidth(char ch) override;

private:
	FT_Library ft;
	FT_Face face;
	std::map<GLchar, Character> Characters;

	GLuint VAO, VBO, EBO;
	BaseShaderProgramRef shader;
	
public:
	// Унаследовано через IFont
	virtual bool Init(const char* font, unsigned int w, unsigned int h) override;


	// Унаследовано через IFont
	virtual float GetXPos() override;

	virtual float GetYPos() override;


	// Унаследовано через IFont
	virtual void SetXPos(float x) override;

	virtual void SetYPos(float y) override;

};