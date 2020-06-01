#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>

#include <map>
#include <iostream>
#include <glm/glm.hpp>

class FreeTypeFont : public IFont
{
public:
  struct Character {
    uint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    FT_Pos     Advance;    // Offset to advance to next glyph
  };
  float posX = 0, posY = 0;

  FreeTypeFont() :
    face(nullptr),
    ft(nullptr),
    shader(nullptr)
  {
  }
  FreeTypeFont(const char* font, int w, int h) :
    face(nullptr),
    ft(nullptr),
    shader(nullptr)
  {
  }
  void RenderText(std::string text, float x, float y, float scale, float color[4]) override;
  virtual float TextWidth(const std::string& text) override;
  virtual float CharWidth(char ch) override;

private:
  FT_Library ft;
  FT_Face face;
  std::map<char, Character> Characters;

  CVertexBuffer* m_VB = nullptr;
  SVertexStream* m_IB = nullptr;
  BaseShaderProgramRef shader;

public:
  // Унаследовано через IFont
  virtual bool Init(const char* font, unsigned int w, unsigned int h) override;

  // Унаследовано через IFont
  virtual float GetXPos() override;

  virtual float GetYPos() override;

  // Унаследовано через IFont
  virtual void SetXPos(float x) override;

  virtual void SetYPos(float y) override;
};
