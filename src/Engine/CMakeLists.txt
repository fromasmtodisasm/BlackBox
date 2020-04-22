cmake_minimum_required(VERSION 3.10)
###################################################
include(GenerateExportHeader)
###################################################
set(BLACKBOX_PROJECT BlackBox)
###################################################
add_library(${BLACKBOX_PROJECT})
###################################################
set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")
set_property(
	DIRECTORY APPEND PROPERTY INCLUDE_DIRECTORIES 
	$<INSTALL_INTERFACE:${BLACKBOX_INCLUDE_DIRS}/public>

	$<BUILD_INTERFACE:${BLACKBOX_INCLUDE_DIRS}/public>
	$<BUILD_INTERFACE:${BLACKBOX_INCLUDE_DIRS}/private>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

target_include_directories(${BLACKBOX_PROJECT}
	INTERFACE
	$<BUILD_INTERFACE:${BLACKBOX_INCLUDE_DIRS}/public>
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

set(CMAKE_CXX_STANDARD 17)
target_compile_features(BlackBox PUBLIC cxx_std_17)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
###################################################
configure_file(${BLACKBOX_PUBLIC_INCLUDE_DIRS}/Profiler/Config.h.in ${CMAKE_BINARY_DIR}/Config.h @ONLY)
###################################################
if (BB_TESET_ENABLE)
  target_compile_definitions(${BLACKBOX_PROJECT} PUBLIC -DTEST)
  target_compile_definitions(${BLACKBOX_PROJECT} PUBLIC -DSINGLE_PASS)
endif()
find_package(OpenGL REQUIRED)
add_package(tinyxml2)
add_package(freetype)
#--------------------------------------------------
###################################################
# Subs
###################################################
add_subdirectory(Math)
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Math)

add_subsystem(Input)
add_subsystem(Geometry)
add_subsystem(Renderer)
add_subsystem(ScriptSystem)
add_subsystem(System)
add_subsystem(Window)
add_subsystem(Profiler)
add_subsystem(Resources)
add_subsystem(Utils)
add_subsystem(Network)
add_subsystem(GUI)
add_subsystem(World)
add_subsystem(Scene)

foreach(sys IN LISTS ALL_PROJECT_SYSTEMS)
	message(STATUS "sub: ${sys}")
endforeach()

set_target_properties(GUI PROPERTIES FOLDER "Engine")
set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")

target_link_libraries(${BLACKBOX_PROJECT} INTERFACE tinyxml2::tinyxml2)
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE ${OPENGL_gl_LIBRARY})
###################################################
generate_export_header(${BLACKBOX_PROJECT}
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/Exports.hpp
)
if (LINUX)
	target_link_libraries(${BLACKBOX_PROJECT} PRIVATE freetype)
else()
	target_link_libraries(${BLACKBOX_PROJECT} PRIVATE Freetype::Freetype)
endif()
###################################################
if (UNIX_OR_CYGWIN)
  target_sources(
    ${BLACKBOX_PROJECT}
    PRIVATE ${ENGINE_DIR}/System/Linux/Runtime.cpp
    )
endif()
###################################################
#--------------------------------------------------
if (LINUX OR UNIX OR APPLE)
	target_link_libraries(BlackBox PUBLIC ${CMAKE_DL_LIBS})
endif()
target_sources(BlackBox
	PRIVATE dummy.cpp
)
#--------------------------------------------------
get_target_property(SOURCE_FILES ${BLACKBOX_PROJECT}  SOURCES)
source_group(
  TREE ${SOURCE_DIR}
  #PREFIX BlackBox
  FILES  ${SOURCE_FILES}
  )
###################################################
#[[
install (
  TARGETS  ${BLACKBOX_PROJECT}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)
install(
	DIRECTORY ${CMAKE_BINARY_DIR}
	DESTINATION include
	COMPONENT developer
	FILES_MATCHING
	PATTERN "*.h"
)
]]
install_this(${BLACKBOX_PROJECT})


install(
  DIRECTORY ${BLACKBOX_PUBLIC_INCLUDE_DIRS}/ DESTINATION include
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION .
)
#[[
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/media DESTINATION .
)
]]
###################################################
