#pragma once

struct ISystem;
struct IScriptObject;
struct IFunctionHandler;

#define HSCRIPT void*
#define HSCRIPTFUNCTION unsigned int
#define THIS_PTR void*
#define HTAG int
#define USER_DATA ULONG_PTR

////////////////////////////////////////////////////////////////////////////
enum ScriptVarType
{
	svtNull = 0,
	svtString,
	svtNumber,
	svtFunction,
	svtObject,
	svtUserData,
};

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/*! main interface of the scripting engine
		this interface is mapped 1:1 on a script state
		all script loaded from the same interface instance are visible together
	*/
struct IScriptSystem
{
	/*! load a script file and run it
			@param sFileName path of the script file
			@param bRaiseError if true the script engine will call IScirptSystemSink::OnError() if
				an error will occur
			@return false if the execution fail if not true

			REMARKS: all global variables and function declared into the executed script
			will persist for all the script system lifetime
		*/
	virtual bool ExecuteFile(const char* sFileName, bool bRaiseError = true, bool bForceReload = false) = 0;
	/*! execute an ASCII buffer
			@param sBuffer an 8bit ASCII buffer containing the script that must be executed
			@param bRaiseError if true the script engine will call IScirptSystemSink::OnError() if
				an error will occur
			@return false if the execution fail if not true

			REMARKS: all global variables and function declared into the executed buffer
			will persist for all the script system lifetime
	*/
	virtual bool ExecuteBuffer(const char* sBuffer, size_t nSize) = 0;

	virtual bool GetGlobalValue(const char* sKey, int& nVal) = 0;
	virtual bool GetGlobalValue(const char* sKey, float& fVal) = 0;
	virtual bool GetGlobalValue(const char* sKey, const char*& sVal) = 0;
};

////////////////////////////////////////////////////////////////////////////
// Description:
//   This interface is used by the C++ function mapped to the script
//   to retrieve the function parameters passed by the script and
//   to return an optiona result value to the script.
#if 0
struct IFunctionHandler
{
	//DOC-IGNORE-BEGIN
	/*! internal use */
	virtual void __Attach(HSCRIPT hScript) = 0;
	/*! internal use */
	virtual THIS_PTR GetThis() = 0;
	//virtual THIS_PTR GetThis2() = 0;
	/*! internal use */
	virtual int GetFunctionID() = 0;
	//DOC-IGNORE-END

	//!	Get the number of parameter passed by lua
	virtual int GetParamCount() = 0;

	/*! get the nIdx param passed by the script
		@param nIdx 1-based index of the parameter
		@param val reference to the C++ variable that will store the value
		@param nReference should the function create strong reference to the object? By default 0, means weak reference is created
	*/
	//##@{
	virtual bool GetParam(int nIdx, int& n) = 0;
	virtual bool GetParam(int nIdx, float& f) = 0;
	virtual bool GetParam(int nIdx, const char*& s) = 0;
#if defined(WIN64) || defined(LINUX)
	inline bool GetParam(int nIdx, char*& s) { return GetParam(nIdx, (const char*&)s); }
#endif
#if defined(WIN64) || defined(LINUX64)
	virtual bool GetParam(int nIdx, INT_PTR & n) = 0;	//## AMD Port
#endif
	virtual bool GetParam(int nIdx, bool& b) = 0;
	virtual bool GetParam(int nIdx, IScriptObject* pObj) = 0;
	virtual bool GetParam(int nIdx, HSCRIPTFUNCTION& hFunc, int nReference = 0) = 0;
	virtual bool GetParam(int nIdx, USER_DATA& ud) = 0;
	virtual bool GetParamUDVal(int nIdx, USER_DATA& val, int& cookie) = 0;	//## AMD Port
//#ifdef WIN64
	inline bool GetParamUDVal(int nIdx, INT_PTR& val, int& cookie) { return GetParamUDVal(nIdx, (USER_DATA&)val, cookie); }	//## AMD Port
//#endif
	//##@}
	virtual ScriptVarType GetParamType(int nIdx) = 0;

	/*! get the return value that you must return from your "SCRIPT_FUNCTION"
		@param the value that xou want to return to the script
	*/
	//##@{
	virtual int EndFunctionNull() = 0;
	virtual int EndFunction(int nRetVal) = 0;
	virtual int EndFunction(float fRetVal) = 0;
	virtual int EndFunction(const char* fRetVal) = 0;
	virtual int EndFunction(bool bRetVal) = 0;
	virtual int EndFunction(IScriptObject* pObj) = 0;
	virtual int EndFunction(HSCRIPTFUNCTION hFunc) = 0;
	virtual int EndFunction(USER_DATA ud) = 0;
	virtual int EndFunction() = 0;

	// 2 return params versions.
	virtual int EndFunction(int nRetVal1, int nRetVal2) = 0;
	virtual int EndFunction(float fRetVal1, float fRetVal2) = 0;
	//##@}

	virtual void Unref(HSCRIPTFUNCTION hFunc) = 0;
};
#endif
