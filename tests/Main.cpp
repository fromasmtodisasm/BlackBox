#include <iostream>
#include "IGame.hpp"
#include "Utils.hpp"

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

int main(int argc, char *argv[]) {
  chdir(getBasePath(string(argv[0])).c_str());
  IGame *game = CreateIGame("MyGame");
  game->init();
  game->run();  

  return 0;
}
