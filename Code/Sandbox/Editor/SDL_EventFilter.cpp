#include "SDL_EventFilter.h"
#include <BlackBox/Input/IInput.hpp>

// Inherited via QAbstractNativeEventFilter

SDL_EventFilter::SDL_EventFilter(HWND hwnd)
	: m_Hwnd(hwnd)
	, m_WndPrc((WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC))
{
	
}

bool SDL_EventFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
{
	//if (eventType == "windows_generic_MSG")
	{
#ifdef WIN32
		//gEnv->pInput->GrabInput(true);
		MSG* winMsg = (MSG*)message;
		// This is wrong condition!!! to be more precise, the passed window handle is incorrect
		//TODO: Understand which window handle should be passed to this filter
		//if (winMsg->hwnd != m_Hwnd)
		//	return false;
		switch (winMsg->message)
		{
		// whitelist some events as user events
		case WM_KEYDOWN:
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MBUTTONUP:
		case WM_KEYUP:
		case WM_MOUSEWHEEL:
        case WM_INPUT:
		case WM_CHAR:
			CallWindowProc(m_WndPrc, m_Hwnd, winMsg->message, winMsg->wParam, winMsg->lParam);
            return false;
		}
#endif
	}

	return false;
}
