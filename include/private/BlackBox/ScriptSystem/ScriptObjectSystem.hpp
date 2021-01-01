#pragma once

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>

class CScriptObjectSystem :
  public _ScriptableEx<CScriptObjectSystem>
{
public:
  CScriptObjectSystem(ISystem* pSystem, IScriptSystem* pSS);
  ~CScriptObjectSystem();
  static void InitializeTemplate(IScriptSystem* pSS);

  void Init(IScriptSystem* pScriptSystem, ISystem* pSystem);
  int EnumDisplayFormats(IFunctionHandler* pH);
  int ScreenShot(IFunctionHandler* pH);
  int ClearConsole(IFunctionHandler* pH);
  int ShowConsole(IFunctionHandler* pH);
  int DrawImage(IFunctionHandler* pH);
  int LoadTexture(IFunctionHandler* pH);
  int Log(IFunctionHandler* pH);
  int Print(const char* text, float x, float y, float scale, float r, float g, float b, float a);

private:
  ISystem* m_pSystem;
  ILog* m_pLog;
  IRenderer* m_pRenderer;
  IConsole* m_pConsole;
  IFont* m_pFont;
};