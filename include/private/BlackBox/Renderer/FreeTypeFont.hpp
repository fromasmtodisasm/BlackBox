#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

#include <glm/glm.hpp>
#include <map>
#include <BlackBox/Utils/smartptr.hpp>

void RegisterColorTable();
class CShader;

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
					 ft(nullptr)
	{
		RegisterColorTable();
	}
	FreeTypeFont(const char* font, int w, int h) : face(nullptr),
												   ft(nullptr)
	{
		RegisterColorTable();
	}
	void RenderText(const std::string_view text, float x, float y, float scale, float color[4]) override;
	float TextWidth(const std::string_view text) override;
	float CharWidth(char ch) override;
	bool Init(const char* font, unsigned int w, unsigned int h) override;
	float GetXPos() override;
	float GetYPos() override;
	void SetXPos(float x) override;
	void SetYPos(float y) override;
	void Submit() override;

	void RenderGlyph(uint ch, glm::uvec2& cur_pos, const glm::uvec2& t_size, std::vector<float>& image,  std::vector<uint8>& _test);

	void CreateRasterState();
	void CreateDSState();
	void CreateBlendState();

	~FreeTypeFont();

	void Release() override;
private:
	FT_Library ft;
	FT_Face face;
	std::map<char, Character> Characters;

	CVertexBuffer* m_VB = nullptr;
	SVertexStream* m_IB = nullptr;
	_smart_ptr<CShader> shader;
	ID3D10Texture2D *m_pTexture = NULL;
	ID3D10ShaderResourceView* pTexDepSurface = NULL;

	ID3D10Buffer* m_pConstantBuffer;
	
	static ID3D10SamplerState* m_Sampler;
	static ID3D10InputLayout* m_pFontLayout;
	static ID3D10RasterizerState* m_pRasterizerState;
	static ID3D10DepthStencilState* m_pDSState;
	static ID3D10BlendState* m_pBlendState;
	static bool first_init;

	std::vector<std::array<SVF_P3F_C4B_T2F, 6>> m_CharBuffer;

	static bool printColorTableRegistered;
};
