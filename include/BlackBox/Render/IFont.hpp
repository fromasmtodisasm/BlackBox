#pragma once

struct IFont
{
  virtual bool Init(const char* font, unsigned int w, unsigned int h) = 0;
  virtual void RenderText(std::string text, float x, float y, float scale, float color[4]) = 0;
  virtual float GetXPos() = 0;
  virtual float GetYPos() = 0;
  virtual void SetXPos(float x) = 0;
  virtual void SetYPos(float y) = 0;

  virtual float TextWidth(const std::string& text) = 0;
  virtual float CharWidth(char ch) = 0;
};