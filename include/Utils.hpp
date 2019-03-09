#pragma once
#ifdef _WIN32
  #include <direct.h>
  #define chdir _chdir
#else
  #include <unistd.h>
#endif
#include <string>
std::string getBasePath(std::string fullpath);
