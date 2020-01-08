cmake_minimum_required (VERSION 3.8)
# ---------------------------------------------------------
# project
# ---------------------------------------------------------
project (TranslateXml)
# ---------------------------------------------------------
# MergeXml target
# ---------------------------------------------------------
add_executable(TranslateXml 
	main.cpp
)
# set options
set_target_properties(TranslateXml
PROPERTIES 
	LINK_FLAGS	"-static -static-libgcc -static-libstdc++"
)
# dependencies 
target_link_libraries(TranslateXml
PRIVATE
	Space
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------