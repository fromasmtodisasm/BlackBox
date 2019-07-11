#include <BlackBox/Opengl.hpp>
#include <iostream>
using namespace std;


bool OpenGLLoader()
{
  GLenum err = glewInit();
  if (GLEW_OK != err)
  {
    /* Problem: glewInit failed, something is seriously wrong. */
    cerr << "Error: " << glewGetErrorString(err) << endl;
    return false;
  }
  cout << "Status: Using GLEW " <<  glewGetString(GLEW_VERSION) << endl;
  return true;
}
