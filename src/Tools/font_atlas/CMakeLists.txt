cmake_minimum_required(VERSION 3.10)

project(atlas)

add_executable(atlas freetype-atlas.c)

ADD_PACKAGE(Stb)
ADD_PACKAGE(freetype)
if (LINUX)
	target_link_libraries(${PROJECT_NAME} PRIVATE freetype)
else()
	target_link_libraries(${PROJECT_NAME} PRIVATE Freetype::Freetype)
endif()
set_target_properties(atlas PROPERTIES FOLDER "Engine/Utils")