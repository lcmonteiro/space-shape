/**
 * ------------------------------------------------------------------------------------------------
 * File:   main.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 **
 * std
 */
#include <iostream>
/**
 * space
 */
#include "SConvertXML.h"
#include "SConvertARG.h"
#include "SFileSystem.h"
#include "SFile.h"
#include "SLog.h"
#include "SNullStream.h"
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
        {argv + 1, argv + argc}, 
        {
            {"i", "in"}, 
            {"o", "out"}
        }
    );
    /**
     * log configuration
     */
    DEBUG("arg", args);

    // auto r = Reader("/mnt/c/Workspace/space-shape/Applications/MergeXML/Resources/ConnectionEditor.arxml");

    // auto w = Writer("/tmp/ConnectionEditor.json");

    // Convert::ToPrettyJson(w, Convert::FromXML(r));

    // r = Reader("/mnt/c/Workspace/space-shape/Applications/MergeXML/Resources/ConnectionEditor.arxml");

    // w = Writer("/tmp/ConnectionEditor.bin");

    // Convert::ToBin(w, Convert::FromXML(r));
    return 0;   
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */