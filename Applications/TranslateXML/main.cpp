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
#include "SUtils.h"
#include "SLogic.h"
#include "SConvert.h"
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
int Export(List paths, String out);
/**
 * --------------------------------------------------------
 * Import translated text
 * --------------------------------------------------------
 */
int Import(List paths, String in);
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
    std::map<String, std::function<int()>> funcs{
        {"export", [&](){
            return Export(Find(args["i"], args["p"]), args['o']);
        }},
        {"import", [&](){
            return Import(Find(args["o"], args["p"]), args['i']);
        }}
    };
    return funcs[args["m"]]();   
}
/**
 * ------------------------------------------------------------------------------------------------
 * Tools - Implementation
 * ------------------------------------------------------------------------------------------------
 * Find files
 * ----------------------------------------------------------------------------
 */
List Find(String path, String pattern) {
    /**
     * read from file system
     */
    Var aux = FileSystem::Find(path, [&pattern](auto path, auto type) {
        return (FileSystem::DIR == type) 
            || std::regex_match(path.back(), std::regex(pattern));
    });
    /**
     * filter empty folders
     */ 
    aux = Var::Trim(Logic::ForEach(aux, [](auto, auto var) {
        if(Var::IsInteger(var)) {
            return (Var::Integer(var) == FileSystem::DIR)? Obj(nullptr) : var;  
        }
        return var;
    }));
    /**
     * return a list of path files
     */
    return Logic::ForEach(
        Utils::GetKeys(Convert::ToSimpleMap(aux, "/")), [&path](auto, auto var){
            return Obj(String::Build(path, "/", Var::String(var)));
        }
    );
}
/**
 * ----------------------------------------------------------------------------
 * Export text to translate
 * ----------------------------------------------------------------------------
 */
int Export(List paths, String out) {

    return 0;
}
/**
 * ----------------------------------------------------------------------------
 * Import translated text
 * ----------------------------------------------------------------------------
 */
int Import(List paths, String in) {

    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */