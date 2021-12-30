#pragma once

class CLogFile
{
  public:
	static void WriteLine(const char* msg) { gEnv->pLog->LogToFile(msg); }

};