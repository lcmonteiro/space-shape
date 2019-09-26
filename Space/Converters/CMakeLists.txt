cmake_minimum_required (VERSION 3.8)
# 
include(Pugixml.cmake)
# ---------------------------------------------------------
# SpaceKernel target
# ---------------------------------------------------------
add_library(SpaceConverter 
	SConvertJSON.cpp
	SConvertBin.cpp
	SConvertXML.cpp
	SConvertB64.cpp
	SConvertCSV.cpp
	SConvertARG.cpp
)
# includes
target_include_directories(SpaceConverter
PUBLIC
	.
)
# dependencies 
target_link_libraries(SpaceConverter
PUBLIC
	SpaceKernel
	Pugixml::Pugixml
PRIVATE
	SpaceTool
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------