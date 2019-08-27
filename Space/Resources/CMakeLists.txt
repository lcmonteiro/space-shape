cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# SpaceResource target
# ---------------------------------------------------------
add_library(SpaceResource 
	Linux/SFileSystem.cpp
)
# includes
target_include_directories(SpaceResource
PUBLIC
	.
)
# dependencies 
target_link_libraries(SpaceResource
PUBLIC
	SpaceKernel
	SpaceConverter
	SpaceTool
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------