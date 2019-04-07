#include <iostream>
#include "IGame.hpp"
#include "ISystem.hpp"
#include "Utils.hpp"

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

#if 0//#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
int WINAPI WinMain(
      HINSTANCE hInstance,
      HINSTANCE hPrevInstance,
      PSTR szCmdLine,
      int iCmdShow) 
{
  int argc;
  char **argv;
  argv = CommandLineToArgvA(szCmdLine, &argc);
#else
int main(int argc, char *argv[]) {
#endif
  string path;
  bool debug = false;
  if (argc > 1) {
    if (string(argv[1]) == "-debug")
      debug = true;
  }

  //chdir((path = getBasePath(string(argv[0]))).c_str());
  cout << path << endl;

  ISystem*pSystem = CreateISystem(nullptr);
  IGame *game = CreateIGame("MyGame");
	cout << "ISystem created" << endl;
  if (game->init(pSystem)) {
		cout << "IGame created" << endl;
    game->run();  
	}

  return 0;
}
