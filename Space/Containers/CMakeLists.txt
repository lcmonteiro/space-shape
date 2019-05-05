cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# SpaceContainer target
# ---------------------------------------------------------
add_library(SpaceContainer 
STATIC 
	SVariable.cpp
)
# includes
target_include_directories(SpaceContainer
PUBLIC
	.
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------