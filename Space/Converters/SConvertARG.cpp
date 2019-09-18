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
#include "SLog.h"
/**
 * namespace
 */
using namespace std;
/**
 * ------------------------------------------------------------------------------------------------
 *  From ARGS
 * ------------------------------------------------------------------------------------------------
 *  definitions
 * ----------------------------------------------------------------------------
 */
const auto KEY_PATTERNS = list<regex>{
    regex("-([a-zA-Z])"),
    regex("--([a-zA-Z]+)"),
};
/**
 * ----------------------------------------------------------------------------
 * implementation
 * ----------------------------------------------------------------------------
 */
Var Convert::FromARG(vector<string> args, map<string,string> map) {
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
                auto m = smatch();
                if (regex_match (a, m, p)) {
                    throw Key(m.str(1));
                }
            }
            /**
             * key not found
             * updade as a value 
             */
            switch(out.count(key)) {
                case 0: {
                    out[key]= Obj(a);                        
                    break;
                } 
                case 1: {
                    if(Var::IsList(out[key])) {
                        Var::List(out[key]) += Obj(a);  
                    } else {
                        out[key] = Obj{out[key], Obj(a)};
                    }
                    break;
                }
            }
            /**
             * key consumed
             * clear key
             */
            key = Key();
        } catch(Key k) {
            /**
             * key found
             * update update as a flag if previous key still exist 
             */
            if(!key.empty()) {
                out[key]= Obj(true);
            }
            /**
             * update key
             * try to find key translation on the map 
             */
            auto found = map.find(k);
            if(found != map.end()) {
                key = found->second;
            } else {
                key = k;    
            }
        }
    }
    if(!key.empty()) { 
        out[key]= Obj(true);
    }
    /**
     * return 
     */
    return Obj(move(out));
}   
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */

