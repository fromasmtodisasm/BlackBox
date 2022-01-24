add_library(BlackBoxEngine INTERFACE IMPORTED)

set (BLACKBOX_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/../../..")

set_property(TARGET BlackBoxEngine PROPERTY INTERFACE_INCLUDE_DIRECTORIES
	"${BLACKBOX_SOURCE_DIR}/Include/public/BlackBox/Common"
	#"${BLACKBOX_SOURCE_DIR}/Include/public/BlackBox/3rdParty"
)