# ##############################################################################
sources_platform(ALL)
start_sources()
add_sources(
	"NoUberFile"
	PROJECTS
	3DEngine
	SOURCE_GROUP
	"Source Files"
	# Render folder
	"3DEngine.cpp"
	"MaterialManager.cpp"
	"ObjectManager.cpp"
	"SceneManager.cpp"
	"Main.cpp"
	SOURCE_GROUP
	"Source Files\\\\Geometry"
	"Geometry/Geometry.cpp"
	"Geometry/ObjLoader.cpp"
	"Geometry/StatObj.cpp"
	"Geometry/Plane.cpp"
	"Geometry/Primitives.cpp"
	"Geometry/Quad.cpp"
	SOURCE_GROUP
	"Source Files\\\\Scene"
	"Scene/Scene.cpp"
	"Scene/SceneSerialization.cpp"
	"Scene/XmlSerialization.cpp"
	"World/World.cpp")
end_sources()
enginemodule(3DEngine FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Engine")

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PUBLIC tinyxml2::tinyxml2)
# add_package(assimp)
set(USE_ASSIMP ON)
if(USE_ASSIMP)
	find_package(assimp CONFIG REQUIRED)
	target_link_libraries(${THIS_PROJECT} PRIVATE assimp::assimp)
endif()

# add_private_headers() add_subdirectory(Tools/xml_scene_to_lua)
