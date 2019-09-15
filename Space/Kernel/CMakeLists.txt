cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# SpaceKernel includes
# ---------------------------------------------------------
include(OrderMap.cmake)
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
PUBLIC
	OrderMap::OrderMap
PRIVATE
	SpaceStream
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------