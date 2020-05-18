#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Core/Utils.hpp>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define DS "\\" //dir separator
#else
#define DS "/" //dir separator
#endif

std::string getBasePath(std::string fullpath) {
  size_t pos = fullpath.find_last_of(DS);
  return fullpath.substr(0, pos);
}

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable : 4267)
PCHAR*
CommandLineToArgvA(
  PCHAR CmdLine,
  int* _argc
)
{
  PCHAR* argv;
  PCHAR  _argv;
  ULONG   len;
  ULONG   argc;
  CHAR   a;
  ULONG   i, j;

  BOOLEAN  in_QM;
  BOOLEAN  in_TEXT;
  BOOLEAN  in_SPACE;

  PCHAR szFileName = new CHAR[MAX_PATH + 1];
  GetModuleFileName(NULL, szFileName, MAX_PATH + 1);

  len = strlen(CmdLine) + strlen(szFileName);
  i = ((len + 2) / 2) * sizeof(PVOID) + sizeof(PVOID);
#pragma warning(push)
#pragma warning(disable : 26451)
  argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
    i + (len + 2) * sizeof(CHAR));
#pragma warning(pop)
  if (argv == nullptr)
    return nullptr;

  _argv = (PCHAR)(((PUCHAR)argv) + i);

  argc = 0;
  argv[argc] = szFileName;
  argc = 1;
  in_QM = FALSE;
  in_TEXT = FALSE;
  in_SPACE = TRUE;
  i = 0;
  j = 0;

  while (a = CmdLine[i]) {
    if (in_QM) {
      if (a == '\"') {
        in_QM = FALSE;
      }
      else {
        _argv[j] = a;
        j++;
      }
    }
    else {
      switch (a) {
      case '\"':
        in_QM = TRUE;
        in_TEXT = TRUE;
        if (in_SPACE) {
          argv[argc] = _argv + j;
          argc++;
        }
        in_SPACE = FALSE;
        break;
      case ' ':
      case '\t':
      case '\n':
      case '\r':
        if (in_TEXT) {
          _argv[j] = '\0';
          j++;
        }
        in_TEXT = FALSE;
        in_SPACE = TRUE;
        break;
      default:
        in_TEXT = TRUE;
        if (in_SPACE) {
          argv[argc] = _argv + j;
          argc++;
        }
        _argv[j] = a;
        j++;
        in_SPACE = FALSE;
        break;
      }
    }
    i++;
  }
  _argv[j] = '\0';
  argv[argc] = NULL;

  (*_argc) = argc;
  return argv;
}
#pragma warning(pop)
#endif

DLL_EXPORT std::string wstr_to_str(const std::wstring& ws)
{
  std::string result;
  result.resize(ws.length());
  auto r_it = result.begin();
  for (auto ch : ws)
  {
    std::wctomb(&r_it++[0], ch);
    //result += tmp;
  }
  return result;
}

DLL_EXPORT std::wstring str_to_wstr(const std::string& str)
{
  std::wstring result;
  if (str.length() == 0)
    return L"";
  result.resize(str.length());
  auto r_it = result.begin();
  std::mbstowcs(&r_it++[0], str.data(), str.length());
  return result;
}

DLL_EXPORT void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
  // Get the first occurrence
  size_t pos = data.find(toSearch);

  // Repeat till end is reached
  while (pos != std::string::npos)
  {
    // Replace this occurrence of Sub String
    data.replace(pos, toSearch.size(), replaceStr);
    // Get the next occurrence from the current position
    pos = data.find(toSearch, pos + replaceStr.size());
  }
}

DLL_EXPORT void findAndReplaceAll(std::string& data, std::string toSearch, std::function<std::string(int)> replaceStr)
{
  // Get the first occurrence
  size_t pos = data.find(toSearch);

  // Repeat till end is reached
  int n = 0;
  while (pos != std::string::npos)
  {
    // Replace this occurrence of Sub String
    if ((pos + 1) < data.length() && isdigit(data[pos + 1]))
    {
      data.replace(pos, toSearch.size() + 1, replaceStr(data[pos + 1] - '0'));
      n++;
    }
    else
    {
      data.replace(pos, toSearch.size(), "");
    }
    // Get the next occurrence from the current position
    pos = data.find(toSearch, ++pos);
  }
}

DLL_EXPORT void urldecode2(char* dst, const char* src)
{
    char a, b;
    while (*src) {
        if ((*src == '%') &&
            ((a = src[1]) && (b = src[2])) &&
            (isxdigit(a) && isxdigit(b))) {
            if (a >= 'a')
                a -= 'a' - 'A';
            if (a >= 'A')
                a -= ('A' - 10);
            else
                a -= '0';
            if (b >= 'a')
                b -= 'a' - 'A';
            if (b >= 'A')
                b -= ('A' - 10);
            else
                b -= '0';
            *dst++ = 16 * a + b;
            src += 3;
        }
        else if (*src == '+') {
            *dst++ = ' ';
            src++;
        }
        else {
            *dst++ = *src++;
        }
    }
    *dst++ = '\0';
}
