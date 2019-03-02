#define GLEW_STATIC
#include "IWindow.hpp"

#include <iostream>

/*##############################################*/


/*----------------------------------------------*/


/*##############################################*/

using namespace std;

int main() {
  cout << "Main started" << endl;

  IWindow *win = CreateWindow();
  win->init();
  win->create();
  win->run(); 

  return 0;
}
