#!/bin/bash
case "$1" in
  remake)
    cd $BB_BUILD_DIR && cmake ../..
  ;;

  *)
    cmake --build $BB_BUILD_DIR --target $1 
  ;;

esac
