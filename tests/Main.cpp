#include "IGame.hpp"
#include <iostream>

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

int main() {
  IGame *game = CreateIGame("MyGame");
  game->init();
  game->run();  

  return 0;
}
