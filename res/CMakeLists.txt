#project(resources LANGUAGES)

file(GLOB_RECURSE res_files ${CMAKE_SOURCE_DIR}/res/*)

source_group(
  TREE ${CMAKE_SOURCE_DIR}/res
  #PREFIX Resources 
  FILES
  ${res_files}
  )

add_custom_target( Data SOURCES ${res_files})
set_property(TARGET Data PROPERTY FOLDER "Data")
