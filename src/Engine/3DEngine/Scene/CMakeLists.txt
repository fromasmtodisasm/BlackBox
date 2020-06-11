#cmake_minimum_required(VERSION 3.10)
#project(Scene CXX)
###################################################
#add_library(Scene STATIC)

###################################################

if (LINUX)
	target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2)
else()
	target_link_libraries(${PROJECT_NAME} PUBLIC tinyxml2::tinyxml2)
endif()

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/3DEngine/Scene/Scene.cpp
  PRIVATE ${ENGINE_DIR}/3DEngine/Scene/SceneSerialization.cpp
  PRIVATE ${ENGINE_DIR}/3DEngine/Scene/XmlSerialization.cpp
)
#install_this(Scene)
#add_private_headers()

