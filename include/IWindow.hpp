#pragma once

#define EXPORT _cdecl
typedef void * HWND;
struct IWindow {
 virtual bool create() = 0;
 virtual bool init() = 0;
 virtual bool update() = 0;
 virtual bool shouldCose() = 0;
 virtual HWND get() = 0;
};

IWindow* EXPORT CreateIWindow();
