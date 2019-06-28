cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# SpaceTool target
# ---------------------------------------------------------
add_library(SpaceTool
STATIC 
	SEdit.cpp
	SLogic.cpp
	SSearch.cpp
	SPattern.cpp
)
# includes
target_include_directories(SpaceTool
PUBLIC
	.
)
# dependencies 
target_link_libraries(SpaceTool
PUBLIC
	SpaceKernel
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------