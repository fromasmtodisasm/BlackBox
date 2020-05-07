#pragma once
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdarg>
#include <BlackBox/Core/Utils.hpp>

#define strdup _strdup

class NullLog : public ILog
{
public:
  NullLog(const char* filename);
  ~NullLog();

  // Inherited via ILog
  virtual void Clear() override;
  virtual void Draw(const char* title, bool* p_open) override;
private:
  void Shutdown();
private:
  FILE* output;
  const char* filename = "log.txt";
  bool inited = false;
  std::vector<std::string> log;
  char buf[2048] = {0};

  // Inherited via ILog
  virtual void Release() override;

  // Inherited via ILog
  virtual void LogV(const ELogType nType, const char* szFormat, va_list args) override;

	// Inherited via ILog
	virtual void LogToConsole(const char* command, ...) override;
};

NullLog::NullLog(const char* filename) : filename(filename)
{
  output = fopen(filename, "w");
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

void NullLog::Draw(const char* title, bool* p_open)
{
}

inline void NullLog::Shutdown()
{
  for (auto& str : log)
  {
    fputs(str.c_str(), output);
  }
  bool hasConsole = GetISystem()->GetIConsole() != nullptr;
  if (hasConsole && GET_CVAR("stpo_running"))
  {
    fputs("\n\n*****Game stopped*****", output);
  }
  else if (hasConsole && GET_CVAR("window_closed"))
  {
    fputs("\n\n*****Window Closed*****", output);
  }
  else
  {
    fputs("\n\n*****Unknown stop cause*****", output);
  }
}

void NullLog::Release()
{
  delete this;
}

void NullLog::LogV(const ELogType nType, const char* szFormat, va_list args)
{
  vsprintf(buf, szFormat, args);
  auto len = strlen(buf);
  buf[len] = '\n';
  buf[len + 1] = '\0';
  std::cout << buf;
  log.push_back(strdup(buf));
}

void NullLog::LogToConsole(const char* command, ...)
{
}
