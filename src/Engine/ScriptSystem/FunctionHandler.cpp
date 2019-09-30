#include <BlackBox/ScriptSystem/FunctionHandler.hpp>

CFunctionHandler::CFunctionHandler()
{
}

CFunctionHandler::~CFunctionHandler()
{
}

void CFunctionHandler::__Attach(HSCRIPT hScript)
{
}

THIS_PTR CFunctionHandler::GetThis()
{
	return m_ThisPtr;
}

int CFunctionHandler::GetFunctionID()
{
	return 0;
}

int CFunctionHandler::GetParamCount()
{
	return 0;
}

bool CFunctionHandler::GetParam(int nIdx, int& n)
{
	return false;
}

bool CFunctionHandler::GetParam(int nIdx, float& f)
{
	return false;
}

bool CFunctionHandler::GetParam(int nIdx, const char*& s)
{
	return false;
}

bool CFunctionHandler::GetParam(int nIdx, bool& b)
{
	return false;
}

bool CFunctionHandler::GetParam(int nIdx, IScriptObject* pObj)
{
	return false;
}

bool CFunctionHandler::GetParam(int nIdx, HSCRIPTFUNCTION& hFunc, int nReference/* = 0*/)
{
	return false;
}

ScriptVarType CFunctionHandler::GetParamType(int nIdx)
{
	return ScriptVarType();
}

int CFunctionHandler::EndFunctionNull()
{
	return 0;
}

int CFunctionHandler::EndFunction(int nRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(float fRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(const char* fRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(bool bRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(IScriptObject* pObj)
{
	return 0;
}

int CFunctionHandler::EndFunction(HSCRIPTFUNCTION hFunc)
{
	return 0;
}

int CFunctionHandler::EndFunction()
{
	return 0;
}

int CFunctionHandler::EndFunction(int nRetVal1, int nRetVal2)
{
	return 0;
}

int CFunctionHandler::EndFunction(float fRetVal1, float fRetVal2)
{
	return 0;
}

void CFunctionHandler::Unref(HSCRIPTFUNCTION hFunc)
{
}
