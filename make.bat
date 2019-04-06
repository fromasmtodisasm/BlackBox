@echo off
cls & Color 0A

if "%BB_BUILD_DIR%" == "" (
  echo BUILD DIR IS EMPTY
  goto end 
)
if "%BB_SOURCE_DIR%" == "" (
  echo SOURCE DIR IS EMPTY
  goto end 
)
if not exist %BB_BUILD_DIR%\CMakeCache.txt (
  pushd %BB_SOURCE_DIR%
  if not exist %BB_BUILD_DIR% (mkdir %BB_BUILD_DIR%)
  
  cmake -DBUILD_SHARED_LIBS=OFF -DGLM_TEST_ENABLE=OFF -DCMAKE_INSTALL_PREFIX=%BB_INSTALL_DIR%\ %BB_SOURCE_DIR%
  popd
)
cmake --build %BB_BUILD_DIR% --target %1
:end
