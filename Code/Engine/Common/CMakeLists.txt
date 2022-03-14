project(Common CXX)

set(INC "${CMAKE_SOURCE_DIR}/Include/BlackBox")

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Common
	SOURCE_GROUP "3DEngine"
		BlackBox/3DEngine/CryHeaders.h
		BlackBox/3DEngine/I3DEngine.hpp
		BlackBox/3DEngine/IBindable.hpp
		BlackBox/3DEngine/IObject.hpp
		BlackBox/3DEngine/IStatObj.hpp
	SOURCE_GROUP "Core"
		BlackBox/Core/BaseTypes.hpp
		BlackBox/Core/Compiler/Clangspecific.h
		BlackBox/Core/Compiler/MSVCspecific.h
		BlackBox/Core/ICompressionHelper.hpp
		BlackBox/Core/IMarkers.hpp
		BlackBox/Core/Path.hpp
		BlackBox/Core/StlUtils.hpp
		BlackBox/Core/Stream.hpp
		BlackBox/Core/StringUtils.h
		BlackBox/Core/TArrays.h
		BlackBox/Core/Tarray.h
		BlackBox/Core/TypeInfo_decl.h
		BlackBox/Core/Utils.hpp
		BlackBox/Core/Version.hpp
		BlackBox/Core/smartptr.hpp
	
	SOURCE_GROUP "Math"
		BlackBox/Math/MathHelper.hpp
		BlackBox/Math/Cry_Camera.h
		BlackBox/Math/Cry_Color4.h
		BlackBox/Math/Cry_Geo.h
		BlackBox/Math/Cry_GeoDistance.h
		BlackBox/Math/Cry_GeoIntersect.h
		BlackBox/Math/Cry_GeoOverlap.h
		BlackBox/Math/Cry_Math.h
		BlackBox/Math/Cry_Matrix.h
		BlackBox/Math/Cry_Quat.h
		BlackBox/Math/Cry_Vector2.h
		BlackBox/Math/Cry_Vector3.h
		BlackBox/Math/Cry_XOptimise.h
		BlackBox/Math/Geo.hpp
		BlackBox/Math/platform.h
	SOURCE_GROUP "Core\\\\Platform"
		BlackBox/Core/Platform/CryLibrary.h
		BlackBox/Core/Platform/Linux32Specific.h
		BlackBox/Core/Platform/Linux64Specific.h
		BlackBox/Core/Platform/LinuxSpecific.h
		BlackBox/Core/Platform/Linux_Win32Wrapper.h
		BlackBox/Core/Platform/Platform.hpp
		BlackBox/Core/Platform/PlatformDefines.hpp
		BlackBox/Core/Platform/Project/ProjectDefines.hpp
		BlackBox/Core/Platform/StlConfig.h
		BlackBox/Core/Platform/Windows.hpp
		BlackBox/Core/Platform/WindowsSpecific.hpp
		BlackBox/Core/Platform/_CryLibrary.h
		BlackBox/Core/Platform/platform_impl.inl
		BlackBox/Core/Platform/platform_impl_winapi.inl
	SOURCE_GROUP "Core\\\\Platform"
		BlackBox/Core/Platform/Project/ProjectDefines.hpp
	SOURCE_GROUP "EntitySystem"
		BlackBox/EntitySystem/EntityDesc.hpp
		BlackBox/EntitySystem/IEntityRenderState.hpp
		BlackBox/EntitySystem/IEntitySystem.hpp
	SOURCE_GROUP "Input"
		BlackBox/Input/Events.h
		BlackBox/Input/IHardwareMouse.hpp
		BlackBox/Input/IInput.hpp
	SOURCE_GROUP "Input"
		BlackBox/Game/IGame.hpp
	SOURCE_GROUP "Legacy"
		BlackBox/Legacy/AgentParams.h
		BlackBox/Legacy/StlUtils.h
		BlackBox/Legacy/AnimKey.h
		BlackBox/Legacy/ColorDefs.h
		BlackBox/Legacy/CryAnimationScriptCommands.h
		BlackBox/Legacy/CryCharAnimationParams.h
		BlackBox/Legacy/CryCharFxTrailParams.h
		BlackBox/Legacy/CryCharMorphParams.h
		BlackBox/Legacy/CryCompiledFile.h
		BlackBox/Legacy/CryEngineDecalInfo.h
		BlackBox/Legacy/CryHeaders.h
		BlackBox/Legacy/CrySizer.h
		BlackBox/Legacy/CryVersion.h
		BlackBox/Legacy/Cry_Color4.h
		BlackBox/Legacy/Cry_Geo.h
		BlackBox/Legacy/Cry_GeoDistance.h
		BlackBox/Legacy/Cry_GeoIntersect.h
		BlackBox/Legacy/Cry_GeoOverlap.h
		BlackBox/Legacy/Cry_Math.h
		BlackBox/Legacy/Cry_Matrix.h
		BlackBox/Legacy/Cry_Quat.h
		BlackBox/Legacy/Cry_Vector2.h
		BlackBox/Legacy/Cry_Vector3.h
		BlackBox/Legacy/Cry_XOptimise.h
		BlackBox/Legacy/Font.h
		BlackBox/Legacy/IFont.h
		BlackBox/Legacy/IAISystem.h
		BlackBox/Legacy/IAgent.h
		BlackBox/Legacy/list2.h
		BlackBox/Legacy/IMiniLog.h
		BlackBox/Legacy/ILog.h
		BlackBox/Legacy/IBindable.h
		BlackBox/Legacy/ICryAnimation.h
		BlackBox/Legacy/IDataProbe.h
		BlackBox/Legacy/ILipSync.h
		BlackBox/Legacy/IMovieSystem.h
		BlackBox/Legacy/IPhysics.h
		BlackBox/Legacy/IProcess.h
		BlackBox/Legacy/ISound.h
		BlackBox/Legacy/IStatObj.h
		BlackBox/Legacy/IXMLDOM.h
		BlackBox/Legacy/IXml.h
		BlackBox/Legacy/MeshIdx.h
		BlackBox/Legacy/Names.h
		BlackBox/Legacy/Range.h
		BlackBox/Legacy/SerializeBuffer.h
		BlackBox/Legacy/StreamData.h
		BlackBox/Legacy/TAlloc.h
		BlackBox/Legacy/TArrays.h
		BlackBox/Legacy/TString.h
		BlackBox/Legacy/Tarray.h
		BlackBox/Legacy/XmlParser.h
		BlackBox/Legacy/XmlParserFont.h
		BlackBox/Legacy/XmlStream.h
		BlackBox/Legacy/crysound.h
		BlackBox/Legacy/crysound64.h
		BlackBox/Legacy/physinterface.h
		BlackBox/Legacy/platform.h
		BlackBox/Legacy/primitives.h
		BlackBox/Legacy/smartptr.h
	SOURCE_GROUP "Memory"
		BlackBox/Memory/CryMemoryManager.h
		BlackBox/Memory/CryMemoryManager_impl.h
	SOURCE_GROUP "Network"
		BlackBox/Network/INetwork.hpp
		BlackBox/Network/IPAddress.hpp
		BlackBox/Network/Socks.hpp
		BlackBox/Network/Socks_posix.hpp
		BlackBox/Network/Socks_win32.hpp
	#Profiler/Config.h.in
	#Profiler/Drawer2D.h
	#Profiler/Hole_array.h
	#Profiler/HP_Timer.h
	#Profiler/Profiler.h
	#Profiler/Tgaloader.h
	#Profiler/Thread.h
	#Profiler/Utils.h
	SOURCE_GROUP "Renderer"
		BlackBox/Renderer/Camera.hpp
		BlackBox/Renderer/IFont.hpp
		BlackBox/Renderer/IRender.hpp
		BlackBox/Renderer/IShader.hpp
		BlackBox/Renderer/IRenderAuxGeom.hpp
		BlackBox/Renderer/VertexFormats.hpp
		BlackBox/Renderer/RendElement.h
	SOURCE_GROUP "ScriptSystem"
		BlackBox/ScriptSystem/IScriptSystem.hpp
		BlackBox/ScriptSystem/ScriptObject.hpp
		BlackBox/ScriptSystem/_ScriptableEx.hpp
	SOURCE_GROUP "System"
		BlackBox/System/ConsoleCommands.hpp
		BlackBox/System/ConsoleRegistration.h
		BlackBox/System/File/CryFile.h
		BlackBox/System/CrySizer.hpp
		BlackBox/System/CVarOverrides.h
		BlackBox/System/File/ICryPak.hpp
		BlackBox/System/FrameProfiler.hpp
		BlackBox/System/IBitStream.hpp
		BlackBox/System/IClipBoard.hpp
		BlackBox/System/ICmdLine.hpp
		BlackBox/System/ICommand.hpp
		BlackBox/System/IConsole.hpp
		BlackBox/System/IInputHandler.hpp
		BlackBox/System/ILog.hpp
		BlackBox/System/IMiniGUI.hpp
		BlackBox/System/IMiniLog.hpp
		BlackBox/System/IPlatform.hpp
		BlackBox/System/IProcess.hpp
		BlackBox/System/IProjectManager.hpp
		BlackBox/System/IStreamEngine.h
		BlackBox/System/ISystem.hpp
		BlackBox/System/ITextModeConsole.h
		BlackBox/System/ITimer.hpp
		BlackBox/System/IValidator.hpp
		BlackBox/System/IWindow.hpp
		BlackBox/System/IWindowMessageHandler.h
		BlackBox/System/TimeValue.hpp
	SOURCE_GROUP "System\\\\ZLib"
		BlackBox/System/ZLib/IZLibCompressor.h
		BlackBox/System/ZLib/IZlibDecompressor.h
		BlackBox/System/ZLib/ILZ4Decompressor.h
	SOURCE_GROUP "Threading"
		BlackBox/Threading/IThreadManager.h
	SOURCE_GROUP "Utils"
		BlackBox/Utils/Text.hpp
	)

end_sources()

#make_directory(CopyIncludes)
#foreach(src ${SOURCES})
#	#get_filename_component(DIR ${src} )
#	file(RELATIVE_PATH
#		ITEM_PATH_REL # Output variable
#		${BLACKBOX_DIR} # Base directory
#		${src} # Absolute path to the file
#	)
#	#message("Relative path: ${ITEM_PATH_REL}")
#	get_filename_component(REL ${ITEM_PATH_REL} DIRECTORY)
#	file(INSTALL ${src} DESTINATION ${CMAKE_BINARY_DIR}/CopyIncludes/${REL})
#endforeach()

CryFileContainer(Common SOLUTION_FOLDER Common)


macro(install_headers)
	foreach(dir ${ARGN})
		install(DIRECTORY BlackBox/${dir} DESTINATION include/BlackBox)
	endforeach()
endmacro()

install_headers(3DEngine)
install_headers(Core EntitySystem)
install_headers(Game Input Legacy)#Math)
install_headers(Memory Network)
install_headers(Renderer ScriptSystem)
install_headers(System Threading)
install_headers(Utils)

add_library(Interface INTERFACE ${SOURCES})
target_include_directories(Interface INTERFACE
    $<BUILD_INTERFACE:BlackBox>  
    $<INSTALL_INTERFACE:include> 
    $<INSTALL_INTERFACE:include/BlackBox/Legacy> 
)

# FIXME
if (0)
	target_link_libraries(Interface INTERFACE GameLauncher)

	install(TARGETS Interface
		EXPORT blackboxConfig 
		INCLUDES DESTINATION include
	)

	export(TARGETS
		Interface
		NAMESPACE BlackBox::
		FILE "${CMAKE_CURRENT_BINARY_DIR}/blackboxConfig.cmake"
	)
	install(EXPORT
			blackboxConfig
		DESTINATION "${CMAKE_INSTALL_DATADIR}/blackbox/cmake"
		NAMESPACE BlackBox::
	)
endif()
