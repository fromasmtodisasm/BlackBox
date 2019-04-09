#include "Opengl.hpp"
#include <iostream>
using namespace std;


bool OpenGLLoader()
{
  if (!gladLoadGL()) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return false;
  }
  return true;
}
