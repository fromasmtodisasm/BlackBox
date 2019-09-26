#pragma once


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