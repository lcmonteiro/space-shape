cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# Space project
# ---------------------------------------------------------
project (Space)
# folder for core Kernel
add_subdirectory(Kernel)
# folder for Converters
add_subdirectory(Converters)
# folder for Containers
add_subdirectory(Containers)
# folder for Tools
add_subdirectory(Tools)
# folder for resources
add_subdirectory(Resources)
# folder for resources
add_subdirectory(Streams)
# ---------------------------------------------------------
# Space target
# ---------------------------------------------------------
add_library(Space INTERFACE)
# includes
target_include_directories(Space 
INTERFACE
	.
)
# dependencies 
target_link_libraries(Space
INTERFACE
	SpaceKernel
	SpaceTool
	SpaceStream
	SpaceConverter
	SpaceResource
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------