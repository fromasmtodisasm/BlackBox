#pragma once

#define IGAME_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>

/////////////////////////////////////////////////////////////////////////////
// STL //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#pragma warning (disable : 4786)
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <list>
#include <map>	
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#if 0
#if defined USE_DEBUG_NEW
    #if defined(_DEBUG) && !defined(LINUX)
        #include <crtdbg.h>
        #define DEBUG_CLIENTBLOCK new( _NORMAL_BLOCK, __FILE__, __LINE__) 
        #define new DEBUG_CLIENTBLOCK
    #endif
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// Interfaces ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include <BlackBox/Renderer/IRender.hpp>

#include <Cry_Color4.h>
#include <TArray.h>

#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/Core/IMarkers.hpp>
#include <BlackBox/Core/Utils.hpp>

#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ITimer.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/System/FrameProfiler.hpp>
#include <BlackBox/EntitySystem/EntityDesc.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/Input/IHardwareMouse.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
