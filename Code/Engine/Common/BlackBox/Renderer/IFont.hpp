#pragma once

struct IFont
{
	virtual bool  Init(const char* font, unsigned int w, unsigned int h)                                 = 0;
	virtual void  Release()                                                                              = 0;
	virtual void  RenderText(const std::string_view text, float x, float y, float scale, float color[4]) = 0;
	virtual float GetXPos()                                                                              = 0;
	virtual float GetYPos()                                                                              = 0;
	virtual void  SetXPos(float x)                                                                       = 0;
	virtual void  SetYPos(float y)                                                                       = 0;
	virtual float TextWidth(const std::string_view text)                                                 = 0;
	virtual float CharWidth(char ch)                                                                     = 0;
	virtual void  Submit()                                                                               = 0;
};

extern "C"
{
	IFont* CreateIFont();
}