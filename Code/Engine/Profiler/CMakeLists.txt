cmake_minimum_required(VERSION 3.10)
project(Profiler CXX)
###################################################
add_library(Profiler)

###################################################

target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Drawer2D.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/HP_Timer.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Profiler.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Tgaloader.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Thread.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Profiler/Utils.cpp
)

add_private_headers()
