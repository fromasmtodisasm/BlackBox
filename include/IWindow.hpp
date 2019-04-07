#pragma once

#define EXPORT _cdecl
struct IWindow {
 virtual bool create() = 0;
 virtual bool init() = 0;
 virtual void update() = 0;
 virtual void clear() = 0;
 virtual bool closed() = 0;
 virtual void swap() = 0;
 virtual void setTitle(char *) = 0;
 virtual void show() = 0;
 virtual void *getHandle() = 0;
};

IWindow* EXPORT CreateIWindow();
