cmake_minimum_required(VERSION 3.10)
project(Launcher CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(Launcher PRIVATE  System )
target_link_libraries(Launcher PRIVATE  Common)
target_link_libraries(Launcher PRIVATE  FPS_Game)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)


if(WIN32)
  if (MSVC)
    #SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
    #set_target_properties(${PROJECT_NAME}_test PROPERTIES CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows)
  endif()
  if (MINGW)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--subsystem,windows")
  endif()
endif(WIN32)

add_test(NAME mytest COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

if (BUILD_SHARED_LIBS)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
		COMMAND ${CMAKE_COMMAND} -E copy  # which executes "cmake - E copy_if_different..."
			$<TARGET_FILE_DIR:BlackBox>/BlackBox.dll
			$<TARGET_FILE_DIR:Launcher>)      # <--this is in-file
endif()

add_test(NAME TestLauncher COMMAND Launcher -dedicated)

install(
  TARGETS ${PROJECT_NAME} 
  RUNTIME DESTINATION bin
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Launcher)
set_solution_startup_target(Launcher)
#set_property(DIRECTORY . PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
