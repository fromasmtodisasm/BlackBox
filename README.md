# BlackBox 
[![Build Status](https://travis-ci.org/fromasmtodisasm/BlackBox.svg?branch=master)](https://travis-ci.org/fromasmtodisasm/BlackBox)

OpenGL based game engine

To build project, make build directory in repository root
and type this commands:

```bash
git clone --recurse-submodules https://github.com/fromasmtodisasm/BlackBox
cd BlackBox
mkdir build
cd build
export VCPKG_CMAKE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
cmake -DCMAKE_TOOLCHAIN_FILE=${VCPKG_CMAKE} -DBUILD_SHARED_LIBS=OFF -DGLAD_LOADER=ON -DUSE_SFML=ON -DGLM_TEST_ENABLE=OFF -DUSE_GLM=ON ..
```

reflecttion
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/reflect.png)
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/texture_fix.png)

builtin terminal emulator
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/terminal.png)

rotation script demo

[![IMAGE ALT TEXT HERE](https://i.ytimg.com/vi/Ud7BUYordZA/1.jpg)](https://www.youtube.com/watch?v=Ud7BUYordZA&feature=youtu.be)
