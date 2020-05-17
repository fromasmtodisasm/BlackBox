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
#WITH SFML WITHOUT SHARED LIBS
cmake -DBUILD_SHARED_LIBS=OFF -DGLAD_LOADER=ON -DUSE_SFML=ON -D GLM_TEST_ENABLE=OFF -DUSE_GLM=ON ${CMAKE_SOURCE_DIR}
```

```cmake
#WITH SDL WITHOUT SHARED LIBS
#cmake -Ax64 -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CMAKE} -DBUILD_SHARED_LIBS=OFF -DGLAD_LOADER=ON -DUSE_SDL=ON -DGLM_TEST_ENABLE=OFF -DUSE_GLM=ON -DOPTION_STATIC_LINKING=ON .. 
#cmake "-DCMAKE_TOOLCHAIN_FILE=../../vcpkg/scripts/buildsystems/vcpkg.cmake" "-DTOOLCHAIN_FILE=toolchain/linux/Linux_GCC.cmake" -DCMAKE_BUILD_TYPE=Debug  ..

cmake -Ax64 -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CMAKE} -DGLAD_LOADER=ON -DUSE_SDL=ON -DGLM_TEST_ENABLE=OFF -DUSE_GLM=ON -DOPTION_STATIC_LINKING=OFF .. 
```

reflecttion
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/reflect.png)
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/texture_fix.png)

builtin terminal emulator
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/terminal.png)

rotation script demo

[![IMAGE ALT TEXT HERE](https://i.ytimg.com/vi/Ud7BUYordZA/1.jpg)](https://www.youtube.com/watch?v=Ud7BUYordZA&feature=youtu.be)
