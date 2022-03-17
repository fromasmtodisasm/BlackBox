start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Source Files"
		"pch.hpp"
		"resource.h"
		"ResourceCompiler.h"
		"ResourceCompiler.cpp"
		"ResourceCompilerPlugin.def"
		"FileMapping.cpp"
		"FileMapping.h"
		"Pak.cpp"
		"ZipFileFormat.h"
		"ZipFileFormat_info.h"
	SOURCE_GROUP "CommonTools"
		"../../CommonTools/ini.h"
		"../../CommonTools/ini.c"
)
end_sources()
find_package(Crc32c CONFIG REQUIRED)
find_package(ZLIB REQUIRED)

CryResourceCompiler(ResourceCompiler PCH "pch.hpp" SOLUTION_FOLDER "BLACKBOX/Tools/RC")
target_include_directories(${THIS_PROJECT} PRIVATE
	"${BLACKBOX_SOURCE_DIR}/Code/Tools/CommonTools" 
	#"${BLACKBOX_SOURCE_DIR}/Code/Tools/CryXML" 
	#"${BLACKBOX_SOURCE_DIR}/Code/CryEngine/CrySystem/XML"
	)
#target_link_libraries(${THIS_PROJECT} PRIVATE zlib md5 psapi)
target_compile_options(${THIS_PROJECT} PRIVATE /EHsc)

target_compile_definitions(${THIS_PROJECT} PRIVATE -DCRYTOOLS -DRESOURCECOMPILERPC_EXPORTS -DOFFLINE_COMPUTATION -DRC_COMPILER)

target_link_libraries(${THIS_PROJECT} PRIVATE Crc32c::crc32c)
target_link_libraries(${THIS_PROJECT} PRIVATE ZLIB::ZLIB)
