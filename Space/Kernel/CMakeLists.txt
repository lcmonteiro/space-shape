cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# SpaceKernel target
# ---------------------------------------------------------
add_library(SpaceKernel
	SVariable.cpp
	STime.cpp
	SLog.cpp
)
# includes
target_include_directories(SpaceKernel
PUBLIC
	.
)
# connect
target_link_libraries(SpaceKernel
PRIVATE
	SpaceStream
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------