#ifndef VK_RENDERER
#include "D3D/Shader.hpp"
#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>

#include <BlackBox/Renderer/MaterialManager.hpp>

#include "BaseRenderer.hpp"
//#include <BlackBox/Renderer/OpenGL/Core.hpp>

#include <memory>
#include <stb_image_write.h>

#include "D3D/Renderer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <BlackBox\System\ConsoleRegistration.h>

bool FreeTypeFont::first_init = true;
ID3D10SamplerState* FreeTypeFont::m_Sampler;
ID3D10InputLayout* FreeTypeFont::m_pFontLayout;
ID3D10RasterizerState* FreeTypeFont::m_pRasterizerState;
ID3D10DepthStencilState* FreeTypeFont::m_pDSState;
ID3D10BlendState* FreeTypeFont::m_pBlendState;

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
	if (t_size.x < (c_with  + cur_pos.x))
	{
		cur_pos.x = 0;
		cur_pos.y += (int)h;
		static char name[256];
		sprintf(name, "atlas_%d.png", (int)((cur_pos.y) / m_Height));
		//stbi_write_png(name, atlas_size, (int)m_Height, 1, _test.data() + ((int)m_Height - cur_pos.y)*atlas_size, atlas_size);

	}

	//UCHAR* pTexels = (UCHAR*)mappedTex.pData;
	auto pTexels = image.data();
	int pos		 = 0;
	for (UINT row = 0; row < face->glyph->bitmap.rows; row++)
	{
		UINT rowStart = (row + cur_pos.y) * atlas_size; // * mappedTex.RowPitch;
		for (UINT col = 0; col < face->glyph->bitmap.width; col++, pos++)
		{
			UINT colStart					 = col * 1 + cur_pos.x;
			pTexels[rowStart + colStart + 0] = face->glyph->bitmap.buffer[pos] / 255.0f; // Red
			_test[rowStart + colStart + 0]	 = face->glyph->bitmap.buffer[pos]; // Red
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
	//return;
	auto render = GetISystem()->GetIRenderer();
	Vec4 cur_c(color[0], color[1], color[2], color[3]);
	glm::mat4 projection = glm::ortho(0.0f, (float)render->GetWidth(), (float)render->GetHeight(), 0.0f);

	// Iterate through all characters
	const char* c;
	const char* end = text.data() + text.size();
	for (c = text.data(); c != end; c++)
	{
		switch (*c)
		{
		case '\n':
		{
			posX = x = 4;
			posY	 = y += m_Height;
			continue;
		}
		case '$':
		{
			if ((c + 1) != end)
			{
				if (isdigit(*(++c)))
				{
					int colorIndex	= *c - '0';
					ColorB newColor = ColorTable[colorIndex].color;
					color[0]		= newColor.r;
					color[1]		= newColor.g;
					color[2]		= newColor.b;
					//sb->textColor = Vec3(Vec3(color[0], color[1], color[2]));
					cur_c = Vec4(1.f, Vec3(color[2], color[1], color[0]) / 255.f);
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
		Character ch = Characters[*c < 32 ? 0 : *c];
		//Character ch = Characters['%'];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		/*
		Coordinates of quad
		A---D 
		|	|
		B---C
		*/

		// Update VBO for each character
		using P3F_T2F					= SVF_P3F_C4B_T2F;
		Vec2 uv_pos						= Vec2(ch.Pos) / (float)atlas_size;
		Vec2 uv_size					= Vec2(ch.Size) / (float)atlas_size;

		Vec4 pA, pB, pC, pD;
		pA								= Vec4(Vec3{xpos, ypos - h, 0}, 1.f);
		pB								= Vec4(Vec3{xpos, ypos, 0}, 1.f);
		pC								= Vec4(Vec3{xpos + w, ypos, 0}, 1.f);
		pD								= Vec4(Vec3{xpos + w, ypos - h, 0}, 1.f);

		Vec2 tA{0.f / 160, 14.f / 127},
			tB{0.f / 160, 23.f / 127},
			tD{6.f / 160, 14.f / 127},
			tC{6.f / 160, 23.f / 127};
		#if 1 
		tA								= {uv_pos.x, uv_pos.y};
		tB								= {uv_pos.x, uv_pos.y + uv_size.y};
		tC								= {uv_pos.x + uv_size.x, uv_pos.y + uv_size.y};
		tD								= {uv_pos.x + uv_size.x, uv_pos.y};
		#endif

		std::array<P3F_T2F, 6> vertices = {
			P3F_T2F{Vec3(projection * pA), UCol((cur_c)), tA},
			P3F_T2F{Vec3(projection * pB), UCol((cur_c)), tB},
			P3F_T2F{Vec3(projection * pC), UCol((cur_c)), tC},
                                                
			P3F_T2F{Vec3(projection * pC), UCol((cur_c)), tC},
			P3F_T2F{Vec3(projection * pD), UCol((cur_c)), tD},
			P3F_T2F{Vec3(projection * pA), UCol((cur_c)), tA},
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
	float w			= 0.f;
	for (c = text.data(); c != end; c++)
	{
		w += CharWidth(*c);
	}
	return w;
}

float FreeTypeFont::CharWidth(char symbol)
{
	const float scale  = 1.0;
	const Character ch = Characters[symbol];

	const float w = (/*ch.Bearing.x + ch.Size.x + */ (ch.Advance >> 6)) * scale;
	//float h = (ch.Size.y - ch.Bearing.y + ch.Size.y) * scale;
	return w;
}

struct STestSize
{
	int width, height;
	STestSize(const int w, const int h)
		: width(w), height(h)
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
	m_Height = static_cast<float>(h);
	std::set<STestSize> test;
#if 1
	shader = (CShader*)gEnv->pRenderer->Sh_Load("sprite", 0);
#else
	shader = (CShader*)gEnv->pRenderer->Sh_Load("primitive", 0);
#endif
	if (!shader)
		return false;

	if (FT_Init_FreeType(&ft))
	{
		CryError("ERROR::FREETYPE: Could not init FreeType Library");
		return false;
	}

	if (FT_New_Face(ft, (std::string("res/fonts/") + font).c_str(), 0, &face))
	{
		CryError("ERROR::FREETYPE: Failed to load font");
		return false;
	}
	FT_Set_Pixel_Sizes(face, 0, h);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	glm::uvec2 t_size(atlas_size);
	glm::uvec2 cur_pos(0, 0);
	if (first_init)
	{
		D3D10_SAMPLER_DESC desc;
		ZeroStruct(desc);
		desc.AddressU = D3D10_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D10_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D10_TEXTURE_ADDRESS_WRAP;
		desc.Filter	  = D3D10_FILTER(D3D10_FILTER_ANISOTROPIC | D3D10_FILTER_MIN_MAG_MIP_LINEAR);
		auto hr		  = GetDevice()->CreateSamplerState(&desc, &m_Sampler);
		if (FAILED(hr))
		{
			CryError("Error create sampler for font");
			return false;
		}
		{
			//VERTEX_FORMAT_P3F_C4B_T2F
			D3D10_INPUT_ELEMENT_DESC layout[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0},
				//{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0}
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D10_INPUT_PER_VERTEX_DATA, 0}
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
		}
		CreateRasterState();
		CreateDSState();
		CreateBlendState();
		GlobalResources::FontAtlasRV = m_pTextureRV;
		GlobalResources::LinearSampler = m_Sampler;
		first_init = false;
	}

	D3D10_MAPPED_TEXTURE2D mappedTex;
	mappedTex.RowPitch = atlas_size * sizeof(float);

	//m_pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);
	std::vector<float> image(atlas_size * atlas_size);
	std::vector<uint8> _test(atlas_size * atlas_size);


	uint8 ch = 0;
	RenderGlyph(ch, cur_pos, t_size, image, _test);
	for (ch = 32; ch < num_glyphs; ch++)
	{
		RenderGlyph(ch, cur_pos, t_size, image, _test);
	}

	{
		{
			D3D10_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width			  = t_size.x;
			desc.Height			  = t_size.y;
			desc.MipLevels		  = 1;
			desc.ArraySize		  = 1;
			desc.Format			  = DXGI_FORMAT_R32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.Usage			  = D3D10_USAGE_DEFAULT;
			desc.BindFlags		  = D3D10_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags	  = 0;//D3D10_CPU_ACCESS_WRITE;

			D3D10_SUBRESOURCE_DATA srd;
			srd.pSysMem = image.data();
			srd.SysMemPitch = mappedTex.RowPitch;


			GetDevice()->CreateTexture2D(&desc, &srd, &m_pTexture);
		}
		// SEND TO SHADER
		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		D3D10_TEXTURE2D_DESC desc;
		m_pTexture->GetDesc(&desc);
		srvDesc.Format					  = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension			  = D3D10_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels		  = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;
		auto hr							  = GetDevice()->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pTextureRV);
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

		stbi_write_png("atlas.png", atlas_size, atlas_size, 1, _test.data(), atlas_size);
	}
	//m_pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	uint16 indices[] = {
		// Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3	 // Second Triangle
	};

	#if 1
	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_C4B_T2F, "Font", false);
	#else
	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_T2F, "Font", false);
	#endif
#if 0
    Vec3 vertices[] =
    {
        Vec3( 0.0f, 0.5f, 0.5f ),
        Vec3( 0.5f, -0.5f, 0.5f ),
        Vec3( -0.5f, -0.5f, 0.5f ),
    };
	gEnv->pRenderer->UpdateBuffer(m_VB, vertices, 3, false);
#endif

	m_IB = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_IB, indices, sizeof(indices));
	static bool UB_created = false;

	first_init = false;

	//GlobalResources::FontAtlasRV = m_pTextureRV;
	//GlobalResources::LinearSampler = m_Sampler;
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
	if (!shader)
	{
		m_CharBuffer.clear();
		return;
	}
	// Activate corresponding render state
	auto render = GetISystem()->GetIRenderer();
	gEnv->pRenderer->ReleaseBuffer(m_VB);
	SAFE_DELETE(m_VB);
	auto vertex_cnt = 6 * m_CharBuffer.size();
	
	#if 1
	m_VB = gEnv->pRenderer->CreateBuffer(vertex_cnt, VERTEX_FORMAT_P3F_C4B_T2F, "Font", false);
	#else
	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_T2F, "Font", false);
	#endif




	// Render glyph texture over quad
	// Update content of VBO memory
	gEnv->pRenderer->UpdateBuffer(m_VB, m_CharBuffer.data(), vertex_cnt, false);

	shader->Bind();
	GetDevice()->PSSetSamplers(0, 1, &m_Sampler);
	GetDevice()->PSSetShaderResources(0, 1, &m_pTextureRV);
	GetDevice()->IASetInputLayout(m_pFontLayout);
	GetDevice()->RSSetState(m_pRasterizerState);
	GetDevice()->OMSetBlendState(m_pBlendState, 0, 0xffffffff);
	//GetDevice()->OMSetDepthStencilState(m_pDSState, 0);

	static int font_method = 1;
	static bool registered	= false;
	if (!registered)
	{
		REGISTER_CVAR(font_method, font_method, 0, "font_method");
		registered = true;
	}
	switch (font_method)
	{
	case 1:
		gEnv->pRenderer->DrawBuffer(m_VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, vertex_cnt);
		break;
	default:
		for (int i = 0; i < vertex_cnt; i++)
		{
			gEnv->pRenderer->DrawBuffer(m_VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), i, i+1);
		}
		break;
	}


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
	D3D10_RASTERIZER_DESC rasterizerDesc;
	rasterizerDesc.CullMode				 = D3D10_CULL_NONE;
	rasterizerDesc.FillMode				 = D3D10_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.DepthBias			 = false;
	rasterizerDesc.DepthBiasClamp		 = 0;
	rasterizerDesc.SlopeScaledDepthBias	 = 0;
	rasterizerDesc.DepthClipEnable		 = true;
	rasterizerDesc.ScissorEnable		 = false;
	rasterizerDesc.MultisampleEnable	 = false;
	rasterizerDesc.AntialiasedLineEnable = true;

	GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerState);
	GetDevice()->RSSetState(m_pRasterizerState);
}
void FreeTypeFont::CreateDSState()
{
	D3D10_DEPTH_STENCIL_DESC desc;
	//desc.BackFace
	desc.DepthEnable = false;
	desc.StencilEnable = false;
	desc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ZERO;

	GetDevice()->CreateDepthStencilState(&desc, &m_pDSState);
}
void FreeTypeFont::CreateBlendState()
{

	D3D10_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D10_BLEND_DESC));

	BlendState.BlendEnable[0]			= TRUE;
	BlendState.SrcBlend					= D3D10_BLEND_SRC_ALPHA;
	BlendState.DestBlend				= D3D10_BLEND_INV_SRC_ALPHA;
	BlendState.BlendOp					= D3D10_BLEND_OP_ADD;
	BlendState.SrcBlendAlpha			= D3D10_BLEND_ONE;
	BlendState.DestBlendAlpha			= D3D10_BLEND_ZERO;
	BlendState.BlendOpAlpha				= D3D10_BLEND_OP_ADD;
	BlendState.RenderTargetWriteMask[0] = D3D10_COLOR_WRITE_ENABLE_ALL;

	GetDevice()->CreateBlendState(&BlendState, &m_pBlendState);
}
#endif
