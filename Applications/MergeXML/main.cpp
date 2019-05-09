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
#include "SFileSystem.h"
#include "SFile.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------------------------------
 */
int main(int argc, char** argv) {   
    auto r = Reader("/mnt/c/Workspace/space-shape/Applications/MergeXML/Resources/ConnectionEditor.arxml");

    auto w = Writer("/tmp/ConnectionEditor.json");

    Convert::ToPrettyJson(w, Convert::FromXML(r));

    r = Reader("/mnt/c/Workspace/space-shape/Applications/MergeXML/Resources/ConnectionEditor.arxml");

    w = Writer("/tmp/ConnectionEditor.bin");

    Convert::ToBin(w, Convert::FromXML(r));
    return 0;   
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */