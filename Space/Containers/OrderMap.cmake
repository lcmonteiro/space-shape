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
# find project propreties 
# -----------------------------------------------------------------------------
ExternalProject_Get_Property(ordered_map source_dir)
set(MAP_INCLUDE_DIRS ${source_dir}/include/tsl)
file(MAKE_DIRECTORY  ${MAP_INCLUDE_DIRS})
# -----------------------------------------------------------------------------
# OrderMap target
# -----------------------------------------------------------------------------
add_library(OrderMap INTERFACE)
# includes
message("---------------- ${MAP_INCLUDE_DIRS}")
target_include_directories(OrderMap 
INTERFACE 
    MAP_INCLUDE_DIRS
)
# -----------------------------------------------------------------------------
# End
# -----------------------------------------------------------------------------
