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

DLL_EXPORT std::string wstr_to_str(const std::wstring& ws);
DLL_EXPORT std::wstring str_to_wstr(const std::string& str);

DLL_EXPORT void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr);
DLL_EXPORT void findAndReplaceAll(std::string& data, std::string toSearch, std::function<std::string(int)> replaceStr);

DLL_EXPORT void urldecode2(char* dst, const char* src);

namespace stl {
  template<class Type>
  //! Case insensetive less key for any type convertable to const char*.
  struct less_stricmp {
    bool operator()(const Type& left, const Type& right) const {
      return _stricmp(left.c_str(), right.c_str()) < 0;
    }
  };
}