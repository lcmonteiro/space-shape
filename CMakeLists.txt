cmake_minimum_required (VERSION 3.10)
enable_testing()
# project name
project (SpaceData)
# properties
set(CMAKE_CXX_STANDARD 14)
# folder for Applications
add_subdirectory(Applications)
# folder for common use
add_subdirectory(Space)
# folder for common use
add_subdirectory(Test)
# target
add_library(SpaceShare 
INTERFACE
)
# dependencies
target_link_libraries(SpaceShare 
INTERFACE
	Space
)