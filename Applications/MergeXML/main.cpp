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
#include "SConvertARG.h"
/**
 * local
 */
#include "SProfiles.h"
#include "SNormalize.h"
#include "SMinimize.h"
#include "SLearn.h"
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
            {"filter" , "f"},
            {"method" , "m"},
            {"profile", "p"},
            {"log"    , "l"}
        }
    );
    /**
     * Set Debug Level
     */
    SLog::SetLevel(std::map<String, SLog::Level>{
        {"DEBUG"  , SLog::DEBUG},
        {"INFO"   , SLog::INFO},
        {"WARNING", SLog::WARNING},
        {"ERROR"  , SLog::ERROR},
        {""       , SLog::ERROR}
    }[args["l"]]);
    /**
     * Process
     */
    INFO("Arguments", args);
    std::map<String, std::function<int()>> funcs{
        {"learn", [&](){
            return Learn(args["i"], args["f"], args["p"]);
        }},
        {"normalize", [&](){
            return Normalize(args["i"], GetProfiles(args["p"]), args["f"]);
        }},
        {"normalize-recursive", [&](){
            return Normalize(args["i"], args["f"], GetProfiles(args["p"]));
        }},
        {"minimize", [&](){
            return Minimize(args["i"], args["f"], GetProfiles(args["p"]));
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