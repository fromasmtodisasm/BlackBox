# additional target to perform clang-format run, requires clang-format

# get all project files

function(clang_format SOURCE_FILES)
  file(GLOB_RECURSE ALL_SOURCE_FILES ${CMAKE_SOURCE_DIR}/*.cpp ${CMAKE_SOURCE_DIR}/*.h ${CMAKE_SOURCE_DIR}/*.hpp)

  message(STATUS "${SOURCE_FILES}")
  add_custom_target(
          clangformat
          #COMMAND /usr/bin/clang-format
          COMMAND clang-format
          -style=file
          -i
          --sort-includes
          --verbose
          ${ALL_SOURCE_FILES}
  )
set_target_properties(clangformat PROPERTIES FOLDER "Engine/Utils")
endfunction()
