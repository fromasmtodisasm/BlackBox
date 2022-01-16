cmake_minimum_required(VERSION 3.2)
project(ImGUI CXX)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/submodules/imgui)
set(IMGUI-SFML_DIR ${CMAKE_SOURCE_DIR}/submodules/imgui-sfml)

include_directories(
  ${PROJECT_NAME} PUBLIC ${IMGUI_DIR}
)
add_library(${PROJECT_NAME})
target_sources(
  ${PROJECT_NAME}
  PRIVATE ${IMGUI_DIR}/imgui.cpp
  PRIVATE ${IMGUI_DIR}/imgui_draw.cpp
  PRIVATE ${IMGUI-SFML_DIR}/imgui-SFML.cpp
  PRIVATE ${IMGUI_DIR}/imgui_demo.cpp
  )
target_include_directories(
  ${PROJECT_NAME} PUBLIC ${IMGUI_DIR}
)
target_include_directories(
  ${PROJECT_NAME} PUBLIC ${IMGUI-SFML_DIR}
)

target_include_directories(
  ${PROJECT_NAME} PUBLIC "${SFML_SOURCE_DIR}/include"
)

