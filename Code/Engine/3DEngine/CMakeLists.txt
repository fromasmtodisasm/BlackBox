# ##############################################################################
sources_platform(ALL)
start_sources()
add_sources(
	"NoUberFile"
	PROJECTS
	3DEngine
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
