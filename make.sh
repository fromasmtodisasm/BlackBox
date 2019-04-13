#!/bin/bash

if [ -z "$BB_BUILD_DIR" ]; then 
	echo BB_BUILD_DIR is not set
	exit
fi

if [ -z "$BB_SOURCE_DIR" ]; then 
	echo BB_SOURCE_DIR is not set
	exit
fi

case "$1" in
  remake)
    cd $BB_BUILD_DIR && cmake ${BB_SOURCE_DIR}
  ;;

  *)
    if [ ! -f ${BB_BUILD_DIR}/CMakeCache.txt ]; then
      if [ ! -f ${BB_BUILD_DIR} ]; then
        mkdir -p ${BB_BUILD_DIR} 
      fi
      cd $BB_BUILD_DIR && cmake -DCMAKE_TOOLCHAIN_FILE=CMake/x86_64-w64-mingw32.cmake -DBUILD_SHARED_LIBS=OFF -DGLM_TEST_ENABLE=OFF -DGLAD_INSTALL=ON -DGLAD_LOADER=ON -DUSE_SFML=ON ${BB_SOURCE_DIR} 
      cd -
    fi
    cmake --build $BB_BUILD_DIR --target $1 -- -j
  ;;

esac
