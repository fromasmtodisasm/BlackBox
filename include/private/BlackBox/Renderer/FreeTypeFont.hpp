#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

#include <glm/glm.hpp>
#include <map>

void RegisterColorTable();

constexpr static uint atlas_size{160};
constexpr static uint num_glyphs{127};
constexpr static uint symbol_padding = 2;


class FreeTypeFont : public IFont
{
	friend void RegisterColorTable();
public:
	struct Character
	{
		glm::ivec2 Pos;		// Position in atlas
		glm::ivec2 Size;	// Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		FT_Pos Advance;		// Offset to advance to next glyph
	};
	float posX = 0, posY = 0;
	float m_Height = 0;

	FreeTypeFont() : face(nullptr),
					 ft(nullptr),
					 shader(nullptr),
					 image(std::vector<uint8>(atlas_size * atlas_size))
	{
		RegisterColorTable();
	}
	FreeTypeFont(const char* font, int w, int h) : face(nullptr),
												   ft(nullptr),
												   shader(nullptr),
												   image(std::vector<uint8>(atlas_size * atlas_size))
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
	void Submit() override;
	void RenderGlyph(uint ch, glm::uvec2 & cur_pos, const glm::uvec2 & t_size, std::vector<uint8>& image);
	void UpdateAtlas(const glm::uvec4 region, void* data);

	~FreeTypeFont();

private:
	FT_Library ft;
	FT_Face face;
	std::map<char, Character> Characters;

	CVertexBuffer* m_VB = nullptr;
	SVertexStream* m_IB = nullptr;
	BaseShaderProgramRef shader;
	uint texture;
	glm::uvec2 cur_pos{0, 0};
	std::vector<uint8> image;

	std::vector<std::array<SVF_P3F_C4B_T2F, 6>> m_CharBuffer;

	static bool printColorTableRegistered;
};
