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
#include "SBasic.h"
#include "SLogic.h"
#include "SConvertARG.h"
#include "SFileSystem.h"
/**
 * local
 */
#include "SProfiles.h"
#include "SFind.h"
#include "SImport.h"
#include "SExport.h"
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
            {"profile", "p"}
        }
    );
    INFO("Arguments", args);
    /**
     * Process
     */
    std::map<String, std::function<int()>> funcs{
        {"export", [&](){
            return Export(args["i"], GetProfile(args["p"]), args["o"]);
        }},
        {"import", [&](){
            return Import(args["i"], GetProfile(args["p"]), args["o"]);
        }}
    };
    try {
        return funcs[args["m"]]();
    } catch(std::exception& e) {
        ERROR(e.what(), args);
        return -1;
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
