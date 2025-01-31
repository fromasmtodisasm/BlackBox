#pragma once
#ifndef VK_RENDERER
	#include <ft2build.h>
	#include FT_FREETYPE_H
	#include <BlackBox/Renderer/IFont.hpp>
	#include <BlackBox/Renderer/IRender.hpp>
	#include <BlackBox/Core/smartptr.hpp>

	#include <glm/glm.hpp>

	#include <map>
	#include <string_view>

void RegisterColorTable();
class CShader;

constexpr static uint atlas_size{512};
constexpr static uint num_glyphs{127};
constexpr static uint symbol_padding = 2;

class FreeTypeFont : public IFont
{
	friend void RegisterColorTable();

public:
	struct Character
	{
		glm::ivec2 Pos;     // Position in atlas
		glm::ivec2 Size;    // Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		FT_Pos     Advance; // Offset to advance to next glyph
	};
	float posX = 0, posY = 0;
	float m_Height = 0;

	FreeTypeFont()
	    : face(nullptr)
	    , ft(nullptr)
	{
		Characters.insert(std::pair<char, Character>(0, {}));
		RegisterColorTable();
		Characters.insert(std::pair<char, Character>(0, {}));
	}
	FreeTypeFont(const char* font, int w, int h)
	    : face(nullptr)
	    , ft(nullptr)
	{
		RegisterColorTable();
	}
	void  RenderText(const std::string_view text, float x, float y, float scale, float color[4]) override;
	float TextWidth(const std::string_view text) override;
	float CharWidth(char ch) override;
	bool  Init(const char* font, unsigned int w, unsigned int h) override;
	float GetXPos() override;
	float GetYPos() override;
	void  SetXPos(float x) override;
	void  SetYPos(float y) override;
	void  Submit() override;
	void  RenderGlyph(uint ch, glm::uvec2& cur_pos, const glm::uvec2& t_size, std::vector<uint8>& image);
	void  UpdateAtlas(const glm::uvec4 region, void* data);

	void  RenderGlyph(uint ch, glm::uvec2& cur_pos, const glm::uvec2& t_size, std::vector<float>& image);

	void  CreateRasterState();
	void  CreateDSState();
	void  CreateBlendState();

	~FreeTypeFont();

	void Release() override;

	FT_Library                      ft;
	FT_Face                         face;
	std::map<char, Character>       Characters;
	uint8*                          m_FaceData;
	size_t                          m_FaceDataSize;

	CVertexBuffer*                  m_VB            = nullptr;
	SVertexStream*                  m_IB            = nullptr;
	ID3DTexture2D*                  m_pTexture      = NULL;

	ID3DTexture2D*                  m_pWightTexture = NULL;
	ID3DTexture2D*                  m_pGreyTexture  = NULL;

	ID3DShaderResourceView*         m_pTextureRV    = NULL;

	ID3DBuffer*                     m_pConstantBuffer;

	static ID3D11SamplerState*      m_Sampler;
	static ID3D11InputLayout*       m_pFontLayout;
	static ID3D11RasterizerState*   m_pRasterizerState;
	static ID3D11DepthStencilState* m_pDSState;
	static ID3D11BlendState*        m_pBlendState;
	static bool                     first_init;

	#if 1
	std::vector<std::array<SVF_P3F_C4B_T2F, 6>> m_CharBuffer;
	#else
	std::vector<std::array<SVF_P3F_T2F, 6>> m_CharBuffer;
	#endif

	static bool printColorTableRegistered;
};
#endif
