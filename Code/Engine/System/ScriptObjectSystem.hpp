#pragma once

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>
#undef LoadImage

class CScriptObjectSystem :
    public _ScriptableEx<CScriptObjectSystem>
{
public:
	CScriptObjectSystem(ISystem* pSystem, IScriptSystem* pSS);
	~CScriptObjectSystem();
	static void InitializeTemplate(IScriptSystem* pSS);

	void        Init(IScriptSystem* pScriptSystem, ISystem* pSystem);

	//! <code>System.CreateDownload()</code>
	int         CreateDownload(IFunctionHandler* pH);

	int         EnumDisplayFormats(IFunctionHandler* pH);
	int					EnumAAFormats(IFunctionHandler* pH);
	int         ScreenShot(IFunctionHandler* pH);
	int         ClearConsole(IFunctionHandler* pH);
	int         ShowConsole(IFunctionHandler* pH);
	int         DrawImage(IFunctionHandler* pH);
	int         LoadTexture(IFunctionHandler* pH);
	int         LoadImage(IFunctionHandler* pH);
	int         Log(IFunctionHandler* pH);
	int         ScanDirectory(IFunctionHandler* pH);
	int         Print(const char* text, float x, float y, float scale, float r, float g, float b, float a);

	int         LogToConsole(IFunctionHandler* pH);
	int         ExecuteCommand(IFunctionHandler* pH);
	int         Warning(IFunctionHandler* pH);
	int         Error(IFunctionHandler* pH);
	int         IsEditor(IFunctionHandler* pH);
	int         LogAlways(IFunctionHandler* pH);

	int         GetCurrAsyncTime(IFunctionHandler* pH);
	int         GetFrameTime(IFunctionHandler* pH);
	int         IsDevModeEnable(IFunctionHandler* pH);
	int					GetEntities(IFunctionHandler* pH);

private:
	ISystem*   m_pSystem;
	ILog*      m_pLog;
	IRenderer* m_pRenderer;
	IConsole*  m_pConsole;
	IFont*     m_pFont;
	IEntitySystem* m_pEntitySystem;

};