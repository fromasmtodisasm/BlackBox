#pragma once
#include <BlackBox/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>

#define BB_3DENGINE_EXPORTS

template<class T>
T* CryAlignedNew()
{
	return new T;
}
