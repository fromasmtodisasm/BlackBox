# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindD3D11
# ----------
#
# Try to find Vulkan
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``D3D11::D3D11``, if
# D3D11 has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   D3D11_FOUND          - True if D3D11 was found
#   D3D11_INCLUDE_DIRS   - include directories for D3D11
#   D3D11_LIBRARIES      - link against this library to use D3D11
#
# The module will also define two cache variables::
#
#   D3D11_INCLUDE_DIR    - the D3D11 include directory
#   D3D11_LIBRARY        - the path to the D3D11 library
#

 find_path(D3D11_INCLUDE_DIR d3dx11.h
    PATHS
    "$ENV{DXSDK_DIR}/Include"
    DOC "The directory where D3D11.h resides")

find_library(D3D11_LIBRARY d3d11.lib
    NO_DEFAULT_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_CMAKE_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_SYSTEM_PATH
    PATH_SUFFIXES x64
    PATHS
    "$ENV{DXSDK_DIR}/Lib/x64"
    DOC "The directory where d3d11.lib resides")

find_library(D3DX11_LIBRARY d3dx11.lib
    NO_DEFAULT_PATH
    PATHS
    "$ENV{DXSDK_DIR}/Lib/x64"
    DOC "The directory where d3dx11.lib resides")

set (D3D11_LIBRARIES ${D3D11_LIBRARY} )


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(D3D11
  DEFAULT_MSG
  D3D11_LIBRARY D3DX11_LIBRARY D3D11_INCLUDE_DIR)

mark_as_advanced(D3D11_INCLUDE_DIR D3D11_LIBRARY D3DX11_LIBRARY)

if(D3D11_FOUND AND NOT TARGET D3D11)
  add_library(D3D11 INTERFACE)
  target_link_libraries(D3D11 INTERFACE  ${D3D11_LIBRARIES} ${D3DX11_LIBRARY})
  target_include_directories(D3D11 INTERFACE "${D3D11_INCLUDE_DIR}")
endif()
