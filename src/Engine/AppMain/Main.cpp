#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

extern "C"
{
	int AppMain(int argc, char* argv[]);
};

int main(int argc, char* argv[]) {
  int status = EXIT_FAILURE;
  string path;
  std::string cmdline;
  for (int i = 0; i < argc; i++)
  {
    cmdline = cmdline + " " + argv[i];
  }

  SSystemInitParams params;

  std::cout << "Current path is " << fs::current_path() << '\n';

  time_t t = time(nullptr);
  std::stringstream ss;
  ss << "logs/" << std::put_time(std::localtime(&t), "%H-%M-%S") << ".txt";
  params.sLogFileName = strdup(ss.str().c_str());

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
