#pragma once
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>

class CFunctionHandler : public IFunctionHandler
{
public:
  CFunctionHandler(CScriptSystem* pSS, lua_State* L);
  ~CFunctionHandler();

  virtual void __Attach(HSCRIPT hScript) override;

  virtual THIS_PTR GetThis() override;

  virtual INT_PTR GetFunctionID() override;

  virtual int GetParamCount() override;

  virtual bool GetParam(int nIdx, int& n) override;

  virtual bool GetParam(int nIdx, float& f) override;

  virtual bool GetParam(int nIdx, const char*& s) override;

  virtual bool GetParam(int nIdx, bool& b) override;

  virtual bool GetParam(int nIdx, IScriptObject* pObj) override;

  virtual bool GetParam(int nIdx, HSCRIPTFUNCTION& hFunc, int nReference = 0) override;

  virtual bool GetParam(int nIdx, USER_DATA& ud) override;

  template<typename T> bool GetParamAny(int nIdx, T& val)
  {
		int nRealIdx = nIdx + m_paramIdOffset;
    bool res = m_pSS->ToAny(val, nRealIdx);
    return res;
  }

  virtual ScriptVarType GetParamType(int nIdx) override;

  virtual int EndFunctionNull() override;

  virtual int EndFunction(int nRetVal) override;

  virtual int EndFunction(float fRetVal) override;

  virtual int EndFunction(const char* fRetVal) override;

  virtual int EndFunction(bool bRetVal) override;

  virtual int EndFunction(IScriptObject* pObj) override;

  virtual int EndFunction(HSCRIPTFUNCTION hFunc) override;

  virtual int EndFunction() override;

  virtual int EndFunction(int nRetVal1, int nRetVal2) override;

  virtual int EndFunction(float fRetVal1, float fRetVal2) override;

  virtual void Unref(HSCRIPTFUNCTION hFunc) override;

  virtual bool GetParamUDVal(int nIdx, USER_DATA& val, int& cookie) override;

  virtual int EndFunction(USER_DATA ud) override;

private:
  lua_State* L;
  THIS_PTR				m_ThisPtr = nullptr;
  CScriptSystem* m_pSS = nullptr;
  const char* m_sFuncName;
  int							m_paramIdOffset = 1; // on first place __this
};