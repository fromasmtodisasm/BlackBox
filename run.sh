#!/bin/bash

clear
./build.sh

EXE=""
case `uname` in
  MINGW*)
    EXE=WindowsLauncher/Debug/Launcher;;
  LINUX*)
    EXE=LinuxLauncher/Launcher;;
  *)
    echo "Unknown OS"
esac


./build/src/Launcher/$EXE "$1"
