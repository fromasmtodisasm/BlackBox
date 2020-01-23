#pragma once
#ifdef GUI
#include <BlackBox/System/ILog.hpp>
#include <imgui.h>

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct CLog : public ILog
{
  ImGuiTextBuffer     Buf;
  ImGuiTextFilter     Filter;
  ImVector<int>       LineOffsets;        // Index to lines offset. We maintain this with AddLog() calls, allowing us to have a random access on lines
  bool                AutoScroll;
  bool                ScrollToBottom;

  CLog()
  {
    AutoScroll = true;
    ScrollToBottom = false;
    Clear();
  }

  void    Clear();

  void    Log(const char* fmt, ...) IM_FMTARGS(2);

  void    Draw(const char* title, bool* p_open = NULL);
};
#endif