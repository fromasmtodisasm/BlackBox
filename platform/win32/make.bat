@echo off
if "%BB_BUILD_DIR%" = "" (
  echo BUILD DIR IS EMPTY
  exit
)
if "%BB_SOURCE_DIR%" = "" (
  echo SOURCE DIR IS EMPTY
  exit
)
if not exist %BB_BUILD_DIR%\CMakeCache.txt (
  pushd
  cd %BB_BUILD_DIR% && cmake -DBUILD_SHARED_LIBS=OFF -DGLM_TEST_ENABLE=OFF %BB_SOURCE_DIR%
  popd
)
cmake --build %BB_BUILD_DIR% --target %1
