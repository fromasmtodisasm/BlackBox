#include <iostream>
#include "IGame.hpp"
#include "Utils.hpp"

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

  // chdir((path = getBasePath(string(argv[0]))).c_str());
  cout << argv[0] << endl;
  cout << path << endl;

  IGame *game = CreateIGame("MyGame");
  game->init(debug);
  game->run();  

  return 0;
}
