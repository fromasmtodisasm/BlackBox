// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

/*************************************************************************
   -------------------------------------------------------------------------
   $Id$
   $DateTime$
   Description:	Dummy font implementation (dedicated server)
   -------------------------------------------------------------------------
   History:
   - Jun 30,2006:	Created by Sascha Demetrio

*************************************************************************/

#ifndef __NULLFONT_H__
#define __NULLFONT_H__

#pragma once
#define  USE_NULLFONT
#if defined(USE_NULLFONT)

	//#include <CryFont/IFont.h>
	#include <IFont.h>
#include <BlackBox/Renderer/IFont.hpp>
#include <codecvt>
#pragma warning(push)
#pragma warning(disable: 4244)

class CNullCryFont : public IFFont
{
public:
	CNullCryFont() {}
	virtual ~CNullCryFont() {}

	virtual void Reset() override
	{
	}
	virtual void Release() override
	{
	}
	virtual bool Load(const char* szFile, unsigned long nWidth, unsigned long nHeight, unsigned long nTTFFlags) override
	{
		return false;
	}
	virtual bool Load(const char* szFile) override
	{
		return false;
	}
	virtual void Free() override
	{
	}
	virtual void SetEffect(const char* szEffect) override
	{
	}
	virtual void SetClippingRect(float fX, float fY, float fW, float fH) override
	{
	}
	virtual void EnableClipping(bool bEnable) override
	{
	}
	virtual void SetColor(const color4f& col, int nPass = 0) override
	{
		m_Color = col;
	}
	virtual void UseRealPixels(bool bRealPixels = true) override
	{
	}
	virtual bool UsingRealPixels() override
	{
		return false;
	}
	virtual void SetSize(const vector2f& size) override
	{
		//m_Size = size;
	}
	virtual vector2f& GetSize() override
	{
		return m_Size;
	}
	virtual float GetCharWidth() override
	{
		//return 0.0f;
		return m_Size.x;
	}
	virtual float GetCharHeight() override
	{
		//return 0.0f;
		return m_Size.y;
	}
	virtual void SetSameSize(bool bSameSize) override
	{
	}
	virtual bool GetSameSize() override
	{
		return false;
	}
	virtual void SetCharWidthScale(float fScale = 1.0f) override
	{
	}
	virtual float GetCharWidthScale() override
	{
		return 0.0f;
	}
	virtual void DrawString(float x, float y, const char* szMsg, const bool bASCIIMultiLine = true) override
	{
	}
	virtual vector2f GetTextSize(const char* szMsg, const bool bASCIIMultiLine = true) override
	{
		return {m_pFont->TextWidth(szMsg), m_Size.y};
	}
	virtual void DrawStringW(float x, float y, const wchar_t* swStr, const bool bASCIIMultiLine = true) override
	{
		std::wstring ws(swStr);
		std::string str(ws.begin(), ws.end());

		m_pFont->RenderText(str.data(), x, y,1.f, m_Color.v);
	}
	virtual void DrawWrappedStringW(float x, float y, float w, const wchar_t* swStr, const bool bASCIIMultiLine = true) override
	{
	}
	virtual vector2f GetTextSizeW(const wchar_t* swStr, const bool bASCIIMultiLine = true) override
	{
		std::wstring ws(swStr);
		std::string str(ws.begin(), ws.end());
		return GetTextSize(str.data());
	}
	virtual vector2f GetWrappedTextSizeW(const wchar_t* swStr, float w, const bool bASCIIMultiLine = true) override
	{
		return vector2f();
	}
	virtual int GetTextLength(const char* szMsg, const bool bASCIIMultiLine = true) override
	{
		return 0;
	}
	virtual int GetTextLengthW(const wchar_t* szwMsg, const bool bASCIIMultiLine = true) override
	{
		return 0;
	}
	virtual void GetMemoryUsage(ICrySizer* pSizer) override
	{
	}
	vector2f m_Size = vector2f(12,12);
	color4f	 m_Color = color4f(1.f, 1.f, 1.f, 1.f);
	IFont*	 m_pFont;
};

class CCryNullFont : public ICryFont
{
public:
	virtual void    Release()                                   {}
	virtual IFFont* NewFont(const char* pFontName)              { return &ms_nullFont; }
	virtual IFFont* GetFont(const char* pFontName)		        { return &ms_nullFont; }
	virtual void    SetRendererProperties(IRenderer* pRenderer) {}
	virtual void	GetMemoryUsage(ICrySizer* pSizer)			{}
	virtual string  GetLoadedFontNames() const                  { return ""; }

//private:
	static CNullCryFont ms_nullFont;
};

#pragma warning(pop)
#endif // USE_NULLFONT

#endif
