#include "CWindow.hpp"
#include <iostream>

CWindow::CWindow() : m_bClose(false) {
	
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
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    return true;
  }
  return false;
}

bool CWindow::create() {
  m_handle = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);

  if (m_handle == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    return false;
  }

  glfwSetWindowUserPointer(m_handle, static_cast<void*>(this));
  glfwSetKeyCallback(m_handle, CWindow::keyCallback);
  return true;
}

bool CWindow::update() {
  glfwMakeContextCurrent(m_handle);

  int width, height;
  glfwGetFramebufferSize(m_handle, &width, &height);

  glViewport(0, 0, width, height);
  glClearColor(1,0,1,1);

  glClear(GL_COLOR_BUFFER_BIT);
  glfwPollEvents();
	glfwSwapBuffers(m_handle);

  return m_bClose == false;
}

bool CWindow::shouldCose() {
  return m_bClose == true;
}

bool CWindow::setCallback(int key, kcbfunc f)
{
	this->kcbfuncs[key] = f;
	return false;
}

void CWindow::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
  // Когда пользователь нажимает ESC, мы устанавливаем свойство CWindowShouldClose в true, 
  // и приложение после этого закроется
  CWindow *win = reinterpret_cast<CWindow*>(glfwGetWindowUserPointer(window));
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    win->m_bClose = true;
}

HWND CWindow::get() {
  return static_cast<void*>(m_handle);
}

IWindow *CreateIWindow() {
	CWindow *win = new CWindow();
	return (win);
}
