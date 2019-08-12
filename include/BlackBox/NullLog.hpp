#pragma once
#include <BlackBox/ILog.hpp>

class NullLog : public ILog
{
public:
  NullLog();
  ~NullLog();

  // Inherited via ILog
  virtual void Clear() override;
  virtual void AddLog(const char* fmt, ...) override;
  virtual void Draw(const char* title, bool* p_open) override;
};

NullLog::NullLog()
{
}

NullLog::~NullLog()
{
}

void NullLog::Clear()
{
}

void NullLog::AddLog(const char* fmt, ...)
{
}

void NullLog::Draw(const char* title, bool* p_open)
{
}
