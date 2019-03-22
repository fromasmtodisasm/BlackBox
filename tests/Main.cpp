#include <iostream>
#include "IGame.hpp"
#include "Utils.hpp"

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

#if 0//#ifdef WIN32
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

  chdir((path = getBasePath(string(argv[0]))).c_str());
  cout << path << endl;

  IGame *game = CreateIGame("MyGame");
  game->init(debug);
  game->run();  

  return 0;
}
