#include "CWindow.hpp"
#include <iostream>

Window::Window()
{
    
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::init()
{
  //Инициализация GLFW
	if (glfwInit())
  {
    //Настройка GLFW
    //Задается минимальная требуемая версия OpenGL.
    //Мажорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //Минорная
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //Выключение возможности изменения размера окна
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    return true;
  }
  return false;
}

bool Window::create()
{
  m_handle = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

  if (m_handle == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    return false;
  }
  return true;
}
void Window::run()
{
  glfwMakeContextCurrent(m_handle);

  int width, height;
  glfwGetFramebufferSize(m_handle, &width, &height);

  glViewport(0, 0, width, height);
  glClearColor(0,0,0,1);

  while(!glfwWindowShouldClose(m_handle))
  {
      glClear(GL_COLOR_BUFFER_BIT);
      glfwPollEvents();
      glfwSwapBuffers(m_handle);
  }

}

IWindow *CreateWindow() {
  Window *win = new Window();
  return (win);
}
