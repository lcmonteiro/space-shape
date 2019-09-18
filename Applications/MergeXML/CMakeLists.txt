cmake_minimum_required (VERSION 3.10)
# ---------------------------------------------------------
# project
# ---------------------------------------------------------
project (MergeXml)
# ---------------------------------------------------------
# MergeXml target
# ---------------------------------------------------------
add_executable(MergeXml 
	main.cpp
)
# includes
target_include_directories(MergeXml
PUBLIC
	Commands
)
# set options
set_target_properties(MergeXml
PROPERTIES 
	LINK_FLAGS	"-static-libgcc -static-libstdc++"
)
# dependencies 
target_link_libraries(MergeXml
PUBLIC
	Space
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------