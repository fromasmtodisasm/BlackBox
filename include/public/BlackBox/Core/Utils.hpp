#pragma once
#include <string>
#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
//#include <windows.h>
//#include <direct.h>
/*#define chdir _chdir
PCHAR*
CommandLineToArgvA(
    PCHAR CmdLine,
    int* _argc
    );
*/
#else
#include <unistd.h>
#endif
#include <string>
std::string getBasePath(std::string fullpath);

std::string wstr_to_str(const std::wstring& ws);
std::wstring str_to_wstr(const std::string& str);

void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr);
void findAndReplaceAll(std::string& data, std::string toSearch, std::function<std::string(int)> replaceStr);

namespace stl {
  template<class Type>
  //! Case insensetive less key for any type convertable to const char*.
  struct less_stricmp {
    bool operator()(const Type& left, const Type& right) const {
      return _stricmp(left.c_str(), right.c_str()) < 0;
    }
  };
}