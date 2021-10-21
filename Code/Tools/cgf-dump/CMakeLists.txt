cmake_minimum_required(VERSION 3.10)

project(cgf-dump)
add_executable(cgf-dump)
set_target_properties(cgf-dump PROPERTIES FOLDER "Engine/Utils")
target_include_directories(cgf-dump PUBLIC ${CMAKE_SOURCE_DIR}/include/public)
target_compile_definitions(cgf-dump PUBLIC DONT_USE_CRY_MEMORY_MANAGER)
target_compile_definitions(cgf-dump PUBLIC WIN64)

target_sources(cgf-dump 
PRIVATE
	CgfDump.cpp
	CgfUtils.cpp
	CgfUtils.h
	ChunkFileReader.cpp
	ChunkFileReader.h
	CryHeaders.h
	FileMapping.cpp
	FileMapping.h
	crc32.cpp
	crc32.h
	stdafx.cpp
	stdafx.h
)
