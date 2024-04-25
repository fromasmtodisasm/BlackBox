#pragma once

#define SYSTEM_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/System/ConsoleRegistration.h>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ITimer.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <ISound.h>

#include "pch.h"

#include "CmdLineArg.hpp"
#include "Console.hpp"
#include "HardwareMouse.hpp"

#ifndef LINUX
#	include "File/CryPak.hpp"
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

template<typename T>
using DynArray = std::vector<T>;

#include "CrySizerImpl.h"

#ifdef NOT_IMPLEMENTED_V
#	undef NOT_IMPLEMENTED_V
#	define NOT_IMPLEMENTED_V                                   \
		CryError("Function: %s not implemented", __FUNCTION__); \
	return {};
#endif

#ifdef NOT_IMPLEMENTED
#	undef NOT_IMPLEMENTED
#	define NOT_IMPLEMENTED                                     \
		CryError("Function: %s not implemented", __FUNCTION__); \
		//assert(0 && __FUNCTION__);
#endif
