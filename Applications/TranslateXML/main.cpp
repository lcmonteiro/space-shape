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
#include <map>
#include <functional>
/**
 * space
 */
#include "SLog.h"
#include "SEdit.h"
#include "SConvertXML.h"
#include "SConvertARG.h"
#include "SFileSystem.h"
#include "SFileStream.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Tools - Declaration
 * ------------------------------------------------------------------------------------------------
 * Find files
 * --------------------------------------------------------
 */
List Find(String path, String pattern);
/**
 * --------------------------------------------------------
 * Export text to translate
 * --------------------------------------------------------
 */
void Export(List paths, String out);
/**
 * --------------------------------------------------------
 * Import translated text
 * --------------------------------------------------------
 */
void Import(List paths, String in);
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
            {"method" , "m"},
            {"pattern", "p"}
        }
    );
    INFO("Arguments", args);
    /**
     * Process
     */
    std::map<String, std::function<void()>> funcs{
        {"export", [&](){
            Export(Find(args["i"], args["p"]), args['o']);
        }},
        {"import", [&](){
            Import(Find(args["o"], args["p"]), args['i']);
        }}
    };
    return 0;   
}
/**
 * ------------------------------------------------------------------------------------------------
 * Tools - Implementation
 * ------------------------------------------------------------------------------------------------
 * Find files
 * ----------------------------------------------------------------------------
 */
List Find(String path, String pattern) {
    
    return {};
}
/**
 * ----------------------------------------------------------------------------
 * Export text to translate
 * ----------------------------------------------------------------------------
 */
void Export(List paths, String out) {

}
/**
 * ----------------------------------------------------------------------------
 * Import translated text
 * ----------------------------------------------------------------------------
 */
void Import(List paths, String in) {

}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */