#include "pch.hpp"
#include "D3D/Renderer.h"
#include <BlackBox/Core/Platform/platform_impl.inl>

IRENDER_API IRenderer* CreateIRender(ISystem* pSystem)
{
	return gcpRendD3D;
}

