project(Common CXX)

set(INC "${CMAKE_SOURCE_DIR}/Include/BlackBox")

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Common
	SOURCE_GROUP "3DEngine"
		3DEngine/CryHeaders.h
		3DEngine/I3DEngine.hpp
		3DEngine/IBindable.hpp
		3DEngine/IObject.hpp
		3DEngine/IStatObj.hpp
	SOURCE_GROUP "Core"
		Core/BaseTypes.hpp
		Core/Compiler/Clangspecific.h
		Core/Compiler/MSVCspecific.h
		Core/CryCustomTypes.h
		Core/ICompressionHelper.hpp
		Core/IMarkers.hpp
		Core/Path.hpp
		Core/StlUtils.hpp
		Core/Stream.hpp
		Core/StringUtils.h
		Core/TArrays.h
		Core/Tarray.h
		Core/TypeInfo_decl.h
		Core/TypeInfo_impl.h
		Core/Utils.hpp
		Core/Version.hpp
		Core/smartptr.hpp
		Core/CryTypeInfo.h
		Core/CryTypeInfo.inl
	
	SOURCE_GROUP "Math"
		Math/MathHelper.hpp
		Math/Cry_Camera.h
		Math/Cry_Color4.h
		Math/Cry_Geo.h
		Math/Cry_GeoDistance.h
		Math/Cry_GeoIntersect.h
		Math/Cry_GeoOverlap.h
		Math/Cry_Math.h
		Math/Cry_Matrix.h
		Math/Cry_Quat.h
		Math/Cry_Vector2.h
		Math/Cry_Vector3.h
		Math/Cry_XOptimise.h
		Math/Geo.hpp
		Math/platform.h
	SOURCE_GROUP "Core\\\\Platform"
		Core/Platform/CryLibrary.h
		Core/Platform/Linux32Specific.h
		Core/Platform/Linux64Specific.h
		Core/Platform/LinuxSpecific.h
		Core/Platform/Linux_Win32Wrapper.h
		Core/Platform/Platform.hpp
		Core/Platform/PlatformDefines.hpp
		Core/Platform/Project/ProjectDefines.hpp
		Core/Platform/StlConfig.h
		Core/Platform/Windows.hpp
		Core/Platform/WindowsSpecific.hpp
		Core/Platform/_CryLibrary.h
		Core/Platform/platform_impl.inl
		Core/Platform/platform_impl_winapi.inl
	SOURCE_GROUP "Core\\\\Platform"
		Core/Platform/Project/ProjectDefines.hpp
	SOURCE_GROUP "EntitySystem"
		EntitySystem/EntityDesc.hpp
		EntitySystem/IEntityRenderState.hpp
		EntitySystem/IEntitySystem.hpp
	SOURCE_GROUP "Input"
		Input/Events.h
		Input/IHardwareMouse.hpp
		Input/IInput.hpp
	SOURCE_GROUP "Input"
		Game/IGame.hpp
	SOURCE_GROUP "Legacy"
		Legacy/AgentParams.h
		Legacy/StlUtils.h
		Legacy/AnimKey.h
		Legacy/ColorDefs.h
		Legacy/CryAnimationScriptCommands.h
		Legacy/CryCharAnimationParams.h
		Legacy/CryCharFxTrailParams.h
		Legacy/CryCharMorphParams.h
		Legacy/CryCompiledFile.h
		Legacy/CryEngineDecalInfo.h
		Legacy/CryHeaders.h
		Legacy/CrySizer.h
		Legacy/CryVersion.h
		Legacy/Cry_Color4.h
		Legacy/Cry_Geo.h
		Legacy/Cry_GeoDistance.h
		Legacy/Cry_GeoIntersect.h
		Legacy/Cry_GeoOverlap.h
		Legacy/Cry_Math.h
		Legacy/Cry_Matrix.h
		Legacy/Cry_Quat.h
		Legacy/Cry_Vector2.h
		Legacy/Cry_Vector3.h
		Legacy/Cry_XOptimise.h
		Legacy/Font.h
		Legacy/IFont.h
		Legacy/IAISystem.h
		Legacy/IAgent.h
		Legacy/list2.h
		Legacy/IMiniLog.h
		Legacy/ILog.h
		Legacy/IBindable.h
		Legacy/ICryAnimation.h
		Legacy/IDataProbe.h
		Legacy/ILipSync.h
		Legacy/IMovieSystem.h
		Legacy/IPhysics.h
		Legacy/IProcess.h
		Legacy/ISound.h
		Legacy/IStatObj.h
		Legacy/IXMLDOM.h
		Legacy/IXml.h
		Legacy/MeshIdx.h
		Legacy/Names.h
		Legacy/Range.h
		Legacy/SerializeBuffer.h
		Legacy/StreamData.h
		Legacy/TAlloc.h
		Legacy/TArrays.h
		Legacy/TString.h
		Legacy/Tarray.h
		Legacy/XmlParser.h
		Legacy/XmlParserFont.h
		Legacy/XmlStream.h
		Legacy/crysound.h
		Legacy/crysound64.h
		Legacy/physinterface.h
		Legacy/platform.h
		Legacy/primitives.h
		Legacy/smartptr.h
	SOURCE_GROUP "Memory"
		Memory/CryMemoryManager.h
		Memory/CryMemoryManager_impl.h
	SOURCE_GROUP "Network"
		Network/INetwork.hpp
		Network/IPAddress.hpp
		Network/Socks.hpp
		Network/Socks_posix.hpp
		Network/Socks_win32.hpp
	#Profiler/Config.h.in
	#Profiler/Drawer2D.h
	#Profiler/Hole_array.h
	#Profiler/HP_Timer.h
	#Profiler/Profiler.h
	#Profiler/Tgaloader.h
	#Profiler/Thread.h
	#Profiler/Utils.h
	SOURCE_GROUP "Renderer"
		Renderer/Camera.hpp
		Renderer/IFont.hpp
		Renderer/IRender.hpp
		Renderer/IShader.hpp
		Renderer/IRenderAuxGeom.hpp
		Renderer/VertexFormats.hpp
		Renderer/RendElement.h
	SOURCE_GROUP "ScriptSystem"
		ScriptSystem/IScriptSystem.hpp
		ScriptSystem/ScriptObject.hpp
		ScriptSystem/_ScriptableEx.hpp
	SOURCE_GROUP "System"
		System/ConsoleCommands.hpp
		System/ConsoleRegistration.h
		System/File/CryFile.h
		System/CrySizer.hpp
		System/CVarOverrides.h
		System/File/ICryPak.hpp
		System/FrameProfiler.hpp
		System/IBitStream.hpp
		System/IClipBoard.hpp
		System/ICmdLine.hpp
		System/ICommand.hpp
		System/IConsole.hpp
		System/IInputHandler.hpp
		System/ILog.hpp
		System/IMiniGUI.hpp
		System/IMiniLog.hpp
		System/IPlatform.hpp
		System/IProcess.hpp
		System/IProjectManager.hpp
		System/IStreamEngine.h
		System/ISystem.hpp
		System/ITextModeConsole.h
		System/ITimer.hpp
		System/IValidator.hpp
		System/IWindow.hpp
		System/IWindowMessageHandler.h
		System/TimeValue.hpp
	SOURCE_GROUP "System\\\\ZLib"
		System/ZLib/IZLibCompressor.h
		System/ZLib/IZlibDecompressor.h
		System/ZLib/ILZ4Decompressor.h
	SOURCE_GROUP "Threading"
		Threading/IThreadManager.h
	SOURCE_GROUP "Utils"
		Utils/Text.hpp
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
		install(DIRECTORY ${dir} DESTINATION include/BlackBox)
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
    $<INSTALL_INTERFACE:include/Legacy> 
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
		DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake"
		NAMESPACE BlackBox::
	)
endif()
