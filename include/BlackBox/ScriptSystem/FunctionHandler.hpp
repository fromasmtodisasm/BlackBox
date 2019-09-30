#pragma once
#include <BlackBox/IScriptSystem.hpp>

#include <lua.hpp>

class CFunctionHandler : public IFunctionHandler
{
public:
	CFunctionHandler();
	~CFunctionHandler();

	// Унаследовано через IFunctionHandler
	virtual void __Attach(HSCRIPT hScript) override;

	virtual THIS_PTR GetThis() override;

	virtual int GetFunctionID() override;

	virtual int GetParamCount() override;

	virtual bool GetParam(int nIdx, int& n) override;

	virtual bool GetParam(int nIdx, float& f) override;

	virtual bool GetParam(int nIdx, const char*& s) override;

	virtual bool GetParam(int nIdx, bool& b) override;

	virtual bool GetParam(int nIdx, IScriptObject* pObj) override;

	virtual bool GetParam(int nIdx, HSCRIPTFUNCTION& hFunc, int nReference = 0) override;

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

private:
	lua_State* L;
	THIS_PTR m_ThisPtr;

};
