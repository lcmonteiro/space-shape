cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# SpaceKernel target
# ---------------------------------------------------------
add_library(SpaceKernel 
STATIC 
	SVariable.cpp
)
# includes
target_include_directories(SpaceKernel
PUBLIC
	.
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------