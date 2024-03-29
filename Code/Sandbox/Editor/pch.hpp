#pragma once

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Project/ProjectDefines.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>

#define USE_PCH
#ifdef USE_PCH
// Shell Extensions.
//#include <Shlwapi.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <chrono>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <codecvt>
#include <complex>
#include <condition_variable>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <deque>
#include <exception>
#include <forward_list>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <valarray>
#include <vector>
#endif // USE_PCH

// Resource includes
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////
// Main Editor include.
//////////////////////////////////////////////////////////////////////////
#include "EditorDefs.h"
#include "IPlugin.h"
#include "PluginManager.h"

using CString = string;

#define CRY_PROFILE_FUNCTION(p)
#define CRY_PROFILE_SECTION_ARG(...)

#define cry_strcpy strcpy
#define cry_strcat strcat

#include <QString>
#include <BlackBox/Math/MathHelper.hpp>
namespace QtUtil
{
	inline string ToString(QString str)
	{
		return str.toStdString();
	}
} // namespace QtUtil

