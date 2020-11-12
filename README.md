# BlackBox 
[![Build Status](https://travis-ci.org/fromasmtodisasm/BlackBox.svg?branch=master)](https://travis-ci.org/fromasmtodisasm/BlackBox)
[![Build Status](https://travis-ci.org/fromasmtodisasm/BlackBox.svg?branch=develop)](https://travis-ci.org/fromasmtodisasm/BlackBox)
OpenGL based game engine

To build project, make build directory in repository root
and type this commands:

```bash
git clone --recurse-submodules https://github.com/fromasmtodisasm/BlackBox
```

```cmake
#WITH SDL WITHOUT SHARED LIBS
#cmake -Ax64 -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CMAKE} -DBUILD_SHARED_LIBS=OFF -DGLAD_LOADER=ON -DUSE_SDL=ON -DGLM_TEST_ENABLE=OFF -DUSE_GLM=ON -DOPTION_STATIC_LINKING=ON .. 
#cmake -DTOOLCHAIN_FILE=toolchain/linux/Linux_GCC.cmake -DUSE_GLM=ON -DGL_RENDERER=ON -DVCPKG_INSTALLER=ON -DCMAKE_BUILD_TYPE=Debug ..

cmake -Ax64 -DGLAD_LOADER=ON -DUSE_SDL=ON -DGLM_TEST_ENABLE=OFF -DUSE_GLM=ON -DOPTION_STATIC_LINKING=OFF -DGL_RENDERER=ON -DVCPKG_INSTALLER=ON ..
```

builtin terminal emulator
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/terminal.png)
