#pragma once

struct IWindow;

struct IPlatform
{
	virtual bool Init() = 0;
	virtual IWindow* CreateWindow(const char* name, int w, int h) = 0;
	virtual IWindow* GetMainWindow() = 0;
	virtual void MoveMouseGlobal(int x, int y) = 0;
	virtual void MoveMouseRelative(IWindow* window, int x, int y) = 0;
	virtual void Update() = 0;
};