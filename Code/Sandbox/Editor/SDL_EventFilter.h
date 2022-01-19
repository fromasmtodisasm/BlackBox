#pragma once
#include <QAbstractNativeEventFilter>
#include <QByteArray>
#include <Windows.h>

class SDL_EventFilter : public QAbstractNativeEventFilter
{
public:
	SDL_EventFilter(HWND hwnd);

	// Inherited via QAbstractNativeEventFilter
	virtual bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

	HWND m_Hwnd;
	WNDPROC m_WndPrc;
};