cmake_minimum_required (VERSION 3.10)
enable_testing()
# project name
project (SpaceData)
# properties
set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
add_compile_options(-Wall -Wextra)
# definitions
# add_definitions(-DVAR_ORDERED_MAP)
# folder for Applications
add_subdirectory(Applications)
# folder for common use
add_subdirectory(Space)
# folder for common use
add_subdirectory(Test)
# target
add_library(SpaceShare 
INTERFACE
)
# dependencies
target_link_libraries(SpaceShare 
INTERFACE
	Space
)