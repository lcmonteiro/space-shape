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
	Commands/Normalize.cpp
)
# dependencies 
target_link_libraries(MergeXml
PRIVATE
	Space
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------