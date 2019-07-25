#pragma once
#include <string>
#ifdef _WIN32
  
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #include <direct.h>
  #define chdir _chdir
  PCHAR*
  CommandLineToArgvA(
      PCHAR CmdLine,
      int* _argc
      ); 
#else
  #include <unistd.h>
#endif
#include <string>
std::string getBasePath(std::string fullpath);

std::string wstr_to_str(std::wstring& ws);
