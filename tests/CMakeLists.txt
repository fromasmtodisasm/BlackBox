cmake_minimum_required(VERSION 3.0)
project(BlackBox_test CXX)

add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(BlackBox_test BlackBox)
include_directories(${CMAKE_SOURCE_DIR}/include)

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


install(
  TARGETS ${PROJECT_NAME} 
  RUNTIME DESTINATION BlackBox/bin
)
