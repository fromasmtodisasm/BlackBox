#pragma once

struct ILog
{
  virtual void Clear() = 0;
  virtual void AddLog(const char* fmt, ...) = 0;
  virtual void Draw(const char* title, bool* p_open) = 0;
  virtual void Release() = 0;
};
