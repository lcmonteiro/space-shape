cmake_minimum_required (VERSION 3.8)
# 
include(Pugixml.cmake)
# ---------------------------------------------------------
# SpaceContainer target
# ---------------------------------------------------------
add_library(SpaceConverter 
STATIC 
	SB64Convert.cpp
	#CsvConvert.cpp
)
# includes
target_include_directories(SpaceConverter
PUBLIC
	.
)
# dependencies 
target_link_libraries(SpaceConverter
PUBLIC
	SpaceContainer
	Pugixml::Pugixml
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------