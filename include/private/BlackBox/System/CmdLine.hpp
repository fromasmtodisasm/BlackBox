#ifndef __CMDLINE_H__
#define __CMDLINE_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <BlackBox/System/ICmdLine.hpp>
#include <BlackBox/System/CmdLineArg.hpp>

#include <string>
#include <vector>

class CCmdLine :
  public ICmdLine
{
public:
  CCmdLine(const char* commandLine);
  virtual ~CCmdLine();

  virtual const ICmdLineArg* GetArg(int n) const;
  virtual int                GetArgCount() const;
  virtual const ICmdLineArg* FindArg(const ECmdLineArgType ArgType, const char* name, bool caseSensitive = false) const;
  virtual const char* GetCommandLine() const { return m_sCmdLine.c_str(); };

private:
  void   PushCommand(const std::string& sCommand, const std::string& sParameter);
  std::string Next(char*& str);

  std::string              m_sCmdLine;
  std::vector<CCmdLineArg> m_args;
};

#endif //__CMDLINE_H__