#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>

void FreeTypeFont::RenderText(std::string text, float x, float y, float scale, float color[4])
{
  // Activate corresponding render state
  shader->Use();
  auto render = GetISystem()->GetIRenderer();
  glm::mat4 uv_projection = glm::mat4(1.0);
  //uv_projection = glm::scale(uv_projection, glm::vec3(1.0f, -1.0f, 1.0f));
  glm::mat4 projection = glm::ortho(0.0f, (float)render->GetWidth(), (float)render->GetHeight(), 0.0f);
  {
    auto c = color;
    shader->Uniform(projection, "projection");
    shader->Uniform(uv_projection, "uv_projection");
    shader->Uniform(Vec3(c[0], c[1], c[2]), "textColor");
  }
  gl::ActiveTexture(GL_TEXTURE0);
  RSS(render, BLEND, true);
  RSS(render, CULL_FACE, false);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Iterate through all characters
  const char* c;
  const char* end = text.data() + text.size();
  for (c = text.data(); c != end; c++)
  {
    if (*c == '\n')
    {
      posX = x = 0;
      posY = y += 18;
      continue;
    }
    if (*c >= 0 && *c <= 255 && iscntrl(*c))
      continue;
    glm::mat4 model(1.0);
    Character ch = Characters[*c];

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y + (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    // Update VBO for each character
    using P3F_TEX2F = struct_VERTEX_FORMAT_P3F_TEX2F;
    P3F_TEX2F vertices[4] = {
      P3F_TEX2F{ Vec3{xpos,      ypos - h, 0}, 0.0, 0.0 },
      P3F_TEX2F{ Vec3{xpos,      ypos,     0}, 0.0, 1.0 },
      P3F_TEX2F{ Vec3{ xpos + w, ypos,     0}, 1.0, 1.0 },
      P3F_TEX2F{ Vec3{ xpos + w, ypos - h, 0}, 1.0, 0.0 }
    };
    shader->Uniform(model, "model");
    // Render glyph texture over quad
    gl::BindTexture2D(ch.TextureID);
    // Update content of VBO memory
    gEnv->pRenderer->UpdateBuffer(m_VB, vertices, 4, false);

    gEnv->pRenderer->DrawBuffer(m_VB, m_IB, 6, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
    // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    posX = x += (ch.Advance >> 6)* scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }
  gl::BindTexture2D(0);
}

float FreeTypeFont::TextWidth(const std::string& text)
{
  const char* c;
  const char* end = text.data() + text.size();
  float w = 0.f;
  for (c = text.data(); c != end; c++)
  {
    GLfloat x = 0;
    w += CharWidth(*c);
  }
  return w;
}

float FreeTypeFont::CharWidth(char symbol)
{
  float scale = 1.0;
  Character ch = Characters[symbol];

  GLfloat w = (/*ch.Bearing.x + ch.Size.x + */(ch.Advance >> 6))* scale;
  //GLfloat h = (ch.Size.y - ch.Bearing.y + ch.Size.y) * scale;
  return w;
}

bool FreeTypeFont::Init(const char* font, unsigned int w, unsigned int h)
{
  ProgramDesc pd = {
    "sprite",
    ShaderDesc("sprite.vs"),
    ShaderDesc("sprite.frag")
  };

  if (!MaterialManager::instance()->loadProgram(pd, false))
    return false;
  shader = MaterialManager::instance()->getProgram(pd.name);
  if (!shader)
    return false;

  if (FT_Init_FreeType(&ft))
  {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    return false;
  }

  if (FT_New_Face(ft, (std::string("res/fonts/") + font).c_str(), 0, &face))
  {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    return false;
  }

  FT_Set_Pixel_Sizes(face, 0, h);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

  for (GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
      continue;
    }
    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      face->glyph->bitmap.width,
      face->glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x
    };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  uint16 indices[] = {  // Note that we start from 0!
    0, 1, 3,  // First Triangle
    1, 2, 3   // Second Triangle
  };

  m_VB = gEnv->pRenderer->CreateBuffer(4, VERTEX_FORMAT_P3F_TEX2F, "BoundingBox", false);

  m_IB = new SVertexStream;
  gEnv->pRenderer->CreateIndexBuffer(m_IB, indices, sizeof(indices));

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