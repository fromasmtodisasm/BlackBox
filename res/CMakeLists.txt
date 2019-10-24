cmake_minimum_required(VERSION 3.0)
project(resources LANGUAGES)

add_library(${PROJECT_NAME} EXCLUDE_FROM_ALL)

file(GLOB_RECURSE res_files ${CMAKE_SOURCE_DIR}/res/*)

target_sources(${PROJECT_NAME} PRIVATE ${res_files})
target_sources(${PROJECT_NAME} PRIVATE dummy.c)

source_group(
  TREE ${CMAKE_SOURCE_DIR}/res
  PREFIX resources 
  FILES
  ${res_files}
  )
