macro(BuildDocs)
	# Require dot, treat the other components as optional
	find_package(Doxygen
				 COMPONENTS dot
				 OPTIONAL_COMPONENTS mscgen dia)

	 if (DOXYGEN_FOUND)
		doxygen_add_docs(BlackBoxDocs
		"${BLACKBOX_DIR}/Code/"
		"${BLACKBOX_DIR}/Include/"
		"${BLACKBOX_DIR}/Data/Scripts/"
		ALL
		#USE_STAMP_FILE
		WORKING_DIRECTORY
			${BLACKBOX_DIR} 

		)

		set(DOXYGEN_PROJECT_BRIEF ON)
		list(APPEND
		DOXYGEN_EXCLUDE_PATTERNS
			"*/pch.hpp"
		)
	 endif()
endmacro()