cmake_minimum_required (VERSION 3.10)
# project name
project (SpaceData)
# ---------------------------------------------------------
# project properties 
# ---------------------------------------------------------
enable_testing()
# properties
set(CMAKE_CXX_STANDARD 14)
# options
add_compile_options(-Wall -Wextra)
# definitions
# add_definitions(-DVAR_ORDERED_MAP)
# ---------------------------------------------------------
# subscrive subdirectories 
# ---------------------------------------------------------
# folder for Applications
add_subdirectory(Applications)
# folder for common use
add_subdirectory(Space)
# folder for common use
add_subdirectory(Test)
# ---------------------------------------------------------
# SpaceShare target
# ---------------------------------------------------------
add_library(SpaceShare 
INTERFACE
)
# dependencies
target_link_libraries(SpaceShare 
INTERFACE
	Space
)
# -----------------------------------------------------------------------------
# End
# -----------------------------------------------------------------------------