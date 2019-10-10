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

namespace stl {
	//! Case insensetive less key for any type convertable to const char*.
	template<class Type>
	struct less_stricmp : public std::binary_function<Type, Type, bool>
	{
		bool operator()(const Type& left, const Type& right) const
		{
			return stricmp(constchar_cast(left), constchar_cast(right)) < 0;
		}
	};
}
