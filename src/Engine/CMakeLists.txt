cmake_minimum_required(VERSION 3.10)
project(Common CXX)
###################################################
include(GenerateExportHeader)
###################################################
set(BLACKBOX_PROJECT BlackBox)
###################################################
add_library(${BLACKBOX_PROJECT})
add_library(${BLACKBOX_PROJECT}::Common ALIAS ${BLACKBOX_PROJECT})
add_library(Common INTERFACE)
add_library(BB_Private INTERFACE)
###################################################
set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")
set_property(DIRECTORY APPEND PROPERTY INCLUDE_DIRECTORIES "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/.." "${BLACKBOX_PUBLIC_INCLUDE_DIRS}/..")

#set(CMAKE_CXX_STANDARD 17)
target_compile_features(BlackBox PUBLIC cxx_std_17)
target_compile_features(Common INTERFACE cxx_std_17)

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
target_include_directories(
BB_Private
	INTERFACE "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/../"
	INTERFACE "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/Common"
	INTERFACE "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/Interfaces/"
	INTERFACE "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/Render/"
	INTERFACE "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/Resources/"
	INTERFACE "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/Utils/"
	INTERFACE "${SUBMODULES_DIR}/stb/"
)
target_include_directories (
Common
	INTERFACE	"${CMAKE_BINARY_DIR}"
	INTERFACE	"${BLACKBOX_PUBLIC_INCLUDE_DIRS}/../"
	)
#--------------------------------------------------
#--------------------------------------------------
###################################################
# Subs
###################################################
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE BB_Private)
add_subdirectory(Math)
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Math)
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Common)

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


set_target_properties(GUI PROPERTIES FOLDER "Engine")
set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")

target_link_libraries(Common INTERFACE tinyxml2::tinyxml2)
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
install (
  TARGETS  ${BLACKBOX_PROJECT}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib/static
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${BLACKBOX_PROJECT}
)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/media DESTINATION ${BLACKBOX_PROJECT}
)
###################################################
