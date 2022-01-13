#pragma once
#ifndef QTWM_DLL
	#define QTWM_DLL
#endif // ! QTWM_DLL

#if defined(_WIN32) && defined(QTWM_DLL)
	#ifdef QToolWindowManager_EXPORTS
		#define QTOOLWINDOWMANAGER_EXPORT __declspec(dllexport)
	#else
		#define QTOOLWINDOWMANAGER_EXPORT __declspec(dllimport)
	#endif
#else
	#define QTOOLWINDOWMANAGER_EXPORT
#endif
