#pragma once

#define EXPORT extern "C"

struct Rect
{
	int left;
	int top;
	int width;
	int height;
	Rect(int l, int t, int w, int h) : left(l), top(t), width(w), height(h) {}
};

typedef void* Params;

struct IWindow {
 virtual bool create(Params params) = 0;
 virtual bool init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen) = 0;
 //virtual bool init() = 0;
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
 virtual Rect &getViewPort() = 0;
 virtual void changeSize(int w, int h) = 0;
};

EXPORT IWindow* CreateIWindow();
