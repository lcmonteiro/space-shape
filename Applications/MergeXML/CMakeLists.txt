cmake_minimum_required (VERSION 3.8)
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
# includes
target_include_directories(MergeXml
PUBLIC
	Commands
)
# dependencies 
target_link_libraries(MergeXml
PUBLIC
	Space
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------