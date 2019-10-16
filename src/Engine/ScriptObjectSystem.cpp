#include <BlackBox/ScriptObjectSystem.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/IConsole.hpp>

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
	m_pRenderer = m_pSystem->GetIRender();
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
}

void CScriptObjectSystem::Init(IScriptSystem* pScriptSystem, ISystem* pSystem)
{
	m_pSystem = pSystem;
	InitGlobal(pScriptSystem, "System", this);
}

#include <lua.hpp>

int CScriptObjectSystem::EnumDisplayFormats(IFunctionHandler* pH)
{
	//SCRIPT_CHECK_PARAMETERS(0);
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
