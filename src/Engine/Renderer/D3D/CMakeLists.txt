###################################################
add_library(D3DBackend INTERFACE)
###################################################

set(D3D_DIR ${PROJECT_SOURCE_DIR}/D3D)

install_this(D3DBackend)

target_sources(${PROJECT_NAME}
  PRIVATE 
  ${D3D_DIR}/Renderer.cpp
  ${D3D_DIR}/BufferManager.cpp
  ${D3D_DIR}/Shader.cpp
  ${D3D_DIR}/AuxRenderer.h
  ${D3D_DIR}/AuxRenderer.cpp
 )

add_compile_definitions("DX_RENDERER")
target_link_libraries(D3DBackend INTERFACE d3d10 d3dcompiler)
target_link_libraries(${PROJECT_NAME} PUBLIC D3DBackend)
