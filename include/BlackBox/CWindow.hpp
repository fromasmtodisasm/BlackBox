#pragma once
#ifdef SFML_WINDOW
#include "CSFMLWindow.hpp"
typedef CSFMLWindow CWindow;
#else
#include "CGLFWWindow.hpp"
typedef CGLFWWindow CWindow;
#endif // SFML_WINDOW

