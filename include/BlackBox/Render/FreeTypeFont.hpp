#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H  
#include <iostream>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/IFont.hpp>
#include <map>
#include <glm/glm.hpp>
#include <CShader.hpp>

class FreeTypeFont : public IFont
{
public:
	struct Character {
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};

	FreeTypeFont()
	{

	}
	FreeTypeFont(const char* font, int w, int h)
	{
	}
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
	FT_Library ft;
	FT_Face face;
	std::map<GLchar, Character> Characters;

	GLuint VAO, VBO, EBO;
	CShaderProgram* shader;
	
public:
	// Унаследовано через IFont
	virtual bool Init(const char* font, int w, int h) override;

};