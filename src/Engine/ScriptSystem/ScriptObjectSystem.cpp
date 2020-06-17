#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectSystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectSystem)

CScriptObjectSystem::CScriptObjectSystem(ISystem* pSystem, IScriptSystem* pSS)
  :
  m_pSystem(pSystem)
{
  m_pLog = m_pSystem->GetILog();
  m_pRenderer = m_pSystem->GetIRenderer();
  m_pConsole = m_pSystem->GetIConsole();
  InitializeTemplate(pSS);
  Init(pSS, pSystem);
}

CScriptObjectSystem::~CScriptObjectSystem()
{
}

void CScriptObjectSystem::InitializeTemplate(IScriptSystem* pSS)
{
  _ScriptableEx<CScriptObjectSystem>::InitializeTemplate(pSS);

#undef SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_CLASSNAME CScriptObjectSystem

  SCRIPT_REG_FUNC(EnumDisplayFormats);
  SCRIPT_REG_FUNC(ScreenShot);
  SCRIPT_REG_FUNC(ClearConsole);
  SCRIPT_REG_FUNC(ShowConsole);
  SCRIPT_REG_FUNC(Log);

	SCRIPT_REG_FUNC(DrawImage);
	SCRIPT_REG_FUNC(LoadTexture);
}

void CScriptObjectSystem::Init(IScriptSystem* pScriptSystem, ISystem* pSystem)
{
  m_pSystem = pSystem;
  InitGlobal(pScriptSystem, "System", this);

}

#include <lua.hpp>

int CScriptObjectSystem::EnumDisplayFormats(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(0);
	if (gEnv->IsDedicated())
  {
    return pH->EndFunction();
  }
  m_pConsole->PrintLine("Enumerating display settings...");
  SmartScriptObject pDispArray(m_pSS);
  SDispFormat* Formats = NULL;
  unsigned int i;
  unsigned int numFormats = m_pRenderer->EnumDisplayFormats(NULL);
  if (numFormats)
  {
    Formats = new SDispFormat[numFormats];
    m_pRenderer->EnumDisplayFormats(Formats);
  }

  for (i = 0; i < numFormats; i++)
  {
    SDispFormat* pForm = &Formats[i];
    SmartScriptObject pDisp(m_pSS);
    pDisp->SetValue("width", pForm->m_Width);
    pDisp->SetValue("height", pForm->m_Height);
    pDisp->SetValue("bpp", pForm->m_BPP);
    pDispArray->SetAt(i + 1, pDisp);
  }

  if (numFormats == 0)        // renderer is not doing his job
  {
    {
      SmartScriptObject pDisp(m_pSS);
      pDisp->SetValue("width", 640);
      pDisp->SetValue("height", 480);
      pDisp->SetValue("bpp", 32);
      pDispArray->SetAt(1, pDisp);
    }
    {
      SmartScriptObject pDisp(m_pSS);
      pDisp->SetValue("width", 800);
      pDisp->SetValue("height", 600);
      pDisp->SetValue("bpp", 32);
      pDispArray->SetAt(2, pDisp);
    }
    {
      SmartScriptObject pDisp(m_pSS);
      pDisp->SetValue("width", 1024);
      pDisp->SetValue("height", 768);
      pDisp->SetValue("bpp", 32);
      pDispArray->SetAt(3, pDisp);
    }
  }

  if (Formats)
    delete[] Formats;
  return pH->EndFunction(pDispArray);
}

int CScriptObjectSystem::ScreenShot(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(1);
  const char* filename;

  pH->GetParam(1, filename);
  m_pRenderer->ScreenShot(filename);
  return pH->EndFunction();
}

int CScriptObjectSystem::ClearConsole(IFunctionHandler* pH)
{
  m_pConsole->Clear();
  return pH->EndFunction();
}

int CScriptObjectSystem::ShowConsole(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(1);
  int show;

  pH->GetParam(1, show);
  m_pConsole->ShowConsole(show);
  return pH->EndFunction();
}

int CScriptObjectSystem::DrawImage(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(6);
	int id;
	int xpos;
	int ypos;
	int w;
	int h;
	int blending_mode;
	pH->GetParams(id, xpos, ypos, w, h, blending_mode);
	gEnv->pRenderer->DrawImage(xpos, ypos, w, h, id, 0, 0, 1, 1, 0, 1, 0, 1);
	return pH->EndFunction();
}

int CScriptObjectSystem::LoadTexture(IFunctionHandler* pH)
{
	const char* name;
	if (pH->GetParams(name))
	{
		auto t = gEnv->pRenderer->LoadTexture(name, 0, 0);
		if (t != nullptr)
		{
			return pH->EndFunction(t->getId());
    }
  }
	return pH->EndFunctionNull();
}

int CScriptObjectSystem::Log(IFunctionHandler* pH)
{
  SCRIPT_CHECK_PARAMETERS(1);
	const char* string;

  pH->GetParam(1, string);
	m_pSystem->Log(string);
  m_pConsole->PrintLine(string);
  return pH->EndFunction();
}
