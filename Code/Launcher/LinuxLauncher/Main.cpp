#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Core/smartptr.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>

#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>

int main(int argc, char* argv[]) {
	std::string cmdline;
	for (int i = 1; i < argc; i++)
	{
		cmdline = cmdline + std::string(" ") + std::string(argv[i]);
	}


	std::cerr << "cmd: " << cmdline << std::endl;
	try
	{
#if 1
		std::cout << "Here" << std::endl;
		SSystemInitParams startupParams;
		startupParams.sLogFileName = "Game.log";

		// Note: lpCmdLine does not contain the filename.
		strcpy(startupParams.szSystemCmdLine, cmdline.c_str());

		if (InitializeEngine(startupParams))
		{
			startupParams.pSystem->Start();
			return EXIT_SUCCESS;
		}
		else
		{
			std::cout << "BAAAAAAAAD\n" << std::endl;
		}
#endif

		std::cout << "Here" << std::endl;
		return EXIT_FAILURE;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	catch(...)
	{

		std::cout << "All bad" << std::endl;
		return EXIT_FAILURE;
	}
  
}
