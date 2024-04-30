#pragma once
#define ENTITYDLL_EXPORTS


#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Core/Stream.hpp>
#include <BlackBox/EntitySystem/EntityDesc.hpp>

#include <BlackBox/System/FrameProfiler.hpp>

#include <vector>
#include <string_view>
using std::string_view;

#pragma warning(disable: 4266)     // no override available for virtual member function from base 'type'; function is hidden
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

extern bool g_bProfilerEnabled;
