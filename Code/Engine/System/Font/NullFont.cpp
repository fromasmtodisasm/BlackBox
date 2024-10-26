#include "pch.hpp"
#include "NullFont.h"
#if defined(USE_NULLFONT)

CNullCryFont CCryNullFont::ms_nullFont;

#endif // USE_NULLFONT

IFFont* CCryNullFont::NewFont(const char* pFontName) { 
	return &ms_nullFont; 
}

IFFont* CCryNullFont::GetFont(const char* pFontName) 
{ 
	if (m_Fonts.find(pFontName) != m_Fonts.end())
	{
		return m_Fonts[pFontName];
	}
	else
	{
		auto font = new CNullCryFont();
		m_Fonts[pFontName] = font;
		return font;
	}
}

IFont* CNullCryFont::GetIFont() {
	if (auto it = m_Fonts.find(int(m_Size.y)); it != m_Fonts.end())
	{
		m_pFont = it->second;
	}
	else
	{
		m_pFont = Env::Renderer()->GetIFont();
		m_pFont->Init("VeraMono.ttf", uint(m_Size.x), uint(m_Size.y));
		m_Fonts[int(m_Size.y)] = m_pFont;
	}

	return m_pFont;
}
