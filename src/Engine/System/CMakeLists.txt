cmake_minimum_required(VERSION 3.15)
project(System CXX)
###################################################
add_library(System)

###################################################

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/System/Commands.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/Console.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/Linux/Runtime.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/Log.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/System.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/SystemEventDispatcher.cpp
  PRIVATE ${SOURCE_DIR}/Engine/System/Timer.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${BLACKBOX_ROOT}/include/System)
