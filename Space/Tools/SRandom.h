/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * File:   Random.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SRANDOM_H
#define SRANDOM_H
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
 * Random - Space
 * ------------------------------------------------------------------------------------------------
 */
namespace Random {
    /**
     * ------------------------------------------------------------------------
     * Random Element
     * ------------------------------------------------------------------------
     */
    inline Float Element(Float min, Float max){
        static std::random_device rd;
        std::default_random_engine gen (rd());
        return Float(std::uniform_real_distribution<>(min, max)(gen));        
    }
    /**
     * ------------------------------------------------------------------------
     * Random Container
     * ------------------------------------------------------------------------
     */
    inline Buffer& Container(Buffer& container){
        static std::random_device rd;
        std::default_random_engine gen (rd());
        for(auto& v : container)
            v = std::uniform_int_distribution<>(0, 255)(gen);
        return container;        
    }
    /**
     * ------------------------------------------------------------------------
     * Shuffle container
     * ------------------------------------------------------------------------
     */
    inline List Shuffle(List data){
        static std::random_device rd;
        std::default_random_engine gen (rd());
        std::shuffle(data.begin(), data.end(), gen);
        return data;          
    }
    /**
     * ------------------------------------------------------------------------
     * Select (n) keys from (data) 
     * ------------------------------------------------------------------------
     */
    inline List Select(Integer n, Map data) {
        static std::random_device rd;
        std::default_random_engine gen (rd());
        // split keys and weight
        std::list<Var> keys;
        std::list<int> weight;
        for(auto& d: data) {
            keys.emplace_back(Obj(d.first)); 
            weight.emplace_back(int(Var(d.second)));   
        }
        // process
        auto out = List();
        for (auto i = 0; i < n && keys.size(); ++i) {
            // random weighted position
            auto pos = std::discrete_distribution<>(weight.begin(), weight.end())(gen);
            // remove position from weights
            weight.erase(std::next(weight.begin(), pos));
            // get key position
            auto pos_k = std::next(keys.begin(), pos);
            // add key
            out.emplace_back(*pos_k);
            // remove position from keys
            keys.erase(pos_k);
        }
        return out;          
    }
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SRANDOM_H */

