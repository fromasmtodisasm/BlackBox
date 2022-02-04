set(MSVC_COMMON_FLAGS 
	/w
	/nologo     # Don't show version info
	#/W3         # Enable warning level 3
	/fp:fast    # Use fast floating point precision model
	/Zc:wchar_t # Parse wchar_t as internal type
	/GF         # Eliminate Duplicate Strings
	/Gy         # Enable function level linking
	/utf-8      # Set source and execution charset to utf-8
	#/Wv:18      # Disable warnings until SDK depedencies switch to UTF-8/ASCII.
	#/MP         # Build with multiple processes
	/bigobj     # Allow larger .obj files

	#/WX         # Treat warnings as errors
	/wd4653     # Ignore PCH for any individual file that has different optimization settings
	/wd4275     # non dll-interface class '' used as base for dll-interface class ''
	/wd4961     # No profile data was merged into '', profile-guided optimizations disabled
	/wd4068     # 'Unknown pragma' - sometimes need these in code to prevent global suppression.
	/wd4554     
	/wd4005     
	/wd4100
	/wd4201
	/wd4458
	/wd4127
	/wd4099
	#-Werror
	#-Wdeprecated-declarations
	#-Winline-new-delete #replacement function 'operator new' cannot be declared 'inline'

	-fms-compatibility
	/EHsc
)
string(REPLACE ";" " " MSVC_COMMON_FLAGS "${MSVC_COMMON_FLAGS}")


set(CMAKE_RC_FLAGS /nologo)

# Override cxx flags
set(CMAKE_CXX_FLAGS "${MSVC_COMMON_FLAGS}" CACHE STRING "C++ Common Flags" FORCE)

set(CMAKE_C_FLAGS_DEBUG "${MSVC_COMMON_FLAGS} /MTd /Od /Ob0 /Oy- /RTC1 /GS /DDEBUG /D_DEBUG" CACHE STRING "C Flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${MSVC_COMMON_FLAGS} /MTd /Od /Ob0 /Oy- /RTC1 /GS /DDEBUG /D_DEBUG" CACHE STRING "C++ Flags" FORCE)
message(FATAL_ERROR "!@#@!#")

# Create PDBs ()
# Create better debug info (/Zo)
# Enable full optimization (/Ox) Same as /Ob2 /Oi /Ot /Oy
# Don't omit frame pointer (/Oy-)
# Disable buffer security checks (/GS-)
set(CMAKE_C_FLAGS_PROFILE "${MSVC_COMMON_FLAGS} /Zo /MD /Ox /Oy- /GS- /DNDEBUG /D_PROFILE" CACHE STRING "C Flags" FORCE)
set(CMAKE_CXX_FLAGS_PROFILE "${MSVC_COMMON_FLAGS} /Zo /MD /Ox /Oy- /GS- /DNDEBUG /D_PROFILE" CACHE STRING "C++ Flags" FORCE)

set(CMAKE_C_FLAGS_RELEASE "${MSVC_COMMON_FLAGS} /Zo /MD /Ox /GS- /DNDEBUG /D_RELEASE" CACHE STRING "C Flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${MSVC_COMMON_FLAGS} /Zo /MD /Ox /GS- /DNDEBUG /D_RELEASE" CACHE STRING "C++ Flags" FORCE)

set(CMAKE_SHARED_LINKER_FLAGS_PROFILE "/debug" CACHE STRING "Profile link flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_PROFILE    "/debug" CACHE STRING "Profile link flags" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS_PROFILE "/debug /INCREMENTAL" CACHE STRING "Profile link flags" FORCE)

set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "/debug" CACHE STRING "Release link flags" FORCE)
set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "/debug" CACHE STRING "Release link flags" FORCE)
set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "/debug  /OPT:REF /OPT:ICF" CACHE STRING "Release link flags" FORCE)



#function (wrap_whole_archive project target source)
#	set(${target} "${${source}}" PARENT_SCOPE)
#	set( whole_archive_link_flags "")
#	foreach(module ${${source}})
#		# Get Type of module target, (STATIC_LIBRARY,SHARED_LIBRARY,EXECUTABLE)
#		get_target_property(ModuleProjectType ${module} TYPE)
#		if (ModuleProjectType STREQUAL "STATIC_LIBRARY")
#			# Only Static library needs a /WHOLEARCHIVE linker switch to prevent linker from optimizing static factories
#			list( APPEND whole_archive_link_flags "/WHOLEARCHIVE:${module}")
#		endif()
#	endforeach()
#	string (REPLACE ";" " " whole_archive_link_flags "${whole_archive_link_flags}")
#	#message(STATUS "${whole_archive_link_flags}")
#	set_target_properties(${project} PROPERTIES LINK_FLAGS ${whole_archive_link_flags})
#endfunction()
function (wrap_whole_archive project target source)
	set(${target} "-Wl,--whole-archive;${${source}};-Wl,--no-whole-archive" PARENT_SCOPE)
endfunction()

message(STATUS "MSVC_VERSION = ${MSVC_VERSION}")
message(STATUS "MSVC_TOOLSET_VERSION = ${MSVC_TOOLSET_VERSION}")
