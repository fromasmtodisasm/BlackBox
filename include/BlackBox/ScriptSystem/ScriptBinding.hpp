#pragma once

#include <vector>
#include <memory>

struct ISystem;
struct IScriptSystem;

struct ScriptBase;

//////////////////////////////////////////////////////////////////////////
class CScriptBindings
{
public:
  CScriptBindings();
  virtual ~CScriptBindings();

  void         Init(ISystem* pSystem, IScriptSystem* pSS);
  void         Done();

private:
  std::vector<std::unique_ptr<ScriptBase>> m_binds;
};