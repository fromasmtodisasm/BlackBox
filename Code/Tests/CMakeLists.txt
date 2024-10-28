cmake_minimum_required(VERSION 3.10)
project(Tests)

# Add Google Test
add_subdirectory(${CMAKE_SOURCE_DIR}/googletest googletest)

# Enable testing
enable_testing()

# Add test executable
add_executable(runTests main.cpp tests.cpp)

# Link Google Test libraries
target_link_libraries(runTests gtest gtest_main)
target_compile_options(runTests PRIVATE /EHsc)

# Add tests
add_test(NAME ExampleTest COMMAND runTests)