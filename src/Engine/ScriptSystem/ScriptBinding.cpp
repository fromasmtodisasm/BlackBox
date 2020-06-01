#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/ScriptSystem/ScriptBinding.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectSystem.hpp>

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
  m_binds.push_back(std::unique_ptr<CScriptObjectSystem>((new CScriptObjectSystem(pSystem, pSS))));
}

//////////////////////////////////////////////////////////////////////////
void CScriptBindings::Done()
{
  // Done script bindings.
  m_binds.clear();
}
