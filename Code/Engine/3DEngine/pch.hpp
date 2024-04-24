#pragma once
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/System/File/ICryPak.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>

#define BB_3DENGINE_EXPORTS

template<class T>
T* CryAlignedNew()
{
	return new T;
}
