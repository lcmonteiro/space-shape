cmake_minimum_required (VERSION 3.10)
# -----------------------------------------------------------------------------
# Test project
# -----------------------------------------------------------------------------
project (Test)
# Configure google-test as a downloadable library.
include(GoogleTest.cmake)
# add test function
function(add_gtests)
    set(options)
    set(oneValue   TARGET)
    set(multiValue INCLUDES SOURCES DEPENDS)
    cmake_parse_arguments(ARG "${options}" "${oneValue}" "${multiValue}" ${ARGN} )
    add_executable (
        ${ARG_TARGET} ${ARG_SOURCES}
    )
    target_include_directories(
        ${ARG_TARGET}
    PRIVATE
        ${ARG_INCLUDES}
    )
    target_link_libraries(
        ${ARG_TARGET}
    PRIVATE
       GTest::GTest GTest::Main ${ARG_DEPENDS}
    )
    add_test(NAME ${ARG_TARGET} COMMAND ${ARG_TARGET})
endfunction()
#
# tests subdirectories
#
add_subdirectory(Space)
# -----------------------------------------------------------------------------
# end
# -----------------------------------------------------------------------------
