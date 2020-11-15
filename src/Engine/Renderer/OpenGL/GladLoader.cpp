#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <iostream>

bool OpenGLLoader()
{
  if (!gladLoadGL()) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return false;
  }
  return true;
}