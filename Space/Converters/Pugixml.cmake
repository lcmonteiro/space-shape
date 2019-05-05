include(ExternalProject)
# -----------------------------------------------------------------------------
# add external project
# -----------------------------------------------------------------------------
ExternalProject_Add(
    pugixml
    GIT_REPOSITORY https://github.com/zeux/pugixml.git
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)
# -----------------------------------------------------------------------------
# project propreties 
# -----------------------------------------------------------------------------
ExternalProject_Get_Property(pugixml source_dir binary_dir)
set(PUGIXML_INCLUDE_DIRS ${source_dir}/src)
set(PUGIXML_LIBRARY_PATH ${binary_dir}/${CMAKE_FIND_LIBRARY_PREFIXES}pugixml.a)
file(MAKE_DIRECTORY ${PUGIXML_INCLUDE_DIRS})
# -----------------------------------------------------------------------------
# Pugixml target
# -----------------------------------------------------------------------------
set(PUGIXML_LIBRARY Pugixml::Pugixml)
add_library(${PUGIXML_LIBRARY} UNKNOWN IMPORTED)
set_target_properties(${PUGIXML_LIBRARY} PROPERTIES
    "IMPORTED_LOCATION" "${PUGIXML_LIBRARY_PATH}"
    "INTERFACE_INCLUDE_DIRECTORIES" "${PUGIXML_INCLUDE_DIRS}")
add_dependencies(${PUGIXML_LIBRARY} pugixml)
# -----------------------------------------------------------------------------
# End
# -----------------------------------------------------------------------------
