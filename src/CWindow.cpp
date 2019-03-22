#include "CWindow.hpp"
#include <iostream>

using namespace std;

CWindow::CWindow(
    char *title, int width, int height 
  ) : 
  m_Width(width), m_Height(height), m_Title(title), m_bClose(false) 
{

}

CWindow::~CWindow() {
	glfwTerminate();
}

bool CWindow::init() {
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

bool CWindow::create() {
  m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);

  if (m_Window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    return false;
  }

  glfwMakeContextCurrent(m_Window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      std::cout << "Failed to initialize OpenGL context" << std::endl;
      return -1;
  }

  glfwSetWindowUserPointer(m_Window, static_cast<void*>(this));
  glfwSetKeyCallback(m_Window, CWindow::keyCallback);
  return true;
}

void CWindow::update() {
  glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
  glfwPollEvents();

  glViewport(0, 0, m_Width, m_Height);
  glClearBufferfv(GL_COLOR, 0, m_BackColor);
}

void CWindow::clear() {
  glClear(GL_COLOR_BUFFER_BIT);
}

bool CWindow::closed() {
  return glfwWindowShouldClose(m_Window);;
}

void CWindow::swap() {
  glfwSwapBuffers(m_Window);
}

void CWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  // Когда пользователь нажимает ESC, мы устанавливаем свойство m_bClose в true, 
  // и приложение после этого закроется
  CWindow *win = reinterpret_cast<CWindow*>(glfwGetWindowUserPointer(window));
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(win->m_Window, win->m_bClose = true);
  if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    win->show();
}

void CWindow::setTitle(char *title) {
  glfwSetWindowTitle(m_Window, title);
}

void CWindow::show() {
  glfwShowWindow(m_Window);
}

IWindow *CreateIWindow() {
	CWindow *win = new CWindow();
	return (win);
}
