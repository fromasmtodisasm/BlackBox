#pragma once
#include <BlackBox/ILog.hpp>
#include <cstdio>
#include <string>
#include <vector>
#include <varargs.h>
#include <Utils.hpp>

class NullLog : public ILog
{
public:
  NullLog();
  ~NullLog();

  // Inherited via ILog
  virtual void Clear() override;
  virtual void AddLog(const char* fmt, ...) override;
  virtual void Draw(const char* title, bool* p_open) override;
private:
  void Shutdown();
private:
  FILE* output;
  char *filename = "log.txt";
  bool inited = false;
  std::vector<std::string> log;
  char buf[2048];

  // Inherited via ILog
  virtual void Release() override;
};

NullLog::NullLog()
{
  output = fopen(filename, "w");
  if (output != nullptr)
    inited = true;
  ZeroMemory(buf, sizeof(buf));
}

NullLog::~NullLog()
{
  if (output)
  {
    Shutdown();
    fclose(output);
  }
}

void NullLog::Clear()
{
  log.clear();
}

void NullLog::AddLog(const char* fmt, ...)
{
  if (inited)
  {
    int old_size = log.size();
    va_list args;
    va_start(args, fmt);
    vsprintf(buf, fmt, args);
    log.push_back(strdup(buf));
    va_end(args);
  }
}

void NullLog::Draw(const char* title, bool* p_open)
{
}

inline void NullLog::Shutdown()
{
  for (auto& str : log)
  {
    fputs(str.c_str(), output);
  }
}

void NullLog::Release()
{
  delete this;
}
