#pragma once

struct ImGuiInputTextCallbackData;

struct IConsole
{
  virtual void    ClearLog() = 0;
  virtual void    AddLog(const char* fmt, ...) = 0;
  virtual void    Draw(const char* title, bool* p_open) = 0;
  virtual void    ExecCommand(const char* command_line) = 0;
  //virtual static int TextEditCallbackStub(ImGuiInputTextCallbackData* data) = 0;
  virtual int     TextEditCallback(ImGuiInputTextCallbackData* data) = 0;
};