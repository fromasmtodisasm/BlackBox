#pragma once

#define SYSTEM_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/CmdLineArg.hpp>
#include <BlackBox/System/HardwareMouse.hpp>

#include <cassert>

#ifdef BB_PLATFORM_LINUX
#include <pwd.h>
#include <sys/types.h>
#endif

#ifndef BB_PLATFORM_LINUX
#include <filesystem>
namespace fs = std::filesystem;
#else
//#include <experimental/filesystem>
//namespace fs = std::experimental::filesystem;
#endif

