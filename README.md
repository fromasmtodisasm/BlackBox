# BlackBox 
[![Build Status](https://travis-ci.org/fromasmtodisasm/BlackBox.svg?branch=master)](https://travis-ci.org/fromasmtodisasm/BlackBox)

OpenGL based game engine

To build project, make build directory in repository root
and type this commands:

```bash
git clone --recurse-submodules https://github.com/fromasmtodisasm/BlackBox
```

```cmake
cmake -DBUILD_SHARED_LIBS=OFF -DGLAD_LOADER=ON -DUSE_SFML=ON -D GLM_TEST_ENABLE=OFF ${CMAKE_SOURCE_DIR}
```

reflecttion
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/reflect.png)
![alt text](https://raw.githubusercontent.com/fromasmtodisasm/BlackBox/develop/screen_shots/texture_fix.png)
