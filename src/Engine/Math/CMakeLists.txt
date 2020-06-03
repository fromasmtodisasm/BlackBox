cmake_minimum_required(VERSION 3.10)
project(Math CXX)
###################################################
add_library(Math INTERFACE)

if (OPTION_USE_GLM)
  add_package(glm)
  include_directories(${GLM_INCLUDE_DIRS})
  target_include_directories(${PROJECT_NAME} INTERFACE ${GLM_INCLUDE_DIR})
  target_compile_definitions(${PROJECT_NAME} INTERFACE -DUSE_GLM)
  target_compile_definitions(${PROJECT_NAME} INTERFACE -DGLM_ENABLE_EXPERIMENTAL)
endif()

install_this(Math)
###################################################
