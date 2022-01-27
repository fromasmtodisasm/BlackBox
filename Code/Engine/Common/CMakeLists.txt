project(Common CXX)

set(INC "${CMAKE_SOURCE_DIR}/include/public/BlackBox")

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Common
	SOURCE_GROUP "3DEngine"
		${INC}/3DEngine/CryHeaders.h
		${INC}/3DEngine/I3DEngine.hpp
		${INC}/3DEngine/IBindable.hpp
		${INC}/3DEngine/IObject.hpp
		${INC}/3DEngine/IStatObj.hpp
	SOURCE_GROUP "Core"
		#${INC}/Core/StlUtils.hpp
		${INC}/Core/MathHelper.hpp
		${INC}/Core/Path.hpp
		${INC}/Core/Primitives.hpp
		${INC}/Core/Stream.hpp
		${INC}/Core/StringUtils.h
		${INC}/Core/TArrays.h
		${INC}/Core/Tarray.h
		${INC}/Core/Utils.hpp
		${INC}/Core/Version.hpp
		${INC}/Core/BaseTypes.hpp
		${INC}/Core/Compiler/MSVCspecific.h
		${INC}/Core/IApp.hpp
		${INC}/Core/ICompressionHelper.hpp
		${INC}/Core/IGame.hpp
		${INC}/Core/IMarkers.hpp
	SOURCE_GROUP "Core\\\\Math"
		${INC}/Core/Math/Cry_Camera.h
		${INC}/Core/Math/Cry_Color4.h
		${INC}/Core/Math/Cry_Geo.h
		${INC}/Core/Math/Cry_GeoDistance.h
		${INC}/Core/Math/Cry_GeoIntersect.h
		${INC}/Core/Math/Cry_GeoOverlap.h
		${INC}/Core/Math/Cry_Math.h
		${INC}/Core/Math/Cry_Matrix.h
		${INC}/Core/Math/Cry_Quat.h
		${INC}/Core/Math/Cry_Vector2.h
		${INC}/Core/Math/Cry_Vector3.h
		${INC}/Core/Math/Cry_XOptimise.h
		${INC}/Core/Math/Geo.hpp
		${INC}/Core/Math/platform.h
	SOURCE_GROUP "Core\\\\Platform"
		${INC}/Core/Platform/CryLibrary.h
		${INC}/Core/Platform/Linux32Specific.h
		${INC}/Core/Platform/Linux64Specific.h
		${INC}/Core/Platform/LinuxSpecific.h
		${INC}/Core/Platform/Linux_Win32Wrapper.h
		${INC}/Core/Platform/Platform.hpp
		${INC}/Core/Platform/PlatformDefines.hpp
		${INC}/Core/Platform/Project/ProjectDefines.hpp
		${INC}/Core/Platform/StlConfig.h
		${INC}/Core/Platform/Windows.hpp
		${INC}/Core/Platform/WindowsSpecific.hpp
		${INC}/Core/Platform/_CryLibrary.h
		${INC}/Core/Platform/platform_impl.inl
		${INC}/Core/Platform/platform_impl_winapi.inl
	SOURCE_GROUP "EntitySystem"
		${INC}/EntitySystem/EntityDesc.hpp
		${INC}/EntitySystem/IEntityRenderState.hpp
		${INC}/EntitySystem/IEntitySystem.hpp
	SOURCE_GROUP "Input"
		${INC}/Input/Events.h
		${INC}/Input/IHardwareMouse.hpp
		${INC}/Input/IInput.hpp
	SOURCE_GROUP "Legacy"
		${INC}/Legacy/AgentParams.h
		${INC}/Legacy/StlUtils.h
		${INC}/Legacy/AnimKey.h
		${INC}/Legacy/ColorDefs.h
		${INC}/Legacy/CryAnimationScriptCommands.h
		${INC}/Legacy/CryCharAnimationParams.h
		${INC}/Legacy/CryCharFxTrailParams.h
		${INC}/Legacy/CryCharMorphParams.h
		${INC}/Legacy/CryCompiledFile.h
		${INC}/Legacy/CryEngineDecalInfo.h
		${INC}/Legacy/CryHeaders.h
		${INC}/Legacy/CrySizer.h
		${INC}/Legacy/CryVersion.h
		${INC}/Legacy/Cry_Color4.h
		${INC}/Legacy/Cry_Geo.h
		${INC}/Legacy/Cry_GeoDistance.h
		${INC}/Legacy/Cry_GeoIntersect.h
		${INC}/Legacy/Cry_GeoOverlap.h
		${INC}/Legacy/Cry_Math.h
		${INC}/Legacy/Cry_Matrix.h
		${INC}/Legacy/Cry_Quat.h
		${INC}/Legacy/Cry_Vector2.h
		${INC}/Legacy/Cry_Vector3.h
		${INC}/Legacy/Cry_XOptimise.h
		${INC}/Legacy/Font.h
		${INC}/Legacy/IAISystem.h
		${INC}/Legacy/IAgent.h
		${INC}/Legacy/IBindable.h
		${INC}/Legacy/ICryAnimation.h
		${INC}/Legacy/IDataProbe.h
		${INC}/Legacy/ILipSync.h
		${INC}/Legacy/IMovieSystem.h
		${INC}/Legacy/IPhysics.h
		${INC}/Legacy/IProcess.h
		${INC}/Legacy/ISound.h
		${INC}/Legacy/IStatObj.h
		${INC}/Legacy/IXMLDOM.h
		${INC}/Legacy/IXml.h
		${INC}/Legacy/MeshIdx.h
		${INC}/Legacy/Names.h
		${INC}/Legacy/Range.h
		${INC}/Legacy/SerializeBuffer.h
		${INC}/Legacy/StreamData.h
		${INC}/Legacy/TAlloc.h
		${INC}/Legacy/TArrays.h
		${INC}/Legacy/TString.h
		${INC}/Legacy/Tarray.h
		${INC}/Legacy/XmlParser.h
		${INC}/Legacy/XmlParserFont.h
		${INC}/Legacy/XmlStream.h
		${INC}/Legacy/crysound.h
		${INC}/Legacy/crysound64.h
		${INC}/Legacy/physinterface.h
		${INC}/Legacy/platform.h
		${INC}/Legacy/primitives.h
		${INC}/Legacy/smartptr.h
	SOURCE_GROUP "Memory"
		${INC}/Memory/CryMemoryManager.h
		${INC}/Memory/CryMemoryManager_impl.h
	SOURCE_GROUP "Network"
		${INC}/Network/INetwork.hpp
		${INC}/Network/IPAddress.hpp
		${INC}/Network/Socks.hpp
		${INC}/Network/Socks_posix.hpp
		${INC}/Network/Socks_win32.hpp
	#Profiler/Config.h.in
	#Profiler/Drawer2D.h
	#Profiler/Hole_array.h
	#Profiler/HP_Timer.h
	#Profiler/Profiler.h
	#Profiler/Tgaloader.h
	#Profiler/Thread.h
	#Profiler/Utils.h
	SOURCE_GROUP "Renderer"
		${INC}/Renderer/AuxRenderer.hpp
		${INC}/Renderer/BaseTexture.hpp
		${INC}/Renderer/Camera.hpp
		${INC}/Renderer/IDrawable.hpp
		${INC}/Renderer/IFont.hpp
		${INC}/Renderer/IGeometry.hpp
		${INC}/Renderer/IMaterial.hpp
		${INC}/Renderer/IMaterialManager.hpp
		${INC}/Renderer/IPostProcessor.hpp
		${INC}/Renderer/IRender.cpp
		${INC}/Renderer/IRender.hpp
		${INC}/Renderer/IRenderAuxGeom.hpp
		${INC}/Renderer/ISceneManager.hpp
		${INC}/Renderer/IShader.hpp
		${INC}/Renderer/IShaderManager.hpp
		${INC}/Renderer/ITechnique.hpp
		${INC}/Renderer/ITexture.hpp
		${INC}/Renderer/Light.hpp
		${INC}/Renderer/Material.hpp
		${INC}/Renderer/QuadTree.hpp
		${INC}/Renderer/Texture.hpp
		${INC}/Renderer/VertexFormats.hpp
	SOURCE_GROUP "ScriptSystem"
		${INC}/ScriptSystem/IScriptSystem.hpp
		${INC}/ScriptSystem/ScriptObject.hpp
		${INC}/ScriptSystem/_ScriptableEx.hpp
	SOURCE_GROUP "System"
		${INC}/System/ConsoleCommands.hpp
		${INC}/System/ConsoleRegistration.h
		${INC}/System/CryFile.h
		${INC}/System/CrySizer.hpp
		${INC}/System/CVarOverrides.h
		${INC}/System/File/ICryPak.hpp
		${INC}/System/FrameProfiler.hpp
		${INC}/System/IBitStream.hpp
		${INC}/System/IClipBoard.hpp
		${INC}/System/ICmdLine.hpp
		${INC}/System/ICommand.hpp
		${INC}/System/IConsole.hpp
		${INC}/System/IInputHandler.hpp
		${INC}/System/ILog.hpp
		${INC}/System/IMiniGUI.hpp
		${INC}/System/IMiniLog.hpp
		${INC}/System/IPlatform.hpp
		${INC}/System/IProcess.hpp
		${INC}/System/IProjectManager.hpp
		${INC}/System/IStreamEngine.h
		${INC}/System/ISystem.hpp
		${INC}/System/ITextModeConsole.h
		${INC}/System/ITimer.hpp
		${INC}/System/IValidator.hpp
		${INC}/System/IWindow.hpp
		${INC}/System/IWindowMessageHandler.h
		${INC}/System/TimeValue.hpp
	SOURCE_GROUP "Threading"
		${INC}/Threading/IThread.h
		${INC}/Threading/IThreadManager.h
	SOURCE_GROUP "Utils"
		${INC}/Utils/smartptr.hpp
		${INC}/Utils/Text.hpp
	)

end_sources()


CryFileContainer(Common SOLUTION_FOLDER "Engine")

macro(install_headers)
	foreach(dir ${ARGN})
		install(DIRECTORY ${INC}/${dir} DESTINATION include/BlackBox)
	endforeach()
endmacro()

install_headers(3DEngine)
install_headers(Core EntitySystem)
#install_headers(Game Input Legacy Math)
install_headers(Input Legacy)
install_headers(Memory Network)
install_headers(Renderer ScriptSystem)
install_headers(System Threading)
install_headers(Utils)

add_library(Interface INTERFACE ${SOURCES})
target_include_directories(Interface INTERFACE
    $<BUILD_INTERFACE:${INC}>  
    $<INSTALL_INTERFACE:include> 
)

install(TARGETS Interface
	EXPORT blackbox-engineConfig 
	INCLUDES DESTINATION include
)

export(TARGETS
	Interface
	NAMESPACE BlackBox::
	FILE "${CMAKE_CURRENT_BINARY_DIR}/blackbox-engineConfig.cmake"
)
install(EXPORT
        blackbox-engineConfig
    DESTINATION "${CMAKE_INSTALL_DATADIR}/blackbox-engine/cmake"
    NAMESPACE BlackBox::
)

