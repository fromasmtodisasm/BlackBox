// drawer2D.cpp

#ifdef ENABLE_PROFILER
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Profiler/Drawer2D.h>
#include <BlackBox/Profiler/Utils.h>
#include <BlackBox/Profiler/Tgaloader.h>
#include <stdio.h>
#include <math.h>

#define FONT_FILENAME	"media/times_new_roman.tga"
#define ATTRIB_VERTEX	0
#define ATTRIB_UV		1

Drawer2D drawer2D;

//-----------------------------------------------------------------------------
bool Drawer2D::init(int win_w, int win_h)
{
  m_win_w = win_w;
  m_win_h = win_h;

  if (!Utils::loadShaders("media/color.vert", "media/color.frag", m_id_vert_color, m_id_frag_color, m_id_prog_color))
  {
    fprintf(stderr, "*** Drawer2D: FAILED loading shaders for uniform color\n");
    return false;
  }

  m_id_uniform_color = glGetUniformLocation(m_id_prog_color, "color");
  if (m_id_uniform_color < 0)
  {
    fprintf(stderr, "*** Drawer2D: FAILED retrieving uniform location\n");
    return false;
  }

  GLuint* vaos[] = { &m_id_vao_rect, &m_id_vao_string };
  GLuint* vbos[] = { &m_id_vbo_rect, &m_id_vbo_string };

  glGenVertexArrays(sizeof(vaos) / sizeof(vaos[0]), *vaos);
  glCheck(glGenBuffers(sizeof(vbos) / sizeof(vbos[0]), *vbos));

  glCheck(glBindVertexArray(m_id_vao_rect));
  glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id_vbo_rect));
  glCheck(glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW));
  glCheck(glBindVertexArray(0));

  glCheck(glBindVertexArray(m_id_vao_string));
  glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id_vbo_string));
  glCheck(glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW));
  glCheck(glBindVertexArray(0));

  if (!initFont())
    return false;

  return true;
}

//-----------------------------------------------------------------------------
void Drawer2D::shut()
{
  glDeleteProgram(m_id_prog_color);
  glDeleteShader(m_id_frag_color);
  glDeleteShader(m_id_vert_color);
  glDeleteBuffers(1, &m_id_vbo_rect);
}

//-----------------------------------------------------------------------------
void Drawer2D::drawRect(const Utils::Rect& rect, const Utils::Color& color, float alpha)
{
  glCheck(glBindVertexArray(m_id_vao_rect));
  glCheck(glEnableVertexAttribArray(ATTRIB_VERTEX));

  glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id_vbo_rect));

  float x1 = rect.x;
  float y1 = rect.y;
  float x2 = rect.x + rect.w;
  float y2 = rect.y + rect.h;

  GLfloat vertices[] = {
    x1, y1,
    x2, y1,
    x2, y2,
    x2, y2,
    x1, y2,
    x1, y1
  };
  glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (const GLvoid*)vertices, GL_DYNAMIC_DRAW));
  glCheck(glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));

  glCheck(glUseProgram(m_id_prog_color));

  float color_array[] = { (float)(color.r) / 255.0f,
               (float)(color.g) / 255.0f,
               (float)(color.b) / 255.0f,
               alpha
  };
  glCheck(glUniform4fv(m_id_uniform_color, 1, color_array));

  glCheck(glDrawArrays(GL_TRIANGLES, 0, 6));

  glCheck(glDisableVertexAttribArray(ATTRIB_VERTEX));
  glCheck(glBindVertexArray(0));
}

//-----------------------------------------------------------------------------
void Drawer2D::drawString(const char* str, float x, float y, const Utils::Color& color)
{
  glCheck(glBindVertexArray(m_id_vao_string));
  glCheck(glEnableVertexAttribArray(ATTRIB_VERTEX));
  glCheck(glEnableVertexAttribArray(ATTRIB_UV));

  glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id_vbo_rect));

  const int nb_chars_side = 16;	// we got 16 characters on one side of our squared image
  const int char_size = 16;	// in the original image, one character is 16x16 pixels

  const float uv_stride = 1.0f / (float)nb_chars_side;	// 0.0625: how much we need to add to the UV
                              // coordinates to go to the next character

  const float screen_char_width = (float)char_size / (float)m_win_w;	// size of one character in the
  const float screen_char_height = (float)char_size / (float)m_win_h;	// [0;1]x[0;1] screen-space basis

  glCheck(glUseProgram(m_id_prog_font));

  gl::ActiveTexture(GL_TEXTURE0);
  gl::BindTexture2D(m_id_tex_font);

  glCheck(glUniform1i(m_id_uniform_tex_font, 0));
  glCheck(glUniform3f(m_id_uniform_font_color,
    ((float)color.r) / 255.0f,
    ((float)color.g) / 255.0f,
    ((float)color.b) / 255.0f));

  float cur_x = x;
  float cur_y = y;

  const char* p = str;
  while (*p)
  {
    char c = *p;
    float u = (float)(((int)c % nb_chars_side) * uv_stride);
    float v = (float)(((nb_chars_side - 1) - (int)floor(float((int)c * uv_stride))) * uv_stride);

    if (c == '\n')
    {
      cur_x = x;
      cur_y -= screen_char_height;
    }
    else
    {
      float u1 = u;
      float v1 = v;
      float u2 = u + uv_stride;
      float v2 = v + uv_stride;

      float x1 = cur_x;
      float y1 = cur_y;
      float x2 = cur_x + screen_char_width;
      float y2 = cur_y + screen_char_height;

      GLfloat vertices[] = {
        x1, y1,
        x2, y1,
        x2, y2,
        x2, y2,
        x1, y2,
        x1, y1,

        u1, v1,
        u2, v1,
        u2, v2,
        u2, v2,
        u1, v2,
        u1, v1,
      };

      glCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (const GLvoid*)vertices, GL_DYNAMIC_DRAW));
      gl::VertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      gl::VertexAttribPointer(ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, 0, (void*)(6 * 2 * sizeof(GLfloat)));

      glCheck(glDrawArrays(GL_TRIANGLES, 0, 6));

      //cur_x += screen_char_width;
      cur_x += 0.9f * screen_char_width;	// HACK
    }

    p++;
  }

  glCheck(glDisableVertexAttribArray(ATTRIB_UV));
  glCheck(glDisableVertexAttribArray(ATTRIB_VERTEX));
  glCheck(glBindVertexArray(0));
}

//-----------------------------------------------------------------------------
bool Drawer2D::initFont()
{
  // Load the TGA image
  TGALoader		tga;
  TGAErrorCode	error = tga.loadFile(FONT_FILENAME);
  if (error != TGA_OK)
  {
    fprintf(stderr, "*** Drawer2D: FAILED loading font " FONT_FILENAME "\n");
    return false;
  }

  // Create the texture
  glCheck(glGenTextures(1, &m_id_tex_font));
  glCheck(glBindTexture(GL_TEXTURE_2D, m_id_tex_font));

  gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  gl::TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glCheck(glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA8,
    tga.getWidth(), tga.getHeight(),
    0,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    (const GLvoid*)tga.getData()));

  // Create the shader
  if (!Utils::loadShaders("media/font.vert", "media/font.frag", m_id_vert_font, m_id_frag_font, m_id_prog_font))
  {
    fprintf(stderr, "*** Drawer2D: FAILED loading shaders for font\n");
    return false;
  }

  // Uniform retrieval
  m_id_uniform_tex_font = glGetUniformLocation(m_id_prog_font, "tex_font");
  if (m_id_uniform_tex_font < 0)
  {
    fprintf(stderr, "*** Drawer2D: FAILED retrieving uniform location\n");
    return false;
  }

  m_id_uniform_font_color = glGetUniformLocation(m_id_prog_font, "font_color");
  if (m_id_uniform_font_color < 0)
  {
    fprintf(stderr, "*** Drawer2D: FAILED retrieving uniform location\n");
    return false;
  }

  return true;
}
#endif