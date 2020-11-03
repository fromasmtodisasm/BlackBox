#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <BlackBox/Core/Platform/CryLibrary.h>

#include <BlackBox/Core/Platform/platform_impl.inl>

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

int main(int argc, char* argv[]) {
	std::string cmdline;
	for (int i = 1; i < argc; i++)
	{
		cmdline = cmdline + std::string(" ") + std::string(argv[i]);
	}

  
	SSystemInitParams startupParams;
	startupParams.sLogFileName = "Game.log";

	// Note: lpCmdLine does not contain the filename.
	strcpy(startupParams.szSystemCmdLine, cmdline.c_str());

	if (InitializeEngine(startupParams))
	{
		startupParams.pSystem->Start();
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
