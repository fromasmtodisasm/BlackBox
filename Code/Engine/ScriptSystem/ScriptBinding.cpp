#include "pch.hpp"
#include "ScriptBinding.hpp"
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>
#if 0
	#include "ScriptObjectSystem.hpp"
#endif

CScriptBindings::CScriptBindings()
{
}

CScriptBindings::~CScriptBindings()
{
	Done();
}

//////////////////////////////////////////////////////////////////////////
void CScriptBindings::Init(ISystem* pSystem, IScriptSystem* pSS)
{
#if 0
  m_binds.push_back(std::unique_ptr<CScriptObjectSystem>((new CScriptObjectSystem(pSystem, pSS))));
#endif
}

//////////////////////////////////////////////////////////////////////////
void CScriptBindings::Done()
{
	// Done script bindings.
	m_binds.clear();
}
