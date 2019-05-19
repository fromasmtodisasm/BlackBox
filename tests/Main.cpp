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
  path = getBasePath(string(argv[0]));
  IEngine*pSystem = CreateIEngine(nullptr);
  pSystem->Init();
  IGame *game = CreateIGame("MyGame");
	pSystem->getILog()->AddLog("[OK] ISystem created\n");
	pSystem->getILog()->AddLog("[INFO] Current working directory: %s\n", path.c_str());
  if (game->init(pSystem)) {
    pSystem->getILog()->AddLog("[OK] IGame created\n");
    game->run();  
	}

  return 0;
}
