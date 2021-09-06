#pragma once
#define IRENDER_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#ifdef DX_RENDERER
#	include <BlackBox/Core/Platform/Windows.hpp>
#	define __IFont_INTERFACE_DEFINED__
#	include <d3d10.h>
#endif
#ifdef VK_RENDERER
#	include <vulkan/vulkan.h>
#endif

#include <array>
#include <sstream>
