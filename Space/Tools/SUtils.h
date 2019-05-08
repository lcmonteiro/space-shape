/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * File:   Sutils.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SUTILS_H
#define SUTILS_H
/**
 * std
 */
#include <algorithm>
#include <iostream>
#include <random>
#include <list>
/**
 * space
 */
#include "SVariable.h"
#include "SLogic.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Utils - Space
 * ------------------------------------------------------------------------------------------------
 */
namespace Utils {
    /**
     * ----------------------------------------------------------------------------------------
     * Random tools
     * ----------------------------------------------------------------------------------------
     */
    static inline Float Random(Float min, Float max){
        static random_device rd;
        default_random_engine gen (rd());
        return Float(uniform_real_distribution<>(min, max)(gen));        
    }
    static inline Buffer& Random(Buffer& container){
        static random_device rd;
        default_random_engine gen (rd());
        for(auto& v : container){
            v = uniform_int_distribution<>(0, 255)(gen);
        }
        return container;        
    }
    static inline List Shuffle(List data){
        static random_device rd;
        default_random_engine gen (rd());
        shuffle(data.begin(), data.end(), gen);
        return data;          
    }
    static inline List Select(Integer n, Map data){
        static random_device rd;
        default_random_engine gen (rd());
        // split keys and weight
        list<Var> keys;
        list<int> weight;
        for(auto& d: data){
            keys.push_back(Obj(d.first)); weight.push_back(int(Var(d.second)));   
        }
        // process
        List out;
        for (Integer i = 0; i < n && !keys.empty(); ++i) {
            // random weighted position
            auto pos = discrete_distribution<>(weight.begin(), weight.end())(gen);
            // remove position from weights
            weight.erase(next(weight.begin(), pos));
            // get key position
            auto pos_k = next(keys.begin(), pos);
            // add key
            out.push_back(*pos_k);
            // remove position from keys
            keys.erase(pos_k);
        }
        return out;          
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Map tools
     * --------------------------------------------------------------------------------------------
     */
    static inline List GetKeys(const Map& map) {
        List keys;
        for (auto& v : map) {
            keys.push_back(Obj(v.first));
        }
        return keys;
    }
    static inline Map SetKeys(const List& keys, const Var def=Obj("")) {
        Map map;
        for (Var k : keys) {
            map[k] = def;
        }
        return map;
    }
    static inline Map Combine(List keys, List values) {
        Map map;
        for (size_t i = 0; i < keys.size() && i < values.size(); ++i) {
            map[Var::ToString(keys[i])] = values[i];
        }
        return map;
    }
    /**
     * --------------------------------------------------------------------------------------------
     * List tools
     * --------------------------------------------------------------------------------------------
     */
    static inline Var ToList(Var data) {
        return Var::IsList(data) ? data : Obj::List({data});
    }
    static inline List& Rotate(List& data) {
        std::rotate(data.begin(), data.begin() + 1, data.end());
        return data;
    }
    /**
     * --------------------------------------------------------------------------------------------
     * String tools
     * --------------------------------------------------------------------------------------------
     */
    static inline List Split(String str, String delim="-") {
        List lst;
        String segment;
        size_t pos = 0;
        // Iterate last segment 
        str += delim;
        // Get first delimiter position   
        while ((pos = str.find(delim)) != std::string::npos) {
            // save substring
            segment = str.substr(0, pos);
            lst.push_back(Obj(segment));
            // remove segment 
            str.erase(0, pos + delim.length());
        }
        return lst;
    }
    static inline String Replace(String str, String::value_type find, String::value_type update) {
        replace(str.begin(), str.end(), find, update);
        return str;
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Var tools
     * --------------------------------------------------------------------------------------------
     */
    static Var Reducer(Var v) {
        return Logic::ForEach(v, [](Var d) {
            return Logic::IfException([&d]() {
                return Obj(Var::ToInteger(d));
            }, Logic::IfException([&d]() {
                return Obj(Var::ToFloat(d));
            }, d));
        });
    }
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SUTILS_H */

