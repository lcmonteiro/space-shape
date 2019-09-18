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
	/mnt/c/Workspace/space-shape/Bin/Space/Containers/ordered_map-prefix/src/ordered_map/include/tsl
)
# connect
target_link_libraries(SpaceKernel
PUBLIC
	SpaceContainer
PRIVATE
	SpaceStream
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------