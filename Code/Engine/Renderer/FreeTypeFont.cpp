#ifndef VK_RENDERER
	#include "D3D/Shader.hpp"
	#include "FreeTypeFont.hpp"
	#include <BlackBox/Renderer/IRender.hpp>

	#include "BaseRenderer.hpp"

	#include <memory>
	#include <stb_image_write.h>

	#include "D3D/Renderer.h"

	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include <BlackBox\System\ConsoleRegistration.h>
	#include <stb_image_write.h>

bool                     FreeTypeFont::first_init = true;
ID3D11SamplerState*      FreeTypeFont::m_Sampler;
ID3D11InputLayout*       FreeTypeFont::m_pFontLayout;
ID3D11RasterizerState*   FreeTypeFont::m_pRasterizerState;
ID3D11DepthStencilState* FreeTypeFont::m_pDSState;
ID3D11BlendState*        FreeTypeFont::m_pBlendState;

struct ColorTable
{
	ColorB color;
	string name;
} ColorTable[10] =
    {
        ColorB(0x00, 0x00, 0x00), "black",
        ColorB(0xff, 0xff, 0xff), "white",
        ColorB(0x00, 0x00, 0xff), "blue",
        ColorB(0x00, 0xff, 0x00), "green",
        ColorB(0xff, 0x00, 0x00), "red",
        ColorB(0x00, 0xff, 0xff), "cyan",
        ColorB(0xff, 0xff, 0x00), "yellow",
        ColorB(0xff, 0x00, 0xff), "purple",
        ColorB(0xff, 0x80, 0x00), "orange",
        ColorB(0x8f, 0x8f, 0x8f), "grey"};

void PrintColorTable(IConsoleCmdArgs*)
{
	gEnv->pLog->Log("ColorTable codes");
	for (int i = 0; i < 10; i++)
	{
		gEnv->pLog->Log("$%d $$%d=%s", i, i, ColorTable[i].name.data());
	}
}

bool FreeTypeFont::printColorTableRegistered = false;

void FreeTypeFont::RenderGlyph(uint ch, glm::uvec2& cur_pos, const glm::uvec2& t_size, std::vector<float>& image, std::vector<uint8>& _test)
{
	// Load character glyph
	uint c = ch;
	auto h = m_Height;
	if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	{
		CryError("ERROR::FREETYTPE: Failed to load Glyph");
		return;
	}
	auto& c_with = face->glyph->bitmap.width;
	auto& c_rows = face->glyph->bitmap.rows;
	if (t_size.x < (c_with + cur_pos.x))
	{
		cur_pos.x = 0;
		cur_pos.y += (int)h;
		static char name[256];
		sprintf(name, "atlas_%d.png", (int)((cur_pos.y) / m_Height));
		//stbi_write_png(name, atlas_size, (int)m_Height, 1, _test.data() + ((int)m_Height - cur_pos.y)*atlas_size, atlas_size);
	}

	//UCHAR* pTexels = (UCHAR*)mappedTex.pData;
	auto pTexels = image.data();
	int  pos     = 0;
	for (UINT row = 0; row < face->glyph->bitmap.rows; row++)
	{
		UINT rowStart = (row + cur_pos.y) * atlas_size; // * mappedTex.RowPitch;
		for (UINT col = 0; col < face->glyph->bitmap.width; col++, pos++)
		{
			UINT colStart                    = col * 1 + cur_pos.x;
			pTexels[rowStart + colStart + 0] = face->glyph->bitmap.buffer[pos] / 255.0f; // Red
			_test[rowStart + colStart + 0]   = face->glyph->bitmap.buffer[pos];          // Red
		}
	}
	// Set texture options
	// Now store character for later use
	Character character = {
	    cur_pos,
	    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
	    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
	    face->glyph->advance.x};
	Characters.insert(std::pair<char, Character>(ch, character));
	cur_pos.x += character.Size.x + symbol_padding;
}

void FreeTypeFont::RenderText(const std::string_view text, float x, float y, float scale, float color[4])
{
	return;
	auto         render = GetISystem()->GetIRenderer();
	Legacy::Vec4 cur_c(color[0], color[1], color[2], color[3]);
	glm::mat4    projection = glm::ortho(0.0f, (float)render->GetWidth(), (float)render->GetHeight(), 0.0f);

	float        Color[]    = {cur_c.g, cur_c.b, 1.f, cur_c.r};
	cur_c                   = Legacy::Vec4(Color[0], Color[1], Color[2], Color[3]);
	// Iterate through all characters
	const char* c;
	const char* end = text.data() + text.size();
	//y += 10;
	for (c = text.data(); c != end; c++)
	{
		switch (*c)
		{
		case '\n':
		{
			posX = x = 4;
			posY     = y += m_Height;
			continue;
		}
		case '$':
		{
			if ((c + 1) != end)
			{
				if (isdigit(*(++c)))
				{
					int    ColorIndex = *c - '0';
					ColorB newColor   = ColorTable[ColorIndex].color;
					Color[0]          = newColor.g / 255.f;
					Color[1]          = newColor.b / 255.f;
					Color[2]          = 1.f;
					Color[3]          = newColor.r / 255.f;
					//sb->textColor = Legacy::Vec3(Legacy::Vec3(Color[0], Color[1], Color[2]));
					cur_c             = Legacy::Vec4(
	#if 1
					    Color[0], // green
					    Color[1], // blue
					    Color[2], //alpha
					    Color[3]  // red
	#else
	//1.f,0.f,255.f,0.f
	#endif
					);
					continue;
				}
			}
		}

		default:
			break;
		}
		if ((*c >= 0 && *c <= 255 && iscntrl(*c)))
			continue;
		glm::mat4 model(1.0);
		Character ch         = Characters[*c < 32 ? 0 : *c];
		//Character ch = Characters['%'];

		float     w          = ch.Size.x * scale;
		float     h          = ch.Size.y * scale;

		float     xpos       = x + ch.Bearing.x * scale;
		float     ypos       = y + (ch.Size.y - ch.Bearing.y) * scale + m_Height;

		/*
		Coordinates of quad
		A---D 
		|	|
		B---C
		*/

		// Update VBO for each character
		using P3F_T2F        = SVF_P3F_C4B_T2F;
		Legacy::Vec2 uv_pos  = Legacy::Vec2(ch.Pos) / (float)atlas_size;
		Legacy::Vec2 uv_size = Legacy::Vec2(ch.Size) / (float)atlas_size;

		Legacy::Vec4 pA, pB, pC, pD;
		pA = Legacy::Vec4(Legacy::Vec3{xpos, ypos - h, 0}, 1.f);
		pB = Legacy::Vec4(Legacy::Vec3{xpos, ypos, 0}, 1.f);
		pC = Legacy::Vec4(Legacy::Vec3{xpos + w, ypos, 0}, 1.f);
		pD = Legacy::Vec4(Legacy::Vec3{xpos + w, ypos - h, 0}, 1.f);

		Legacy::Vec2 tA{0.f / 160, 14.f / 127},
		    tB{0.f / 160, 23.f / 127},
		    tD{6.f / 160, 14.f / 127},
		    tC{6.f / 160, 23.f / 127};
	#if 1
		tA = {uv_pos.x, uv_pos.y};
		tB = {uv_pos.x, uv_pos.y + uv_size.y};
		tC = {uv_pos.x + uv_size.x, uv_pos.y + uv_size.y};
		tD = {uv_pos.x + uv_size.x, uv_pos.y};
	#endif

		std::array<P3F_T2F, 6> vertices = {
		    P3F_T2F{Legacy::Vec3(projection * pA), UCol((cur_c)), tA},
		    P3F_T2F{Legacy::Vec3(projection * pB), UCol((cur_c)), tB},
		    P3F_T2F{Legacy::Vec3(projection * pC), UCol((cur_c)), tC},

		    P3F_T2F{Legacy::Vec3(projection * pC), UCol((cur_c)), tC},
		    P3F_T2F{Legacy::Vec3(projection * pD), UCol((cur_c)), tD},
		    P3F_T2F{Legacy::Vec3(projection * pA), UCol((cur_c)), tA},
		};
		m_CharBuffer.push_back(vertices);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		posX = x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
}

float FreeTypeFont::TextWidth(const std::string_view text)
{
	const char* c;
	const char* end = text.data() + text.size();
	float       w   = 0.f;
	for (c = text.data(); c != end; c++)
	{
		w += CharWidth(*c);
	}
	return w;
}

float FreeTypeFont::CharWidth(char symbol)
{
	return 1;
	const float     scale = 1.0;
	const Character ch    = Characters[symbol];

	const float     w     = (/*ch.Bearing.x + ch.Size.x + */ (ch.Advance >> 6)) * scale;
	//float h = (ch.Size.y - ch.Bearing.y + ch.Size.y) * scale;
	return w;
}

struct STestSize
{
	int width, height;
	STestSize(const int w, const int h)
	    : width(w)
	    , height(h)
	{
	}
};
bool operator<(const STestSize& a, const STestSize& b)
{
	if (a.width != b.width)
		return (a.width < b.width);
	return (a.height < b.height);
}
bool FreeTypeFont::Init(const char* font, unsigned int w, unsigned int h)
{
	return true;
	m_Height = static_cast<float>(h);
	std::set<STestSize> test;

	_smart_ptr<CShader> shader;
	if (!shader)
	{
		shader = GlobalResources::SpriteShader = (CShader*)gEnv->pRenderer->Sh_Load("sprite.Font", 0, 0);
	}

	if (FT_Init_FreeType(&ft))
	{
		CryError("ERROR::FREETYPE: Could not init FreeType Library");
		return false;
	}

	auto     path = PathUtil::Make(string("fonts/").c_str(), font);

	CCryFile file;
	if (!file.Open(path.c_str(), "rb")) return false;

	auto size = file.GetLength();
	m_FaceData.resize(size);
	file.ReadRaw(&m_FaceData[0], size);
	if (FT_New_Memory_Face(ft, &m_FaceData[0], size, 0, &face))
	{
		CryError("ERROR::FREETYPE: Failed to load font");
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, h);

	glm::uvec2 t_size(atlas_size);
	glm::uvec2 cur_pos(0, 0);
	if (first_init)
	{
		D3D11_SAMPLER_DESC desc;
		ZeroStruct(desc);
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter   = D3D11_FILTER(D3D11_FILTER_ANISOTROPIC | D3D11_FILTER_MIN_MAG_MIP_LINEAR);
		auto hr       = GetDevice()->CreateSamplerState(&desc, &m_Sampler);
		if (FAILED(hr))
		{
			CryError("Error create sampler for font");
			return false;
		}
		{
	#if 0
			//VERTEX_FORMAT_P3F_C4B_T2F
			D3D11_INPUT_ELEMENT_DESC layout[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
				//{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			UINT numElements = sizeof(layout) / sizeof(layout[0]);
			auto hr = GetDevice()->CreateInputLayout(
				layout,
				numElements,
				shader->m_Shaders[IShader::Type::E_VERTEX]->m_Bytecode->GetBufferPointer(),
				shader->m_Shaders[IShader::Type::E_VERTEX]->m_Bytecode->GetBufferSize(),
				&m_pFontLayout);
			if (FAILED(hr))
			{
				CryError("Error create input layout for font");
				return false;
			}
			GlobalResources::VERTEX_FORMAT_P3F_C4B_T2F_Layout = m_pFontLayout;
	#endif
		}
		CreateRasterState();
		CreateDSState();
		CreateBlendState();
		first_init = false;
	}

	D3D10_MAPPED_TEXTURE2D mappedTex;
	mappedTex.RowPitch = atlas_size * sizeof(float);

	//m_pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);
	std::vector<float> image(atlas_size * atlas_size);
	std::vector<uint8> _test(atlas_size * atlas_size);

	uint8              ch = 0;
	RenderGlyph(ch, cur_pos, t_size, image, _test);
	for (ch = 32; ch < num_glyphs; ch++)
	{
		RenderGlyph(ch, cur_pos, t_size, image, _test);
	}

	{
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width            = t_size.x;
			desc.Height           = t_size.y;
			desc.MipLevels        = 1;
			desc.ArraySize        = 1;
			desc.Format           = DXGI_FORMAT_R32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.Usage            = D3D11_USAGE_DEFAULT;
			desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags   = 0; //D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA srd;
			srd.pSysMem     = image.data();
			srd.SysMemPitch = mappedTex.RowPitch;

			GetDevice()->CreateTexture2D(&desc, &srd, &m_pTexture);
		}

		struct Texel
		{
			using Type = Legacy::Vec4;
			//DXGI_FORMAT Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		};

		{
			std::vector<Texel::Type> image(16 * 16);
			D3D11_TEXTURE2D_DESC     desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width            = 16;
			desc.Height           = 16;
			desc.MipLevels        = 1;
			desc.ArraySize        = 1;
			desc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.Usage            = D3D11_USAGE_DEFAULT;
			desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags   = 0; //D3D11_CPU_ACCESS_WRITE;

			D3D11_MAPPED_SUBRESOURCE mappedTex;
			mappedTex.RowPitch = 16 * sizeof(Texel::Type);

			for (auto& texel : image)
			{
				texel = Texel::Type(1.f, 1.f, 1.f, 1.f);
			}

			D3D11_SUBRESOURCE_DATA srd;
			srd.pSysMem     = image.data();
			srd.SysMemPitch = mappedTex.RowPitch;

			GetDevice()->CreateTexture2D(&desc, &srd, &m_pWightTexture);
			{
				// SEND TO SHADER
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				D3D11_TEXTURE2D_DESC            desc;
				m_pWightTexture->GetDesc(&desc);
				srvDesc.Format                    = DXGI_FORMAT_R32G32B32A32_FLOAT;
				srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels       = desc.MipLevels;
				srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;
				auto hr                           = GetDevice()->CreateShaderResourceView(m_pWightTexture, &srvDesc, &GlobalResources::WiteTextureRV);
				if (FAILED(hr))
				{
					CryError("Failed to create texture view");
				}
			}
		}
		{
			std::vector<Texel::Type> image(16 * 16);
			D3D11_TEXTURE2D_DESC     desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width            = 16;
			desc.Height           = 16;
			desc.MipLevels        = 1;
			desc.ArraySize        = 1;
			desc.Format           = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.Usage            = D3D11_USAGE_DEFAULT;
			desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags   = 0; //D3D11_CPU_ACCESS_WRITE;

			D3D10_MAPPED_TEXTURE2D mappedTex;
			mappedTex.RowPitch = 16 * sizeof(Texel::Type);

			for (auto& texel : image)
			{
				texel   = Texel::Type(1.f, 1.f, 1.f, 1.f) * 0.3f;
				texel.a = 1.f;
			}

			D3D11_SUBRESOURCE_DATA srd;
			srd.pSysMem     = image.data();
			srd.SysMemPitch = mappedTex.RowPitch;

			GetDevice()->CreateTexture2D(&desc, &srd, &m_pWightTexture);
			{
				// SEND TO SHADER
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				D3D11_TEXTURE2D_DESC            desc;
				m_pWightTexture->GetDesc(&desc);
				srvDesc.Format                    = DXGI_FORMAT_R32G32B32A32_FLOAT;
				srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels       = desc.MipLevels;
				srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;
				auto hr                           = GetDevice()->CreateShaderResourceView(m_pWightTexture, &srvDesc, &GlobalResources::GreyTextureRV);
				if (FAILED(hr))
				{
					CryError("Failed to create texture view");
				}
			}
		}
		// SEND TO SHADER
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		D3D11_TEXTURE2D_DESC            desc;
		m_pTexture->GetDesc(&desc);
		srvDesc.Format                    = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension             = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels       = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;
		auto hr                           = GetDevice()->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureRV);
		if (FAILED(hr))
		{
			CryError("Failed to create texture view");
		}
		//GetDevice()->PSSetShaderResources(0, 1, &m_pTextureRV);
		{
		}
	}

	{
		static char name[256];
		sprintf(name, "atlas_%d.png", (int)((cur_pos.y) / m_Height));
		//stbi_write_png(name, atlas_size, (int)m_Height, 1, _test.data() + ((int)m_Height - cur_pos.y + (int)m_Height)*atlas_size, atlas_size);

	#if 0
		stbi_write_png("atlas.png", atlas_size, atlas_size, 1, _test.data(), atlas_size);
	#else
	#endif
	}
	//m_pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	uint16 indices[] = {
	    // Note that we start from 0!
	    0, 1, 3, // First Triangle
	    1, 2, 3  // Second Triangle
	};

	#if 1
	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_C4B_T2F, "Font", false);
	#else
	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_T2F, "Font", false);
	#endif
	#if 0
    Legacy::Vec3 vertices[] =
    {
        Legacy::Vec3( 0.0f, 0.5f, 0.5f ),
        Legacy::Vec3( 0.5f, -0.5f, 0.5f ),
        Legacy::Vec3( -0.5f, -0.5f, 0.5f ),
    };
	gEnv->pRenderer->UpdateBuffer(m_VB, vertices, 3, false);
	#endif

	m_IB = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_IB, indices, sizeof(indices));
	static bool UB_created         = false;

	first_init                     = false;

	GlobalResources::FontAtlasRV   = m_pTextureRV;
	GlobalResources::LinearSampler = m_Sampler;
	return true;
}

float FreeTypeFont::GetXPos()
{
	return posX;
}

float FreeTypeFont::GetYPos()
{
	return posY;
}

void FreeTypeFont::SetXPos(float x)
{
	posX = x;
}

void FreeTypeFont::SetYPos(float y)
{
	posY = y;
}

	#if 0
IFont* CreateIFont()
{
	return new FreeTypeFont();
}
	#endif

void RegisterColorTable()
{
	if (!FreeTypeFont::printColorTableRegistered)
	{
		gEnv->pConsole->AddCommand("printcb", PrintColorTable, 0, "Print color font encoding");
	}
}

void FreeTypeFont::Submit()
{
	m_CharBuffer.resize(0);
	return;

	auto vertex_cnt = 6 * m_CharBuffer.size();
	if (!GlobalResources::SpriteShader || !vertex_cnt)
	{
		m_CharBuffer.clear();
		return;
	}
	// Activate corresponding render state
	auto render = GetISystem()->GetIRenderer();
	gEnv->pRenderer->ReleaseBuffer(m_VB);

	#if 1
	m_VB = gEnv->pRenderer->CreateBuffer(vertex_cnt, VERTEX_FORMAT_P3F_C4B_T2F, "Font", false);
	#else
	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_T2F, "Font", false);
	#endif

	// Render glyph texture over quad
	// Update content of VBO memory
	gEnv->pRenderer->UpdateBuffer(m_VB, m_CharBuffer.data(), vertex_cnt, false);

	GlobalResources::SpriteShader->Bind();
	GetDeviceContext()->PSSetSamplers(0, 1, &m_Sampler);
	GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTextureRV);
	#if 0
	GetDeviceContext()->IASetInputLayout(m_pFontLayout);
	#endif
	GetDeviceContext()->RSSetState(m_pRasterizerState);
	GetDeviceContext()->OMSetBlendState(m_pBlendState, 0, 0xffffffff);
	GetDeviceContext()->OMSetDepthStencilState(m_pDSState, 0);

	gEnv->pRenderer->DrawBuffer(m_VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, vertex_cnt);

	m_CharBuffer.resize(0);
}

FreeTypeFont::~FreeTypeFont()
{
	#if 0
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	#endif
	gEnv->pRenderer->ReleaseIndexBuffer(m_IB);
	gEnv->pRenderer->ReleaseBuffer(m_VB);
}

void FreeTypeFont::Release()
{
	delete this;
}
void FreeTypeFont::CreateRasterState()
{
	// Set up rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode              = D3D11_CULL_NONE;
	rasterizerDesc.FillMode              = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias             = false;
	rasterizerDesc.DepthBiasClamp        = 0;
	rasterizerDesc.SlopeScaledDepthBias  = 0;
	rasterizerDesc.DepthClipEnable       = true;
	rasterizerDesc.ScissorEnable         = false;
	rasterizerDesc.MultisampleEnable     = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
	GetDeviceContext()->RSSetState(m_pRasterizerState);
}
void FreeTypeFont::CreateDSState()
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroStruct(desc);
	//desc.BackFace
	desc.DepthEnable    = false;
	desc.StencilEnable  = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	GetDevice()->CreateDepthStencilState(&desc, &m_pDSState);
}
void FreeTypeFont::CreateBlendState()
{
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));

	BlendState.RenderTarget[0].BlendEnable           = TRUE;
	BlendState.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp               = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha         = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha        = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha          = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	GetDevice()->CreateBlendState(&BlendState, &m_pBlendState);
	GlobalResources::FontBlendState = m_pBlendState;
}
#endif
