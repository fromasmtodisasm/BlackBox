cmake_minimum_required(VERSION 3.10)
project(Launcher CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_executable(${PROJECT_NAME} 
	Main.cpp
    "Resource.rc"
)
target_link_libraries(Launcher PRIVATE  BlackBox)
target_link_libraries(Launcher PRIVATE  System)
add_dependencies(Launcher Game)
#target_link_libraries(Launcher PRIVATE  FPS_Game)


if(WIN32)
  if (MSVC)
    #SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows /ENTRY:main")
    #set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows)
  endif()
  if (MINGW)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,windows")
  endif()
endif(WIN32)

add_test(NAME mytest COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

install(
  TARGETS ${PROJECT_NAME} 
  RUNTIME DESTINATION bin
)

install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ DESTINATION bin
)
#message(FATAL_ERROR "bindir:" ${CMAKE_CURRENT_BINARY_DIR}$<CONFIG>)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Launcher)
set_solution_startup_target(Launcher)
#set_property(DIRECTORY . PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
