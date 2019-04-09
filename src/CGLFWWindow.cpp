#include "CGLFWWindow.hpp"
#include <iostream>

using namespace std;

CGLFWWindow::CGLFWWindow(
    char *title, int width, int height 
  ) : 
  m_Width(width), m_Height(height), m_Title(title), m_bClose(false) 
{

}

CGLFWWindow::~CGLFWWindow() {
	glfwTerminate();
}

bool CGLFWWindow::init() {
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
    //glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    //Выключение возможности двойной буфферизации
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

    //glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    glfwWindowHint(GLFW_FOCUSED , GL_TRUE);

    //glfwWindowHint(GLFW_DECORATED, GL_FALSE);

    return true;
  }
  return false;
}

bool CGLFWWindow::create() {
  m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);

  if (m_Window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    return false;
  }

  glfwMakeContextCurrent(m_Window);

  if (!OpenGLLoader()) {
      std::cout << "Failed to initialize OpenGL context" << std::endl;
      return -1;
  }

  glfwSetWindowUserPointer(m_Window, static_cast<void*>(this));
  glfwSetKeyCallback(m_Window, CGLFWWindow::keyCallback);
  return true;
}

void CGLFWWindow::update() {
  glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
  glfwPollEvents();

  glViewport(0, 0, m_Width, m_Height);
  glClearBufferfv(GL_COLOR, 0, m_BackColor);
}

void CGLFWWindow::clear() {
  glClear(GL_COLOR_BUFFER_BIT);
}

bool CGLFWWindow::closed() {
  return glfwWindowShouldClose(m_Window);;
}

void CGLFWWindow::swap() {
  glfwSwapBuffers(m_Window);
}

void CGLFWWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  // Когда пользователь нажимает ESC, мы устанавливаем свойство m_bClose в true, 
  // и приложение после этого закроется
  CGLFWWindow *win = reinterpret_cast<CGLFWWindow*>(glfwGetWindowUserPointer(window));
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(win->m_Window, win->m_bClose = true);
  if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    win->show();
}

void CGLFWWindow::setTitle(char *title) {
  glfwSetWindowTitle(m_Window, title);
}

void CGLFWWindow::show() {
  glfwShowWindow(m_Window);
}

IWindow *CreateIWindow() {
  CGLFWWindow *win;// = new CGLFWWindow();
	return (win);
}
