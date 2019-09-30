#include <BlackBox/IGame.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/Utils.hpp>
#include <BlackBox/ILog.hpp>

#include <iostream>

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

  //chdir((path = getBasePath(string(argv[0]))).c_str());
  //path = getBasePath(string(argv[0]));
  ISystem*pSystem = CreateISystem(nullptr);
  if (!pSystem->Init())
  {
    pSystem->Release();
    return EXIT_FAILURE;
  }
	pSystem->getILog()->AddLog("[OK] ISystem created\n");
	pSystem->getILog()->AddLog("[INFO] Current working directory: %s\n", path.c_str());
  pSystem->Start();

  return 0;
}
