#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

#include <memory>

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

bool FreeTypeFont::printColorTableRegistered = false;
struct alignas(16) SpriteConstantBuffer
{
	Mat4 projection;
	Mat4 model;
	Mat4 uv_projection;
	Vec3 textColor;
	Vec2 uv;
};

using SpriteBuffer = gl::ConstantBuffer<SpriteConstantBuffer>;
using SpriteBufferPtr = std::shared_ptr<SpriteBuffer>;
SpriteBufferPtr  spriteBuffer;

void FreeTypeFont::RenderText(const std::string& text, float x, float y, float scale, float color[4])
{
	// Activate corresponding render state
	shader->Use();
	auto render				= GetISystem()->GetIRenderer();
	glm::mat4 uv_projection = glm::mat4(1.0);
	//uv_projection = glm::scale(uv_projection, glm::vec3(1.0f, -1.0f, 1.0f));
	glm::mat4 projection = glm::ortho(0.0f, (float)render->GetWidth(), (float)render->GetHeight(), 0.0f);
	gl::ActiveTexture(GL_TEXTURE0);
	RSS(render, BLEND, true);
	RSS(render, CULL_FACE, false);
	RSS(render, DEPTH_TEST, false);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Vec4 cur_c(color[0], color[1], color[2], color[3]);

	{
		auto c = color;
		auto sb = spriteBuffer;
		sb->projection = projection;
		sb->uv_projection = uv_projection;
		sb->textColor = Vec3(c[0], c[1], c[2]);
	}

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
					auto sb = spriteBuffer;
					//sb->textColor = Vec3(Vec3(color[0], color[1], color[2]));
					cur_c = Vec4(Vec3(Vec3(color[0], color[1], color[2]) / 255.f),1);
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

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		using P3F_T2F		= SVF_P3F_C4B_T2F;
		Vec2 uv_pos			= Vec2(ch.Pos) / 4096.f;
		Vec2 uv_size		= Vec2(ch.Size) / 4096.f;
		P3F_T2F vertices[6] = {
			P3F_T2F{Vec3{xpos, ypos - h, 0}, UCol(Vec4(cur_c)), uv_pos.x, uv_pos.y},
			P3F_T2F{Vec3{xpos, ypos, 0}, UCol(Vec4(cur_c)), uv_pos.x, uv_pos.y + uv_size.y},
			P3F_T2F{Vec3{xpos + w, ypos, 0}, UCol(Vec4(cur_c)), uv_pos.x + uv_size.x, uv_pos.y + uv_size.y},

			P3F_T2F{Vec3{xpos + w, ypos, 0}, UCol(Vec4(cur_c)), uv_pos.x + uv_size.x, uv_pos.y + uv_size.y},
			P3F_T2F{Vec3{xpos + w, ypos - h, 0}, UCol(Vec4(cur_c)), uv_pos.x + uv_size.x, uv_pos.y},
			P3F_T2F{Vec3{xpos, ypos - h, 0}, UCol(Vec4(cur_c)), uv_pos.x, uv_pos.y},
		};
		auto sb = spriteBuffer;
		sb->model = model;
		sb->Update();
		// Render glyph texture over quad
		gl::BindTexture2D(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		gEnv->pRenderer->UpdateBuffer(m_VB, vertices, 6, false);

		//gEnv->pRenderer->DrawBuffer(m_VB, m_IB, 6, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
		gEnv->pRenderer->DrawBuffer(m_VB, 0, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES), 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		posX = x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	gl::BindTexture2D(GL_TEXTURE_2D, 0);
	glDepthMask(GL_TRUE);
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

	glm::uvec2 t_size(4096);
	glm::uvec2 cur_pos(0,0);

	// Generate texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		t_size.x,//face->glyph->bitmap.width,
		t_size.y,//face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	for (GLubyte ch = 0; ch < 255; ch++)
	{
		// Load character glyph
		GLuint c = ch;
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			CryError("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}
		#if 0
		test.insert(
			STestSize(
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows));
		#endif
		auto& c_with = face->glyph->bitmap.width;
		auto& c_rows = face->glyph->bitmap.rows;
		if ((t_size.x - cur_pos.x) < c_with)
		{
			cur_pos.x = 0;
			cur_pos.y += h;	
		}
		//glTexImage2D(
		//	GL_TEXTURE_2D,
		//	0,
		//	GL_RED,
		//	t_size.x,//face->glyph->bitmap.width,
		//	t_size.y,//face->glyph->bitmap.rows,
		//	0,
		//	GL_RED,
		//	GL_UNSIGNED_BYTE,
		//	face->glyph->bitmap.buffer);
		//void (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
		glTexSubImage2D(GL_TEXTURE_2D,
							 0,
							 cur_pos.x,
							 cur_pos.y,
							 face->glyph->bitmap.width,
							 face->glyph->bitmap.rows,
							 GL_RED,
							 GL_UNSIGNED_BYTE,
							 face->glyph->bitmap.buffer);

		// Set texture options
		// Now store character for later use
			Character character = {
				texture,
				cur_pos,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x};
			Characters.insert(std::pair<GLchar, Character>(ch, character));
			cur_pos.x += character.Size.x;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	uint16 indices[] = {
		// Note that we start from 0!
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};

	m_VB = gEnv->pRenderer->CreateBuffer(6, VERTEX_FORMAT_P3F_C4B_T2F, "BoundingBox", false);

	m_IB = new SVertexStream;
	gEnv->pRenderer->CreateIndexBuffer(m_IB, indices, sizeof(indices));
	static bool UB_created = false;

	if (!UB_created)
		spriteBuffer = SpriteBuffer::Create(15);

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
