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
	if (eventType == "windows_generic_MSG")
	{
#ifdef WIN32
		gEnv->pInput->GrabInput(true);
		MSG* winMsg = (MSG*)message;
		if (winMsg->hwnd != m_Hwnd)
			return false;
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

		//case WM_INPUT:
			CallWindowProc(m_WndPrc, m_Hwnd, winMsg->message, winMsg->wParam, winMsg->lParam);
            return true;
        case WM_INPUT:
			CallWindowProc(m_WndPrc, m_Hwnd, winMsg->message, winMsg->wParam, winMsg->lParam);
            return true;
		}
#endif
	}

	return false;
}
