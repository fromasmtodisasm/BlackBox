#if 0
#define _LAUNCHER
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

extern "C"
{
	int AppMain(int argc, char* argv[]);
};

int main(int argc, char* argv[])
{
	int			status = EXIT_FAILURE;
	string		path;
	std::string cmdline;
	for (int i = 0; i < argc; i++)
	{
		cmdline = cmdline + " " + argv[i];
	}

	SSystemInitParams params;

	time_t			  t = time(nullptr);
	std::stringstream ss;
	ss << "logs/" << std::put_time(std::localtime(&t), "%H-%M-%S") << ".txt";
	auto logFileName	= ss.str();
	params.sLogFileName = logFileName.c_str();

	snprintf(params.szSystemCmdLine, 512, "%s", cmdline.c_str());
	ISystem* pSystem = CreateSystemInterface(params);
	if (pSystem)
	{
		pSystem->GetILog()->Log("ISystem created");
		pSystem->GetILog()->Log("Current working directory: %s", path.c_str());
		gEnv = pSystem->GetGlobalEnvironment();

		status = AppMain(argc, argv);
	}
	pSystem->Release();

	return status;
}
#endif
