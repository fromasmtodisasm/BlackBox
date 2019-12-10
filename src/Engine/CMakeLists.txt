cmake_minimum_required(VERSION 3.15)
project(Common CXX)
###################################################
include(GenerateExportHeader)
include(FindFreetype)
###################################################
set(BLACKBOX_PROJECT BlackBox)
###################################################
add_library(${BLACKBOX_PROJECT})
add_library(${BLACKBOX_PROJECT}::Common ALIAS ${BLACKBOX_PROJECT})
add_library(Common INTERFACE)
add_library(BB_Private INTERFACE)
###################################################
set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")
#set_directory_properties(PROPERTIES INCLUDE_DIRECTORIES "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/.." )
set_property(DIRECTORY APPEND PROPERTY INCLUDE_DIRECTORIES "${BLACKBOX_PRIVATE_INCLUDE_DIRS}/.." "${BLACKBOX_PUBLIC_INCLUDE_DIRS}/..")

target_compile_features(BlackBox
PUBLIC
	cxx_strong_enums
PRIVATE
	cxx_lambdas
	cxx_range_for
	cxx_final
)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/Tools/Cmake/modules/")
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
###################################################
configure_file(${BLACKBOX_PUBLIC_INCLUDE_DIRS}/Profiler/Config.h.in ${CMAKE_BINARY_DIR}/Config.h @ONLY)
###################################################
if (BB_TESET_ENABLE)
  target_compile_definitions(${BLACKBOX_PROJECT} PUBLIC -DTEST)
  target_compile_definitions(${BLACKBOX_PROJECT} PUBLIC -DSINGLE_PASS)
endif()
find_package(OpenGL REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(Freetype REQUIRED)
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
add_subdirectory(${ENGINE_DIR}/Input Input)
add_subdirectory(${ENGINE_DIR}/Geometry Geometry)
add_subdirectory(${ENGINE_DIR}/Math Math)
add_subdirectory(${ENGINE_DIR}/Render Renderer)
add_subdirectory(${ENGINE_DIR}/ScriptSystem ScriptSystem)
add_subdirectory(${ENGINE_DIR}/System System)
add_subdirectory(${ENGINE_DIR}/Window Window)
add_subdirectory(${ENGINE_DIR}/Profiler Profiler)
add_subdirectory(${ENGINE_DIR}/Resources Resources)
add_subdirectory(${ENGINE_DIR}/Utils Utils)
add_subdirectory(${ENGINE_DIR}/Network Network)
add_subdirectory(${ENGINE_DIR}/GUI GUI)
add_subdirectory(${ENGINE_DIR}/World World)
add_subdirectory(${ENGINE_DIR}/Scene Scene)

target_link_libraries(${BLACKBOX_PROJECT} PRIVATE BB_Private)
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE Geometry)
set_target_properties(Geometry PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE Renderer)
set_target_properties(Renderer PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE System)
set_target_properties(System PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Common)
#set_target_properties(Common PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Input)
set_target_properties(Input PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Math)
set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Profiler)
set_target_properties(Profiler PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PUBLIC Resources)
set_target_properties(Resources PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE ScriptSystem)
set_target_properties(ScriptSystem PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE Window)
set_target_properties(Window PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE OpenGlBackend)
set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE Utils)
set_target_properties(Utils PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE Network)
set_target_properties(Network PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE Scene)
set_target_properties(Scene PROPERTIES FOLDER "Engine")
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE World)
set_target_properties(World PROPERTIES FOLDER "Engine")

set_target_properties(GUI PROPERTIES FOLDER "Engine")

target_link_libraries(Common INTERFACE tinyxml2::tinyxml2)
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE ${OPENGL_gl_LIBRARY})
###################################################
generate_export_header(${BLACKBOX_PROJECT}
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/Exports.hpp
)
target_include_directories(${BLACKBOX_PROJECT} PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${BLACKBOX_PROJECT} PRIVATE ${FREETYPE_LIBRARIES})
###################################################
if (UNIX_OR_CYGWIN)
  target_sources(
    ${BLACKBOX_PROJECT}
    PRIVATE ${ENGINE_DIR}/System/Linux/Runtime.cpp
    )
endif()
###################################################
#file(GLOB_RECURSE PRIVATE_INCLUDE_FILES ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/*.hpp ${BLACKBOX_PRIVATE_INCLUDE_DIRS}/*.h)
#file(GLOB_RECURSE PUBLIC_INCLUDE_FILES ${BLACKBOX_PUBLIC_INCLUDE_DIRS}/*.hpp ${BLACKBOX_PUBLIC_INCLUDE_DIRS}/*.h)
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
#[[
target_sources(${BLACKBOX_PROJECT}
  PRIVATE
  ${PRIVATE_INCLUDE_FILES}
  )
source_group(
  TREE ${BLACKBOX_PUBLIC_INCLUDE_DIRS}
  PREFIX Common
  FILES
  ${PUBLIC_INCLUDE_FILES}
  )
]]
target_sources(${BLACKBOX_PROJECT}
  PRIVATE
  ${IMGUI_SOURCES}
  )
###################################################
install (
  TARGETS  ${BLACKBOX_PROJECT}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  #ARCHIVE DESTINATION lib/static
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${BLACKBOX_PROJECT}
)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/media DESTINATION ${BLACKBOX_PROJECT}
)
###################################################
