// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

// -------------------------------------------------------------------------
//  File name:   HTTPDownloader.h
//  Version:     v1.00
//  Created:     24/9/2004.
//  Compilers:   Visual Studio.NET 2003
//  Description:
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#if BB_PLATFORM_WINDOWS
	#include <BlackBox/Core/Platform/Windows.hpp>
	#include <wininet.h> // requires <windows.h>
	#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
	#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>
	#include <dbghelp.h>

	#define HTTP_BUFFER_SIZE (16384)

enum
{
	HTTP_STATE_WORKING = 0,
	HTTP_STATE_COMPLETE,
	HTTP_STATE_CANCELED,
	HTTP_STATE_ERROR,
	HTTP_STATE_NONE,
};

class CDownloadManager;

class CHTTPDownloader : public _ScriptableEx<CHTTPDownloader>
{
public:
	CHTTPDownloader();
	virtual ~CHTTPDownloader();

	static void   InitializeTemplate(IScriptSystem *pSS);

	int           Create(ISystem* pISystem, CDownloadManager* pParent);
	int           Download(const char* szURL, const char* szDestination);
	void          Cancel();
	int           GetState()             { return m_iState; };
	int           GetFileSize() const    { return m_iFileSize; };
	const string& GetURL() const         { return m_szURL; };
	const string& GetDstFileName() const { return m_szDstFile; };
	void          Release();

	int           Download(IFunctionHandler* pH);
	int           Cancel(IFunctionHandler* pH);
	int           Release(IFunctionHandler* pH);

	int           GetURL(IFunctionHandler* pH);
	int           GetFileSize(IFunctionHandler* pH);
	int           GetFileName(IFunctionHandler* pH);

	void          OnError();
	void          OnComplete();
	void          OnCancel();

	IScriptObject* GetScriptObject() { return m_pScriptThis; };
private:

	static DWORD   DownloadProc(CHTTPDownloader* _this);
	void           CreateDownloadThread();
	DWORD          DoDownload();
	void           PrepareBuffer();
	//IScriptObject* GetScriptObject() { return m_pScriptThis; };

	string            m_szURL;
	string            m_szDstFile;
	HANDLE            m_hThread;
	HINTERNET         m_hINET;
	HINTERNET         m_hUrl;

	unsigned char*    m_pBuffer;
	int               m_iFileSize;

	volatile int      m_iState;
	volatile bool     m_bContinue;

	ISystem*          m_pSystem;
	CDownloadManager* m_pParent;
};

#endif
