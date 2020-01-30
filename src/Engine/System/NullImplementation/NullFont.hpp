#pragma once
#include <BlackBox/Renderer/IFont.hpp>

class CNullFont : public IFont
{
public:
  bool Init(const char* font, unsigned int w, unsigned int h) override { return true; };
  void RenderText(std::string text, float x, float y, float scale, float color[4]) override {};
  float GetXPos() override { return 0.f; };
  float GetYPos() override { return 0.f; };
  void SetXPos(float x) override {};
  void SetYPos(float y) override {};

  float TextWidth(const std::string& text) override { return 0.f; };
  float CharWidth(char ch) override { return 0.f; };
};
