#pragma once
#include <string>
#include <vector>

namespace utils {
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

std::string wstr_to_str(const std::wstring& ws);
std::wstring str_to_wstr(std::string& str);

std::vector<std::string> split(const std::string& input, const std::string& regex);
}
