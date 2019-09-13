include(ExternalProject)
# -----------------------------------------------------------------------------
# add external project
# -----------------------------------------------------------------------------
ExternalProject_Add(
    ordered_map
    GIT_REPOSITORY https://github.com/Tessil/ordered-map
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)
# -----------------------------------------------------------------------------
# project propreties 
# -----------------------------------------------------------------------------
ExternalProject_Get_Property(ordered_map source_dir binary_dir)
#set(MAP_INCLUDE_DIRS ${source_dir}/src)
set(MAP_INCLUDE_DIRS ${source_dir}/include/tsl)
set(MAP_LIBRARY_PATH ${binary_dir}/${CMAKE_STATIC_LIBRARY_PREFIX}ordered_map${CMAKE_STATIC_LIBRARY_SUFFIX})
file(MAKE_DIRECTORY  ${MAP_INCLUDE_DIRS})

message("--------------${MAP_INCLUDE_DIRS}")
# -----------------------------------------------------------------------------
# Pugixml target
# -----------------------------------------------------------------------------
set(MAP_LIBRARY OrderMap::OrderMap)
add_library(${MAP_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${MAP_LIBRARY} PROPERTIES
    "IMPORTED_LOCATION" "${MAP_LIBRARY_PATH}"
    "INTERFACE_INCLUDE_DIRECTORIES" "${MAP_INCLUDE_DIRS}")
add_dependencies(${MAP_LIBRARY} ordered_map)
# -----------------------------------------------------------------------------
# End
# -----------------------------------------------------------------------------
