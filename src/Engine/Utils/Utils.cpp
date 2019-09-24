#include <BlackBox/Utils.hpp>
#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
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
        i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);
#pragma warning(push)
#pragma warning(disable : 26451)
        argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
            i + (len+2)*sizeof(CHAR));
#pragma warning(pop)
        if (argv == nullptr)
          return nullptr;

        _argv = (PCHAR)(((PUCHAR)argv)+i);

        argc = 0;
        argv[argc] = szFileName;
        argc = 1;
        in_QM = FALSE;
        in_TEXT = FALSE;
        in_SPACE = TRUE;
        i = 0;
        j = 0;

        while( a = CmdLine[i] ) {
            if(in_QM) {
                if(a == '\"') {
                    in_QM = FALSE;
                } else {
                    _argv[j] = a;
                    j++;
                }
            } else {
                switch(a) {
                case '\"':
                    in_QM = TRUE;
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    in_SPACE = FALSE;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if(in_TEXT) {
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = FALSE;
                    in_SPACE = TRUE;
                    break;
                default:
                    in_TEXT = TRUE;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
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


std::string wstr_to_str(const std::wstring& ws)
{
	std::string result;
	result.resize(ws.length());
	auto r_it = result.begin();
	char tmp[6] = { 0 };
	for (auto ch : ws)
	{
		std::wctomb(&r_it++[0], ch);
		//result += tmp;
		
	}
	return result;
}

std::wstring str_to_wstr(const std::string& str)
{
	std::wstring result;
	if (str.length() == 0)
		return L" ";
	result.resize(str.length());
	auto r_it = result.begin();
	char tmp[6] = { 0 };
	std::mbstowcs(&r_it++[0], str.data(), str.length());
	return result;

}
