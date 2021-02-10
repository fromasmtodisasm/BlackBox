###################################################
add_library(D3DBackend INTERFACE)
###################################################

set(D3D_DIR ${CMAKE_CURRENT_SOURCE_DIR}/D3D)
install_this(D3DBackend)
find_package(D3D11 REQUIRED)
add_compile_definitions("DX_RENDERER")
target_link_libraries(D3DBackend INTERFACE ${D3D11_LIBRARIES} d3dcompiler dxguid)
