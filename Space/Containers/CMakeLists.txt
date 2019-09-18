cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# SpaceContainer includes
# ---------------------------------------------------------
include(OrderMap.cmake)
# ---------------------------------------------------------
# SpaceContainer target
# ---------------------------------------------------------
add_library(SpaceContainer INTERFACE)
# includes
target_include_directories(SpaceContainer 
INTERFACE 
	.
)
# dependencies 
target_link_libraries(SpaceContainer 
INTERFACE
	OrderMap
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------