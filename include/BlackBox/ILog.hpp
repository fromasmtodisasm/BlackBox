#pragma once
#include "IMiniLog.hpp"

struct ILog : public IMiniLog
{
  virtual void Clear() = 0;
  virtual void Draw(const char* title, bool* p_open) = 0;
  virtual void Release() = 0;
};
