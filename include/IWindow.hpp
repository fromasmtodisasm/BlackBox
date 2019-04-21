#pragma once

#define EXPORT extern "C"

struct IWindow {
 virtual bool create() = 0;
 virtual bool init() = 0;
 virtual void update() = 0;
 virtual void clear() = 0;
 virtual bool closed() = 0;
 virtual void swap() = 0;
 virtual void setTitle(const char *) = 0;
 virtual void show() = 0;
 virtual int getWidth() = 0;
 virtual int getHeight() = 0;
 virtual void *getHandle() = 0;
 virtual void setFlags(int flags) = 0;
};

EXPORT IWindow* CreateIWindow();
