cmake_minimum_required(VERSION 3.15)
project(BlackBox)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(game)
add_subdirectory(tests)
add_subdirectory(res)

