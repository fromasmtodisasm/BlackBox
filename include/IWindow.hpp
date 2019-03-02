#pragma once

#define EXPORT _cdecl

struct IWindow {

 virtual bool create() = 0;
 virtual bool init() = 0;
 virtual void run() = 0;
};

EXPORT IWindow *CreateWindow();
