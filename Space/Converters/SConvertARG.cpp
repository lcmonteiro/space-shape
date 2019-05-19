/**
 * ------------------------------------------------------------------------------------------------
 * File:   XmlConverter.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 **
 * std
 */
#include <list>
#include <regex>
/** 
 * space
 */
#include "SConvertARG.h"
/**
 * ------------------------------------------------------------------------------------------------
 *  From XML
 * ------------------------------------------------------------------------------------------------
 *  Public
 * ----------------------------------------------------------------------------
 * definitions
 */
const auto KEY_PATTERNS = std::list<std::regex>{
    std::regex("-([a-zA-Z])"),
    std::regex("--([a-zA-Z])+"),
};
/**
 * implementation
 */
Var Convert::FromARG(std::vector<std::string> args) {
    /**
     * init values
     */
    auto out = Map(); 
    auto key = Key();  
    /**
     * iterate args
     */
    for (auto& a : args) {
        try {
            /**
             * try find key
             */
            for(auto& p : KEY_PATTERNS) {
                auto m = std::smatch();
                if (std::regex_match (a, m, p)) {
                    throw Key(m.str(1));
                }
            }
            /**
             * not a key 
             */
            switch(out.count(key)) {
                // new one
                case 0:  out[key]= Obj(a);                        break;
                // list transform                        
                case 1:  out[key]= Obj::List({out[key], Obj(a)}); break;
                // append
                default: Var::List(out[key]) += Obj(a);           break;
            }
            /**
             * remove key
             */
            key = Key();
        } catch(Key k) {
            /**
             * found a key
             */
            if(!key.empty()) {
                out[key]= Obj(true);
            }
            /**
             * update key
             */
            key = k;
        }
    }
    if(!key.empty()) { 
        out[key]= Obj(true);
    }
    /**
     * return 
     */
    return Obj(std::move(out));
}

/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */

