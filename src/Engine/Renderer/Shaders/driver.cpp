#pragma warning(push, 0)
#include <BlackBox/Core/Platform/platform_impl.inl>
#include "driver.hpp"
#include "Scanner.hpp"

#define PARSERDRIVER_EXPORTS

Driver::Driver() :
    trace_parsing(false),
    trace_scanning(false),
    scanner(new Scanner(*this)), parser(*scanner, *this) {
}


bool Driver::parse(const std::string& f) {
    file = f;
    location.initialize(&file);
    scan_begin();
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
	if (res == 0)
	{
		gEnv->pLog->Log("$3[FX] File %s passed", file.c_str());	
	}

    scan_end();
    return res == 0;
}

void Driver::scan_begin()
{
	scanner->set_debug(trace_scanning);
	if (file.empty() || file == "-")
	{
	}
	else
	{
		stream.open(file);
		if (!stream.is_open())
		{
			gEnv->pLog->LogError("[FX] File %s not found", file.c_str());
		}
		else
		{
			gEnv->pLog->Log("$3[FX] File %s opened", file.c_str());
		}
		//std::cout << file << std::endl;
		gEnv->pLog->Log(file.c_str());
		scanner->yyrestart(&stream);
	}
}

void Driver::scan_end()
{
    stream.close();
}

void Driver::Release()
{
	delete this;
}

extern "C" DLL_EXPORT IDriver* CreateParserDriver()
{
	return new Driver();
}
#pragma warning(pop)

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}

