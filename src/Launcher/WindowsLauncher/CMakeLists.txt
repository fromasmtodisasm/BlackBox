cmake_minimum_required(VERSION 3.10)

project(Launcher CXX)
set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_executable(${PROJECT_NAME} WIN32
	Main.cpp
    #"Resource.rc"
)
add_definitions(-D_LAUNCHER)
target_link_libraries(Launcher PRIVATE  BlackBox)
target_link_libraries(Launcher PRIVATE  System)
target_link_libraries(Launcher PRIVATE  Steam)
add_dependencies(Launcher Game)

add_test(NAME mytest COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

install(
  TARGETS ${PROJECT_NAME} 
  RUNTIME DESTINATION bin
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Launcher)
set_solution_startup_target(Launcher)
