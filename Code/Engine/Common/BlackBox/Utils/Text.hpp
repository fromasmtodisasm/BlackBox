#pragma once

inline void PrintRightAlignedText(float posY, const char* szText, IFont* pFont = nullptr)
{
	SDrawTextInfo info;
	float         rightMargin = 20;
	info.font                 = pFont;
#if 0
		auto& color = info.color;
		color[0]	= 1.0; //green
		color[1]	= 1.0;
		color[2]	= 1.0; //alpha
		color[3]	= 0.0; //red
#endif
	gEnv->pRenderer->Draw2dText(gEnv->pRenderer->GetWidth() - info.font->TextWidth(szText) - rightMargin, posY, szText, info);
}
