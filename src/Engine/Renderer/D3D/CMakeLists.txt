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

 FIND_PATH(DX_INCLUDE_PATH d3d10.h
    PATHS
    "$ENV{DXSDK_DIR}/Include"
    "C:/Program Files (x86)/Microsoft DirectX SDK/Include"
    DOC "The directory where D3D10.h resides")

find_library(D3D10_LIBRARY d3d10.lib
    PATHS
    "$ENV{DXSDK_DIR}/Lib/x86"
    "C:/Program Files (x86)/Microsoft DirectX SDK/Lib/x86"
    DOC "The directory where d3d10.lib resides")

find_library(D3DX10_LIBRARY d3dx10.lib
    PATHS
    "$ENV{DXSDK_DIR}/Lib/x86"
    "C:/Program Files (x86)/Microsoft DirectX SDK/Lib/x86"
    DOC "The directory where d3dx10.lib resides")


set (D3D11_FOUND "YES")
set (D3D11_LIBRARIES ${D3D11_LIB})

add_compile_definitions("DX_RENDERER")
target_link_libraries(D3DBackend INTERFACE d3d10 d3dcompiler dxguid)
target_link_libraries(${PROJECT_NAME} PUBLIC D3DBackend)
target_include_directories(${PROJECT_NAME} PRIVATE ${DX_INCLUDE_PATH})
