#!/bin/bash
case "$1" in
  remake)
    cd $BB_BUILD_DIR && cmake ${BB_SOURCE_DIR}
  ;;

  *)
    if [ ! -f ${BB_BUILD_DIR}/CMakeCache.txt ]; then
      cd $BB_BUILD_DIR && cmake -DCMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake -DBUILD_SHARED_LIBS=OFF -DGLM_TEST_ENABLE=OFF ${BB_SOURCE_DIR} 
      cd -
    fi
    cmake --build $BB_BUILD_DIR --target $1 -- -j8  
  ;;

esac
