cmake_minimum_required (VERSION 3.8)
# 
include(Pugixml.cmake)
# ---------------------------------------------------------
# SpaceKernel target
# ---------------------------------------------------------
add_library(SpaceConverter 
STATIC 
	SConvertJSON.cpp
	SConvertXML.cpp
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
	SpaceKernel
	Pugixml::Pugixml
)
# ---------------------------------------------------------
# End
# ---------------------------------------------------------