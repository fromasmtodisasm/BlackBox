# ##############################################################################
sources_platform(ALL)
start_sources()
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Source Files"
		"3DEngine.cpp"
		"MaterialManager.cpp"
		"ObjectManager.cpp"
		"Main.cpp"
	SOURCE_GROUP "Source Files\\\\Geometry"
		"Geometry/Geometry.cpp"
		"Geometry/StatObj.cpp"
		"Geometry/Plane.cpp"
		"Geometry/Primitives.cpp"
		"Geometry/Quad.cpp"
	SOURCE_GROUP "Source Files\\\\Scene"
		"Scene/XmlSerialization.cpp"
	SOURCE_GROUP "Assimp"
		Assimp/CGFImporter.cpp
		Assimp/CGFImporter.h
	SOURCE_GROUP "Assimp\\\\CGF"
		Assimp/CGF/CgfDump.cpp
		Assimp/CGF/CgfDump.h
		Assimp/CGF/CgfUtils.cpp
		Assimp/CGF/CgfUtils.h
		Assimp/CGF/ChunkFileReader.cpp
		Assimp/CGF/ChunkFileReader.h
		Assimp/CGF/CryHeaders.h
		Assimp/CGF/FileMapping.cpp
		Assimp/CGF/FileMapping.h
		Assimp/CGF/crc32.cpp
		Assimp/CGF/crc32.h
		Assimp/CGF/stdafx.cpp
		Assimp/CGF/stdafx.h


)
end_sources()
EngineModule(3DEngine PCH "pch.hpp" SOLUTION_FOLDER "Engine")

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PUBLIC tinyxml2::tinyxml2)
# add_package(assimp)
set(USE_ASSIMP ON)
if(USE_ASSIMP)
	add_package(assimp )
	target_link_libraries(${THIS_PROJECT} PRIVATE assimp::assimp)
endif()

target_compile_definitions(${THIS_PROJECT} PRIVATE -DUSE_FILE_MAPPING)

