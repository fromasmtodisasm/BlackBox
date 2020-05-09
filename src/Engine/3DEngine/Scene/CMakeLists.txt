#cmake_minimum_required(VERSION 3.10)
#project(Scene CXX)
###################################################
#add_library(Scene STATIC)

###################################################

#target_link_libraries(${PROJECT_NAME} PRIVATE Math)
#target_link_libraries(${PROJECT_NAME} PRIVATE Renderer)
target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2::tinyxml2)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/Renderer/Scene/Scene.cpp
  PRIVATE ${ENGINE_DIR}/Renderer/Scene/SceneSerialization.cpp
  PRIVATE ${ENGINE_DIR}/Renderer/Scene/XmlSerialization.cpp
)
#install_this(Scene)
#add_private_headers()

