cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# Select platform
# ---------------------------------------------------------
if(MSVC OR MINGW)
    set(platform Windows)
else()
	set(platform Linux)
endif()
# ---------------------------------------------------------
# SpaceResource target
# ---------------------------------------------------------
add_library(SpaceResource 
	${platform}/SFileSystem.cpp
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