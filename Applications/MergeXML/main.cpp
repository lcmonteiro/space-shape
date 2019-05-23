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
#include "SEdit.h"
#include "SConvertXML.h"
#include "SConvertARG.h"
#include "SConvertBIN.h"
#include "SFileSystem.h"
#include "SNullStream.h"
#include "SFileStream.h"
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
    INFO("Arguments", args);
    /**
     * Process
     */
    auto r = File::Reader("/mnt/c/Workspace/space-shape/Applications/MergeXML/Resources/ConnectionEditor.arxml");

    auto w = File::Writer("/tmp/ConnectionEditor.json");

    Convert::ToPrettyJson(w, Convert::FromXML(r));

    r = File::Reader("/mnt/c/Workspace/space-shape/Applications/MergeXML/Resources/ConnectionEditor.arxml");

    w = File::Writer("/tmp/ConnectionEditor.bin");

    Convert::ToBin(w, Convert::FromXML(r));
    return 0;   
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */