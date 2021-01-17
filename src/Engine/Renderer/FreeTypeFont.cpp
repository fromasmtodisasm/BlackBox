#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include "D3D/Shader.hpp"

#include <BlackBox/Renderer/MaterialManager.hpp>

#include "BaseRenderer.hpp"
//#include <BlackBox/Renderer/OpenGL/Core.hpp>

#include <memory>

#include "D3D/Renderer.h"

using ColorB = glm::u8vec3;

bool FreeTypeFont::first_init = true;
ID3D10SamplerState* FreeTypeFont::m_Sampler{};
ID3D10InputLayout* FreeTypeFont::m_pFontLayout{};

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

void FreeTypeFont::RenderText(const std::string_view text, float x, float y, float scale, float color[4])
{
	return;
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
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// Update VBO for each character
		using P3F_T2F					= SVF_P3F_C4B_T2F;
		Vec2 uv_pos						= Vec2(ch.Pos) / 4096.f;
		Vec2 uv_size					= Vec2(ch.Size) / 4096.f;
		std::array<P3F_T2F, 6> vertices = {
			P3F_T2F{Vec3(projection * Vec4(Vec3{xpos, ypos - h, 0}, 1.f)), UCol((cur_c)), uv_pos.x, uv_pos.y},
			P3F_T2F{Vec3(projection * Vec4(Vec3{xpos, ypos, 0}, 1.f)), UCol((cur_c)), uv_pos.x, uv_pos.y + uv_size.y},
			P3F_T2F{Vec3(projection * Vec4(Vec3{xpos + w, ypos, 0}, 1.f)), UCol((cur_c)), uv_pos.x + uv_size.x, uv_pos.y + uv_size.y},

			P3F_T2F{Vec3(projection * Vec4(Vec3{xpos + w, ypos, 0}, 1.f)), UCol((cur_c)), uv_pos.x + uv_size.x, uv_pos.y + uv_size.y},
			P3F_T2F{Vec3(projection * Vec4(Vec3{xpos + w, ypos - h, 0}, 1.f)), UCol((cur_c)), uv_pos.x + uv_size.x, uv_pos.y},
			P3F_T2F{Vec3(projection * Vec4(Vec3{xpos, ypos - h, 0}, 1.f)), UCol((cur_c)), uv_pos.x, uv_pos.y},
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
#if 0
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

	glm::uvec2 t_size(4096);
	glm::uvec2 cur_pos(0,0);

	// Create the render target texture
	{
		{
			D3D10_TEXTURE2D_DESC desc;
			ZeroMemory( &desc, sizeof(desc) );
			desc.Width			  = t_size.x;
			desc.Height			  = t_size.y;
			desc.MipLevels		  = 1;
			desc.ArraySize		  = 1;
			desc.Format			  = DXGI_FORMAT_R32_FLOAT;
			desc.SampleDesc.Count = 1;
			desc.Usage			  = D3D10_USAGE_DYNAMIC;
			desc.BindFlags		  = D3D10_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags	  = D3D10_CPU_ACCESS_WRITE;

			GetDevice()->CreateTexture2D( &desc, NULL, &m_pTexture );	
			
		}
		// SEND TO SHADER
		D3D10_SHADER_RESOURCE_VIEW_DESC srvDesc;
		D3D10_TEXTURE2D_DESC desc;
		m_pTexture->GetDesc(&desc);
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension			  = D3D10_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels		  = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = desc.MipLevels - 1;
		auto hr							  = GetDevice()->CreateShaderResourceView(m_pTexture, &srvDesc, &pTexDepSurface);
		if (FAILED(hr))
		{
			CryError("Failed to create texture view");
		}
		GetDevice()->PSSetShaderResources(0, 1, &pTexDepSurface);
	}
	if (first_init)
	{
		D3D10_SAMPLER_DESC desc;
		ZeroStruct(desc);
		desc.AddressU = D3D10_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D10_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D10_TEXTURE_ADDRESS_CLAMP;
		desc.Filter	  = D3D10_FILTER_MIN_MAG_MIP_LINEAR;
		auto hr = GetDevice()->CreateSamplerState(&desc, &m_Sampler);
		if (FAILED(hr))
		{
			CryError("Error create sampler for font");
			return false;
		}
		{
			//VERTEX_FORMAT_P3F_C4B_T2F
			D3D10_INPUT_ELEMENT_DESC layout[] = {
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
				//{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
				//{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D10_INPUT_PER_VERTEX_DATA, 0}
			};
			auto hr = GetDevice()->CreateInputLayout(
				layout,
				1, 
				shader->m_Shaders[IShader::Type::E_VERTEX]->m_Bytecode->GetBufferPointer(),
				shader->m_Shaders[IShader::Type::E_VERTEX]->m_Bytecode->GetBufferSize(),
				&m_pFontLayout
			);
			if (FAILED(hr))
			{
				CryError("Error create input layout for font");
				return false;
			}
		}
		first_init = false;
	}

	for (uint8 ch = 0; ch < 255; ch++)
	{
		// Load character glyph
		uint c = ch;
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			CryError("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}
#	if 0
		test.insert(
			STestSize(
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows));
#	endif
		auto& c_with = face->glyph->bitmap.width;
		auto& c_rows = face->glyph->bitmap.rows;
		if ((t_size.x - cur_pos.x) < c_with)
		{
			cur_pos.x = 0;
			cur_pos.y += h;	
		}
		D3D10_MAPPED_TEXTURE2D mappedTex;
		m_pTexture->Map(D3D10CalcSubresource(0, 0, 1), D3D10_MAP_WRITE_DISCARD, 0, &mappedTex);

		UCHAR* pTexels = (UCHAR*)mappedTex.pData;
		int pos		   = 0;
		for (UINT row = cur_pos.y; row < face->glyph->bitmap.rows; row++)
		{
			UINT rowStart = row * mappedTex.RowPitch;
			for (UINT col = 0; col < face->glyph->bitmap.width; col++, pos++)
			{
				UINT colStart					 = col * 1;
				pTexels[rowStart + colStart + 0] = face->glyph->bitmap.buffer[pos]; // Red
			}
		}

		m_pTexture->Unmap(D3D10CalcSubresource(0, 0, 1));


		// Set texture options
		// Now store character for later use
			Character character = {
				cur_pos,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x};
			Characters.insert(std::pair<char, Character>(ch, character));
			cur_pos.x += character.Size.x;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	uint16 indices[] = {
		// Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3	 // Second Triangle
	};

	//m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_C4B_T2F, "BoundingBox", false);
	m_VB = gEnv->pRenderer->CreateBuffer(3, VERTEX_FORMAT_P3F, "BoundingBox", false);
    Vec3 vertices[] =
    {
        Vec3( 0.0f, 0.5f, 0.5f ),
        Vec3( 0.5f, -0.5f, 0.5f ),
        Vec3( -0.5f, -0.5f, 0.5f ),
    };
	gEnv->pRenderer->UpdateBuffer(m_VB, vertices, 3, false);

	m_IB = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_IB, indices, sizeof(indices));
	static bool UB_created = false;

	first_init = false;
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

IFont* CreateIFont()
{
	return new FreeTypeFont();
}

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
	//shader->Use();
	auto render = GetISystem()->GetIRenderer();
	//gl::ActiveTexture(GL_TEXTURE0);
	RSS(render, BLEND, true);
	RSS(render, CULL_FACE, false);
	RSS(render, DEPTH_TEST, false);
	//glDepthMask(GL_FALSE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//gl::BindTexture2D(GL_TEXTURE_2D, texture);
#if 0
	gEnv->pRenderer->ReleaseBuffer(m_VB);
	SAFE_DELETE(m_VB);
	auto vertex_cnt = 6 * m_CharBuffer.size();
	m_VB			= gEnv->pRenderer->CreateBuffer(vertex_cnt, VERTEX_FORMAT_P3F_C4B_T2F, "BoundingBox", false);

	// Render glyph texture over quad
	// Update content of VBO memory
	gEnv->pRenderer->UpdateBuffer(m_VB, m_CharBuffer.data(), vertex_cnt, false);
#endif

	auto vertex_cnt = 3;
	//gEnv->pRenderer->DrawBuffer(m_VB, m_IB, 6, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
	shader->Bind();
	GetDevice()->PSSetSamplers(0, 1, &m_Sampler);
	GetDevice()->PSSetShaderResources(0,1, &pTexDepSurface);
	GetDevice()->IASetInputLayout(m_pFontLayout);
	gEnv->pRenderer->DrawBuffer(m_VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, vertex_cnt);

	//gl::BindTexture2D(GL_TEXTURE_2D, 0);
	//glDepthMask(GL_TRUE);
	m_CharBuffer.resize(0);
}

FreeTypeFont::~FreeTypeFont()
{
	gEnv->pRenderer->ReleaseIndexBuffer(m_IB);
	gEnv->pRenderer->ReleaseBuffer(m_VB);
}

void FreeTypeFont::Release()
{
	delete this;
}
