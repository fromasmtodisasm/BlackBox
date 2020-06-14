#pragma once

#ifdef IWINDOW_EXPORTS
  #define IWINDOW_API DLL_EXPORT
#else
  #define IWINDOW_API DLL_IMPORT
#endif

struct Rect
{
  int left;
  int top;
  int width;
  int height;
  Rect(int l, int t, int w, int h) : left(l), top(t), width(w), height(h) {}
  Rect() : left(0), top(0), width(0), height(0) {}
};

typedef void* Params;
typedef struct Cursor Cursor;
typedef void* GLContext;
typedef void* DisplayMode;

struct IWindow {
  virtual void Release() = 0;
  virtual bool create(Params params) = 0;
  virtual bool init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen) = 0;
  //virtual bool Init() = 0;
  virtual void update() = 0;
  virtual void clear() = 0;
  virtual bool closed() = 0;
  virtual void swap() = 0;
  virtual void setTitle(const char*) = 0;
  virtual void show() = 0;
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
  virtual void* getHandle() = 0;
  virtual void setFlags(int flags) = 0;
  virtual Rect& getViewPort() = 0;
  virtual void changeSize(int w, int h) = 0;
  virtual void setCursor(Cursor* cursor) = 0;
  virtual GLContext getContext() = 0;
  virtual DisplayMode GetDesktopMode()		 = 0;
  virtual void EnterFullscreen(bool mode) = 0;
};

extern "C" {
  IWINDOW_API IWindow* CreateIWindow();
  typedef IWindow* (*PFNCREATEWINDOW)();
}
