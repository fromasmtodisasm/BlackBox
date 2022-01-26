cmake_minimum_required(VERSION 3.10)
# ##############################################################################
# ##############################################################################
start_sources()
sources_platform(ALL)
add_sources(
  "NoUberFile"
  PROJECTS
  Input
  SOURCE_GROUP
  "Source Files"
  "Input.cpp"
  "ActionMap.cpp"
  "ActionMapManager.cpp"
  "BaseInput.cpp"
  "InputActionMap.cpp"
  "InputCVars.cpp"
  "InputDevice.cpp"
  "InputHandler.cpp"
  "SDLInput.cpp"
  "SDLKeyboard.cpp"
  "SDLMouse.cpp"
  "SDLPad.cpp")
end_sources()

enginemodule(Input FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Engine")

if(OPTION_USE_LINUXINPUT)
  target_compile_definitions(${THIS_PROJECT} PUBLIC -DUSE_LINUXINPUT)
endif()
message(STATUS "Using sdl for Input System")
# ##############################################################################
set(PROJECT_NAME ${THIS_PROJECT})
add_sdl(${THIS_PROJECT})
# ##############################################################################
# target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)

target_include_directories(
  ${THIS_PROJECT}
  PRIVATE "${CMAKE_BINARY_DIR}"
  PRIVATE "${CMAKE_SOURCE_DIR}/include/"
  # PRIVATE $<TARGET_PROPERTY:SDL2,INTERFACE_INCLUDE_DIRECTORIES>
)
