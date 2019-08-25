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
            {"filter" , "f"},
            {"method" , "m"},
            {"profile" ,"p"}
        }
    );
    INFO("Arguments", args);
    /**
     * Process
     */
    std::map<String, std::function<int()>> funcs{
        {"learn", [&](){
            return Learn(args["i"], args["f"], args["p"]);
        }},
        {"normalize", [&](){
            return Normalize(args["i"], args["f"], GetProfile(args["p"]));
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