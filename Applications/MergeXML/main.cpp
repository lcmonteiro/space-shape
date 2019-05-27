/**
 * ------------------------------------------------------------------------------------------------
 * File:   main.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 **
 * space
 */
#include "SLog.h"
#include "SConvertARG.h"
/**
 * commands
 */
#include "Normalize.h"
#include "Learn.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------------------------------
 */
int main(int argc, char** argv) {   
    /**
     * parse arguments
     */
    auto args = Convert::FromARG(
        {argv + 1, argv + argc}, {
            {"in"     , "i"}, 
            {"out"    , "o"},
            {"pattern", "p"}
        }
    );
    INFO("Arguments", args);
    /**
     * Process
     */
    INFO("Filesystem", Command::Explorer(args["in"])); 
    /**
     * finish
     */
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */