#include <iostream>
#include "IGame.hpp"
#include "Utils.hpp"

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

int main(int argc, char *argv[]) {
  string path;
  chdir((path = getBasePath(string(argv[0]))).c_str());
  cout << argv[0] << endl;
  cout << path << endl;
  IGame *game = CreateIGame("MyGame");
  game->init();
  game->run();  

  return 0;
}
