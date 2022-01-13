#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/System/System.hpp>
//#include <CrySystem/SystemInitParams.h>
//#include "DebugCallStack.h"
//#include "MemReplay.h"

LONG WINAPI MyUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionPtrs)
{
	static char msg[1024];

	CryError(
		"Code: %x\nAddress: 0x%p",
		pExceptionPtrs->ExceptionRecord->ExceptionCode,
		pExceptionPtrs->ExceptionRecord->ExceptionAddress);
	// Do something, for example generate error report
	MessageBox(NULL, msg, "Exception", 0);
	//..

	// Execute default exception handler next
	return EXCEPTION_EXECUTE_HANDLER;
}

ISystem* CreateSystemInterface(SSystemInitParams& startupParams)
{
	std::unique_ptr<CSystem> pSystem = std::make_unique<CSystem>(startupParams);
	startupParams.pSystem			 = pSystem.get();
	ModuleInitISystem(pSystem.get(), "System");
#if BB_PLATFORM_WINDOWS
	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
#endif

#if CRY_PLATFORM_DURANGO
#	if !defined(_LIB)
	m_env = pSystem->GetGlobalEnvironment();
#	endif
	m_env.pWindow = startupParams.hWnd;
#endif
	// the earliest point the system exists - w2e tell the callback
	if (startupParams.pUserCallback)
		startupParams.pUserCallback->OnSystemConnect(pSystem.get());

	if (!pSystem->Init())
	{
		CryMessageBox("System initialization failed!", "Engine initialization failed!");
		pSystem.release();
		startupParams.pSystem = nullptr;
		gEnv->pSystem		  = nullptr;

		return nullptr;
	}
	pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_SYSTEM_INIT_DONE, 0, 0);
	// run main loop
	if (!startupParams.bManualEngineLoop)
	{
		pSystem->RunMainLoop();
		return nullptr;
	}

	return pSystem.release();
}
