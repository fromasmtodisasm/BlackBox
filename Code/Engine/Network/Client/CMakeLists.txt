# CMakeList.txt : CMake project for Client, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (Client "Client.cpp" "Client.h")

target_sources(Client 
	PRIVATE
		users.txt
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Client PROPERTY CXX_STANDARD 20)
endif()

find_package(imgui CONFIG REQUIRED)
target_link_libraries(Client PRIVATE Common)
# TODO: Add tests and install targets if needed.
