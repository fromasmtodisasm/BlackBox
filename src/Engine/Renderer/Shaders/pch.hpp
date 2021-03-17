#pragma once
#define IRENDER_EXPORTS

#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>

#ifdef DX_RENDERER
#define __IFont_INTERFACE_DEFINED__
#include <d3d10.h>
#endif

#include <array>
#include <sstream>