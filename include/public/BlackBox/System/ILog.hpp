#pragma once
#include "IMiniLog.hpp"

struct ILog : public IMiniLog
{
	//set the file used to log to disk
	virtual void	SetFileName(const char *command = NULL) = 0;
	//
	virtual const char*	GetFileName() = 0;

	//log to the file specified in setfilename
  virtual void	LogToFile(const char *command,...) = 0;	

  virtual void Clear() = 0;
  virtual void Draw(const char* title, bool* p_open) = 0;
  virtual void Release() = 0;

	//log to console only
	virtual void	LogToConsole(const char *command,...) = 0;
};