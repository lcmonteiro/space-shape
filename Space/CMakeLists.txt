cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# Space project
# ---------------------------------------------------------
project (Space)
# folder for core Kernel
add_subdirectory(Kernel)
# folder for Converters
add_subdirectory(Converters)
# folder for Tools
add_subdirectory(Tools)
# folder for resources
add_subdirectory(Resources)
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
	SpaceConverter
	SpaceResource
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------