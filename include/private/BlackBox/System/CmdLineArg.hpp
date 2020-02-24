#ifndef __CMDLINEARG_H__
#define __CMDLINEARG_H__

#if _MSC_VER > 1000
#pragma once
#endif

#include <BlackBox/System/ICmdLine.hpp>
#include <string>

class CCmdLineArg :
  public ICmdLineArg
{
public:
  CCmdLineArg(const char* name, const char* value, ECmdLineArgType type);
  virtual ~CCmdLineArg();

  const char* GetName() const;
  const char* GetValue() const;
  const ECmdLineArgType GetType() const;
  const float           GetFValue() const;
  const int             GetIValue() const;

private:

  ECmdLineArgType m_type;
  std::string     m_name;
  std::string     m_value;
};

#endif //__CMDLINEARG_H__