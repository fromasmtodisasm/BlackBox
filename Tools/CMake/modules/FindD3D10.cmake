# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

#.rst:
# FindD3D10
# ----------
#
# Try to find Vulkan
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``D3D10::D3D10``, if
# D3D10 has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables::
#
#   D3D10_FOUND          - True if D3D10 was found
#   D3D10_INCLUDE_DIRS   - include directories for D3D10
#   D3D10_LIBRARIES      - link against this library to use D3D10
#
# The module will also define two cache variables::
#
#   D3D10_INCLUDE_DIR    - the D3D10 include directory
#   D3D10_LIBRARY        - the path to the D3D10 library
#
 find_path(D3D10_INCLUDE_DIR d3dx11.h
    PATHS
    "$ENV{DXSDK_DIR}/Include"
    DOC "The directory where D3D10.h resides")

find_library(D3D10_LIBRARY d3d10.lib
    PATHS
    "$ENV{DXSDK_DIR}/Lib/x64"
    DOC "The directory where d3d11.lib resides")

find_library(D3DX10_LIBRARY d3dx10.lib
    PATHS
    "$ENV{DXSDK_DIR}/Lib/x64"
    DOC "The directory where d3dx11.lib resides")

set (D3D10_LIBRARIES ${D3D10_LIBRARY} )


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(D3D10
  DEFAULT_MSG
  D3D10_LIBRARY D3DX10_LIBRARY D3D10_INCLUDE_DIR)

mark_as_advanced(D3D10_INCLUDE_DIR D3D10_LIBRARY D3DX10_LIBRARY)

if(D3D10_FOUND AND NOT TARGET D3D10)
  add_library(D3D10 INTERFACE)
  target_link_libraries(D3D10 INTERFACE  ${D3D10_LIBRARIES} ${D3DX10_LIBRARY})
  target_include_directories(D3D10 INTERFACE "${D3D10_INCLUDE_DIR}")
endif()
