#include "IGame.hpp"
#include <iostream>

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

int main() {
  cout << "Main started" << endl;

  IGame *game = CreateIGame();
  game->init();
  game->run();  

  return 0;
}
