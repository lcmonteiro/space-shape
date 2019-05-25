cmake_minimum_required (VERSION 3.10)
# ---------------------------------------------------------
# add Tools test 
# ---------------------------------------------------------
add_gtests(
TARGET 
    TestSpaceKernel
SOURCES 
    Kernel/TestVariable.cpp
DEPENDS
    Space
)
# ---------------------------------------------------------
# add Tools test 
# ---------------------------------------------------------
add_gtests(
TARGET 
    TestSpaceTool
SOURCES 
    Tools/TestEdit.cpp
DEPENDS
    Space
)
# ---------------------------------------------------------
# end
# ---------------------------------------------------------