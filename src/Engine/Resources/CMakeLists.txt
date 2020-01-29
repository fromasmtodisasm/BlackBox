cmake_minimum_required(VERSION 3.10)
project(Resources CXX)
###################################################
add_library(${PROJECT_NAME})

###################################################

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)
target_link_libraries(${PROJECT_NAME} PRIVATE Render)

target_sources(${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Engine/Resources/MaterialManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/ObjectManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/SceneManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/ShaderManager.cpp
  PRIVATE ${SOURCE_DIR}/Engine/Resources/TextureManager.cpp
)

add_private_headers()
