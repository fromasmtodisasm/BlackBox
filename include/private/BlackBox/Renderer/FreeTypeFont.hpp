#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

#include <glm/glm.hpp>
#include <map>

void RegisterColorTable();

class FreeTypeFont : public IFont
{
	friend void RegisterColorTable();
public:
	struct Character
	{
		uint TextureID;		// ID handle of the glyph texture
		glm::ivec2 Pos;		// Position in atlas
		glm::ivec2 Size;	// Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		FT_Pos Advance;		// Offset to advance to next glyph
	};
	float posX = 0, posY = 0;
	float m_Height = 0;

	FreeTypeFont() : face(nullptr),
					 ft(nullptr),
					 shader(nullptr)
	{
		RegisterColorTable();
	}
	FreeTypeFont(const char* font, int w, int h) : face(nullptr),
												   ft(nullptr),
												   shader(nullptr)
	{
		RegisterColorTable();
	}
	void RenderText(const std::string& text, float x, float y, float scale, float color[4]) override;
	float TextWidth(const std::string& text) override;
	float CharWidth(char ch) override;
	bool Init(const char* font, unsigned int w, unsigned int h) override;
	float GetXPos() override;
	float GetYPos() override;
	void SetXPos(float x) override;
	void SetYPos(float y) override;

private:
	FT_Library ft;
	FT_Face face;
	std::map<char, Character> Characters;

	CVertexBuffer* m_VB = nullptr;
	SVertexStream* m_IB = nullptr;
	BaseShaderProgramRef shader;

	std::vector<std::array<SVF_P3F_C4B_T2F, 6>> m_CharBuffer;

	static bool printColorTableRegistered;
};
