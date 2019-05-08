/**
 * ------------------------------------------------------------------------------------------------
 * File:   main.cpp
 * Author: Luis Monteiro
 *
 * Created on December 8, 2015, 11:48 PM
 * ------------------------------------------------------------------------------------------------
 **
 * std
 */
#include <iostream>
/**
 * space
 */
#include "SXmlConvert.h"
#include "SFileSystem.h"
#include "SFile.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Main
 * ------------------------------------------------------------------------------------------------
 */
int main(int argc, char** argv) {   
    auto in = String(
        "/mnt/c/Workspace/space-shape/Applications/MergeXML/Resources/ConnectionEditor.arxml"); 
    
    Reader r (in);

    auto v = XmlConvert::fromXML(r);


    JsonConvert::ToPrettyJson(std::cout, v) << std::endl;


    return 0;   
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */