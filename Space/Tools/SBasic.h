/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * File:   SBasic.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SBASIC_H
#define SBASIC_H
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
 * Basic - Space
 * ------------------------------------------------------------------------------------------------
 */
namespace Tools {
namespace Basic {
    /**
     * --------------------------------------------------------------------------------------------
     * Map tools
     * --------------------------------------------------------------------------------------------
     * Get Keys
     * -----------------------------------------------------------------------
     */
    inline List GetKeys(const Map& map) {
        List keys;
        for (auto& v : map) {
            keys.push_back(Obj(v.first));
        }
        return keys;
    }
    /**
     * -----------------------------------------------------------------------
     * Set keys with a default value 
     * -----------------------------------------------------------------------
     */
    inline Map SetKeys(const List& keys, const Var def=Obj("")) {
        Map map;
        for (Var k : keys) {
            map[k] = def;
        }
        return map;
    }
    /**
     * -----------------------------------------------------------------------
     * Combine key and values together 
     * -----------------------------------------------------------------------
     */
    inline Map Combine(const List& keys, const List& values) {
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
     * Rotate 
     * -----------------------------------------------------------------------
     */
    inline List& Rotate(List& data) {
        std::rotate(data.begin(), data.begin() + 1, data.end());
        return data;
    }
    inline List Rotate(List&& data) {
        std::rotate(data.begin(), data.begin() + 1, data.end());
        return std::move(data);
    }
    /**
     * ------------------------------------------------------------------------
     * Select 
     * ------------------------------------------------------------------------
     */
    inline List& Select(List& data, size_t beg, size_t end) {
        if(data.size() > end)
            data.erase(data.begin() + end, data.end());
        if(data.size() > beg)
            data.erase(data.begin(), data.begin() + beg); 
        return data;
    }
    inline List Select(List&& data, size_t beg, size_t end) {
        return std::move(Select(data, beg, end));
    }
    /**
     * --------------------------------------------------------------------------------------------
     * String tools
     * --------------------------------------------------------------------------------------------
     * Split 
     * ------------------------------------------------------------------------
     */
    inline List Split(String str, String delim="-") {
        auto end = std::string::npos;
        // save a sub string foreach find
        List lst;
        for(auto pos = str.find(delim); pos != end; pos = str.find(delim)) {   
            // save substring
            lst.emplace_back(Obj(str.substr(0, pos)));
            // remove segment 
            str.erase(0, pos + delim.length());
        }
        lst.push_back(Obj(str.substr()));
        return lst;
    }
    /**
     * -----------------------------------------------------------------------
     * Replace 
     * -----------------------------------------------------------------------
     */
    inline String& Replace(
        String& str, String::value_type find, String::value_type update
        ) {
        std::replace(str.begin(), str.end(), find, update);
        return str;
    }
    inline String Replace(
        String&& str, String::value_type find, String::value_type update
        ){
        std::replace(str.begin(), str.end(), find, update);
        return std::move(str);
    }
    /**
     * ------------------------------------------------------------------------
     * Sort
     * ------------------------------------------------------------------------
     */
    template<typename Function>
    inline List& Sort(List& list, Function f) {
        std::sort(list.begin(), list.end(), f);
        return list;
    }
    template<typename Function>
    inline List Sort(List&& list, Function f) {
        std::sort(list.begin(), list.end(), f);
        return std::move(list);
    }
    /**
     * ------------------------------------------------------------------------
     * Accumulate
     * ------------------------------------------------------------------------
     */
    template<typename Type, typename Function>
    inline Type Accumulate(const List& list, Type base, Function f) {
        return std::accumulate(list.begin(), list.end(), base, f);
    }
    /**
     * -----------------------------------------------------------------------
     * Count 
     * ------------------------------------------------------------------------
     */
    inline size_t Count(const String& str, String::value_type find) {
        return std::count(str.begin(), str.end(), find);
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Var tools
     * --------------------------------------------------------------------------------------------
     * Reducer
     * ------------------------------------------------------------------------
     */
    inline Var Reducer(Var v) {
        return Logic::ForEach(v, [](const List&, Var d) -> Var {
            return Logic::IfException([&d]() {
                return Obj(Var::ToInteger(d));
            }, Logic::IfException([&d]() {
                return Obj(Var::ToFloat(d));
            }, d));
        });
    }
}}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SBASIC_H */

