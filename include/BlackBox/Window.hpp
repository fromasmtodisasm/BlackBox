#pragma once
#ifdef SFML_WINDOW
#include "SFMLWindow.hpp"
typedef CSFMLWindow CWindow;
#else
#include "GLFWWindow.hpp"
typedef CGLFWWindow CWindow;
#endif // SFML_WINDOW

