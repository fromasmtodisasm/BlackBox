#include <BlackBox/IGame.hpp>
#include <BlackBox/ISystem.hpp>
//#include <BlackBox/Utils.hpp>
#include <BlackBox/ILog.hpp>

#include <iostream>
#include <filesystem>
#include <sstream>

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

int main(int argc, char *argv[]) {
  string path;
  bool debug = false;
  if (argc > 1) {
    if (string(argv[1]) == "-debug")
      debug = true;
  }

	//std::filesystem::current_path("../../");
	cout << "current path: " << std::filesystem::current_path() << endl;

  //chdir((path = getBasePath(string(argv[0]))).c_str());
  //path = getBasePath(string(argv[0]));
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

	std::time_t t = std::time(nullptr);
	std::stringstream ss;
	ss << "logs/" << std::put_time(std::localtime(&t), "%H-%M-%S") << ".txt";
	params.sLogFileName = _strdup(ss.str().c_str());

	snprintf(params.szSystemCmdLine, 512, "%s", cmdline.c_str());
  ISystem*pSystem = CreateSystemInterface(params);
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
