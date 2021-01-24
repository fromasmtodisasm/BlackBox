#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>

#include <memory>
#include <stb_image_write.h>

using ColorB = glm::u8vec3;

struct ColorTable
{
	ColorB color;
	string name;
}  ColorTable[10] =
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
	ColorB(0x8f, 0x8f, 0x8f), "grey"
};

void PrintColorTable(IConsoleCmdArgs*)
{
	gEnv->pLog->Log("ColorTable codes");
	for (int i = 0; i < 10; i++)
	{
		gEnv->pLog->Log("$%d $$%d=%s", i, i, ColorTable[i].name.data());
	}
}

void FreeTypeFont::RenderGlyph(uint ch, glm::uvec2& cur_pos, const glm::uvec2& t_size, std::vector<uint8>& image)
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
	static int num_rows = 0;
	if (t_size.x < (c_with  + cur_pos.x))
	{
		cur_pos.x = 0;
		cur_pos.y += (int)h;
		static char name[256];
		num_rows++;
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
			UINT colStart = col * 1 + cur_pos.x;
			assert(rowStart + colStart < image.size());
			pTexels[rowStart + colStart + 0] = face->glyph->bitmap.buffer[pos]; // / 255.0f; // Red
			//_test[rowStart + colStart + 0]	 = face->glyph->bitmap.buffer[pos]; // Red
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

bool FreeTypeFont::printColorTableRegistered = false;

void FreeTypeFont::RenderText(const std::string& text, float x, float y, float scale, float color[4])
{
	auto pRenderer = GetISystem()->GetIRenderer();
	Vec4 cur_c(color[0], color[1], color[2], color[3]);
	glm::mat4 projection = glm::ortho(0.0f, (float)pRenderer->GetWidth(), (float)pRenderer->GetHeight(), 0.0f);

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
					int colorIndex = *c - '0';
					ColorB newColor = ColorTable[colorIndex].color;
					color[0] = newColor.r;
					color[1] = newColor.g;
					color[2] = newColor.b;
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
		if (auto it = Characters.find(*c); it == Characters.end())	
		{
			#if 0
			auto old_pos = cur_pos;
			RenderGlyph(*c, cur_pos, glm::uvec2{atlas_size}, image);	
			UpdateAtlas(glm::uvec4{old_pos, glm::uvec2{cur_pos - old_pos}}, image.data());
			#endif
		}
		glm::mat4 model(1.0);
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		using P3F_T2F					= SVF_P3F_C4B_T2F;
		Vec2 uv_pos						= Vec2(ch.Pos) / (float)atlas_size;
		Vec2 uv_size					= Vec2(ch.Size) / (float)atlas_size;

		Vec4 pA, pB, pC, pD;
		pA								= Vec4(Vec3{xpos, ypos - h, 0}, 1.f);
		pB								= Vec4(Vec3{xpos, ypos, 0}, 1.f);
		pC								= Vec4(Vec3{xpos + w, ypos, 0}, 1.f);
		pD								= Vec4(Vec3{xpos + w, ypos - h, 0}, 1.f);

		Vec2 tA, tB, tC, tD;
		tA								= {uv_pos.x, uv_pos.y};
		tB								= {uv_pos.x, uv_pos.y + uv_size.y};
		tC								= {uv_pos.x + uv_size.x, uv_pos.y + uv_size.y};
		tD								= {uv_pos.x + uv_size.x, uv_pos.y};

		std::array<P3F_T2F, 6> vertices = {
			P3F_T2F{Vec3(projection * pA), UCol((cur_c)), tA},
			P3F_T2F{Vec3(projection * pB), UCol((cur_c)), tB},
			P3F_T2F{Vec3(projection * pC), UCol((cur_c)), tC},

			P3F_T2F{Vec3(projection * pC), UCol((cur_c)), tC},
			P3F_T2F{Vec3(projection * pD), UCol((cur_c)), tD},
			P3F_T2F{Vec3(projection * pA), UCol((cur_c)), tA},
		};		m_CharBuffer.push_back(vertices);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		posX = x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
}

float FreeTypeFont::TextWidth(const std::string& text)
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

	const GLfloat w = (/*ch.Bearing.x + ch.Size.x + */ (ch.Advance >> 6)) * scale;
	//GLfloat h = (ch.Size.y - ch.Bearing.y + ch.Size.y) * scale;
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
bool operator< (const STestSize& a, const STestSize& b) {
	if (a.width != b.width) return (a.width < b.width);
	return (a.height < b.height);
}
void FreeTypeFont::UpdateAtlas(const glm::uvec4 region, void* data)
{
	glTextureSubImage2D(this->texture, 0, region.x, region.y, region.z, region.w, GL_RED, GL_UNSIGNED_BYTE, data);
}

bool FreeTypeFont::Init(const char* font, unsigned int w, unsigned int h)
{
	m_Height = static_cast<float>(h);
	std::set<STestSize> test;
	shader = gEnv->pRenderer->Sh_Load("sprite", 0);
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
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	glm::uvec2 t_size(atlas_size);

	// Generate texture
	gl::CreateTextures2D(GL_TEXTURE_2D, 1, &texture);
	glTextureStorage2D(texture, 1, GL_R8, atlas_size, atlas_size);

	//std::vector<float> image(atlas_size * atlas_size);

	uint8 ch = 0;
	RenderGlyph(ch, cur_pos, t_size, image);
	for (ch = 32; ch < num_glyphs; ch++)
	{
		RenderGlyph(ch, cur_pos, t_size, image);
	}
	UpdateAtlas({0, 0, atlas_size, atlas_size}, image.data());
	stbi_write_png("atlas.png", atlas_size, atlas_size, 1, image.data(), atlas_size);

	uint16 indices[] = {
		// Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_C4B_T2F, "BoundingBox", false);

	m_IB = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_IB, indices, sizeof(indices));
	static bool UB_created = false;

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
	if (m_CharBuffer.size() == 0)
		return;
	// Activate corresponding pRenderer state
	shader->Use();
	auto render				= GetISystem()->GetIRenderer();
	gl::ActiveTexture(GL_TEXTURE0);
	RSS(render, BLEND, true);
	RSS(render, CULL_FACE, false);
	RSS(render, DEPTH_TEST, false);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gl::BindTexture2D(GL_TEXTURE_2D, texture);
	gEnv->pRenderer->ReleaseBuffer(m_VB);
	SAFE_DELETE(m_VB);
	auto vertex_cnt = 6 * m_CharBuffer.size();
	m_VB = gEnv->pRenderer->CreateBuffer(vertex_cnt, VERTEX_FORMAT_P3F_C4B_T2F, "BoundingBox", false);


	// Render glyph texture over quad
	// Update content of VBO memory
	gEnv->pRenderer->UpdateBuffer(m_VB, m_CharBuffer.data(), vertex_cnt, false);

	//gEnv->pRenderer->DrawBuffer(m_VB, m_IB, 6, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
	gEnv->pRenderer->DrawBuffer(m_VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, vertex_cnt);

	gl::BindTexture2D(GL_TEXTURE_2D, 0);
	glDepthMask(GL_TRUE);
	m_CharBuffer.resize(0);
}

FreeTypeFont::~FreeTypeFont()
{
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	gEnv->pRenderer->ReleaseIndexBuffer(m_IB);
}
