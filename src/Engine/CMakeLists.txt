cmake_minimum_required(VERSION 3.15)
project(BlackBox CXX)
###################################################
include(GenerateExportHeader)
include(FindFreetype)
###################################################
add_library(${PROJECT_NAME})
add_library(Common INTERFACE)
add_library(BB_Private INTERFACE)
###################################################
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine")

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
set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src/Engine)
set(SUBMODULES_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)
set(BLACKBOX_INCLUDE_DIRS ${INCLUDE_DIRECTORIES}/BlackBox)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
###################################################
configure_file(${BLACKBOX_INCLUDE_DIRS}/Profiler/Config.h.in ${CMAKE_BINARY_DIR}/Config.h @ONLY)
###################################################
if (BB_TESET_ENABLE)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DTEST)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DSINGLE_PASS)
endif()
find_package(OpenGL REQUIRED)
find_package(tinyxml2 CONFIG REQUIRED)
find_package(Freetype REQUIRED)
#--------------------------------------------------
target_include_directories(
BB_Private
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Common"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Interfaces/"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Render/"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Resources/"
	INTERFACE "${CMAKE_SOURCE_DIR}/include/BlackBox/Utils/"
	INTERFACE "${SUBMODULES_DIR}/stb/"
)
target_include_directories (
Common
	INTERFACE	"${CMAKE_BINARY_DIR}"
	INTERFACE	"${BLACKBOX_ROOT}/include/"
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

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Geometry)
set_target_properties(Geometry PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE Renderer)
set_target_properties(Renderer PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE System)
set_target_properties(System PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PUBLIC Common)
#set_target_properties(Common PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PUBLIC Input)
set_target_properties(Input PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PUBLIC Math)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PUBLIC Profiler)
set_target_properties(Profiler PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PUBLIC Resources)
set_target_properties(Resources PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE ScriptSystem)
set_target_properties(ScriptSystem PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE Window)
set_target_properties(Window PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)
set_target_properties(Utils PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE Network)
set_target_properties(Network PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE Scene)
set_target_properties(Scene PROPERTIES FOLDER "Engine")
target_link_libraries(${PROJECT_NAME} PRIVATE World)
set_target_properties(World PROPERTIES FOLDER "Engine")

set_target_properties(GUI PROPERTIES FOLDER "Engine")

target_link_libraries(Common INTERFACE tinyxml2::tinyxml2)
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_gl_LIBRARY})
###################################################
generate_export_header(${PROJECT_NAME}
	EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/Exports.hpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FREETYPE_LIBRARIES})
###################################################
if (UNIX_OR_CYGWIN)
  target_sources(
    ${PROJECT_NAME}
    PRIVATE ${ENGINE_DIR}/System/Linux/Runtime.cpp
    )
endif()
###################################################
file(GLOB_RECURSE INCLUDE_FILES ${BLACKBOX_INCLUDE_DIRS}/*.hpp ${BLACKBOX_INCLUDE_DIRS}/*.h)
#--------------------------------------------------
if (LINUX OR UNIX OR APPLE)
	target_link_libraries(BlackBox PUBLIC ${CMAKE_DL_LIBS})
endif()
#--------------------------------------------------
get_target_property(SOURCE_FILES ${PROJECT_NAME}  SOURCES)
source_group(
  TREE ${SOURCE_DIR}
  #PREFIX BlackBox
  FILES  ${SOURCE_FILES}
  )
target_sources(${PROJECT_NAME}
  PRIVATE
  ${INCLUDE_FILES}
  )
source_group(
  TREE ${CMAKE_SOURCE_DIR}/include/BlackBox
  PREFIX BlackBox\\Inc
  FILES
  ${INCLUDE_FILES}
  )
target_sources(${PROJECT_NAME}
  PRIVATE
  ${IMGUI_SOURCES}
  )
###################################################
install (
  TARGETS  ${PROJECT_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  #ARCHIVE DESTINATION lib/static
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/res DESTINATION ${PROJECT_NAME}
)
install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/media DESTINATION ${PROJECT_NAME}
)
###################################################
