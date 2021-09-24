#pragma once

#define SYSTEM_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>
#include <BlackBox/System/CmdLineArg.hpp>
#include <BlackBox/System/Console.hpp>
#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/System/HardwareMouse.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ITimer.hpp>
#include <BlackBox/System/IWindow.hpp>

#ifndef LINUX
#	include <BlackBox/System/File/CryPak.hpp>
#endif

#include <BlackBox/Core/Utils.hpp>

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <streambuf>
#include <string>
#include <thread>

#include <stdio.h>

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

