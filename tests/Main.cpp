#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/ILog.hpp>

#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

int main(int argc, char* argv[]) {
  string path;
  bool debug = false;
  if (argc > 1) {
    if (string(argv[1]) == "-debug")
      debug = true;
  }

  std::string cmdline;
  if (argc > 1)
  {
    cmdline += argv[1];
    for (int i = 2; i < argc; i++)
    {
      cmdline = cmdline + " " + argv[i];
    }
  }

  SSystemInitParams params;

  time_t t = time(nullptr);
  std::stringstream ss;
  ss << "logs/" << std::put_time(std::localtime(&t), "%H-%M-%S") << ".txt";
  params.sLogFileName = strdup(ss.str().c_str());

  snprintf(params.szSystemCmdLine, 512, "%s", cmdline.c_str());
  ISystem* pSystem = CreateSystemInterface(params);
  if (!pSystem->Init())
  {
    pSystem->Release();
    return EXIT_FAILURE;
  }
  pSystem->GetILog()->Log("[OK] ISystem created\n");
  pSystem->GetILog()->Log("[INFO] Current working directory: %s\n", path.c_str());
  pSystem->Start();
  pSystem->Release();

  return 0;
}
