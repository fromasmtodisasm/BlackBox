
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code
//	Copyright (c) Crytek 2001-2004
//
//	File: ScriptObjectLanguage.h
//
//  Description:
//		Interface for the CScriptObjectLanguage class.
//		This class implements all language-related script-functions.
//
//		REMARKS:
//		After initialization of the script-object it will be globally accessable through scripts using the namespace "Language".
//
//		Example:
//		Language.LoadStringTable("script_table.xml");
//
//		IMPLEMENTATIONS NOTES:
//		These function will never be called from C-Code. They're script-exclusive.
//
//	History:
//	- File Created by Marco Corbetta
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTOBJECTLANGUAGE_H__A399B2EE_7076_4B23_9A77_0A2F7FEFFEAB__INCLUDED_)
	#define AFX_SCRIPTOBJECTLANGUAGE_H__A399B2EE_7076_4B23_9A77_0A2F7FEFFEAB__INCLUDED_

	#if _MSC_VER > 1000
		#pragma once
	#endif // _MSC_VER > 1000

	#include <BlackBox\ScriptSystem\_ScriptableEx.hpp>

class CStringTableMgr;

//////////////////////////////////////////////////////////////////////
class CScriptObjectLanguage :
    public _ScriptableEx<CScriptObjectLanguage>
{
public:
	//! constructor
	CScriptObjectLanguage();
	//! destructor
	virtual ~CScriptObjectLanguage();
	//!
	void        Init(IScriptSystem* pScriptSystem, CStringTableMgr* pMgr);
	//!
	void        AddString(const char* s, int nID);

	//! return -1 if string not found
	int         GetStringID(const char* szKey);
	//!
	int         LoadStringTable(IFunctionHandler* pH);
	//!
	int         GetEnglish(IFunctionHandler* pH);
	//!
	static void InitializeTemplate(IScriptSystem* pSS);

private:
	CStringTableMgr* m_pMgr; //!<
};

#endif // !defined(AFX_SCRIPTOBJECTLANGUAGE_H__A399B2EE_7076_4B23_9A77_0A2F7FEFFEAB__INCLUDED_)
