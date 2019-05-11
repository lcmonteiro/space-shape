/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   SSearch.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SSEARCH_H
#define SSEARCH_H
/**
 * std
 */
#include <regex>
/**
 * space
 */
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Search free functions
 * ------------------------------------------------------------------------------------------------
 */
namespace Search {
    /**
     * --------------------------------------------------------------------------------------------
     * Match Tree
     * --------------------------------------------------------------------------------------------
     * Execute all branches that match the tree expression
     * ------------------------------------------------------------------------
     */
    template<typename F>
    Var Execute(Key expr, F func, Var on);
    /**
     * ------------------------------------------------------------------------
     * Update all branches that match the tree expression
     * ------------------------------------------------------------------------
     */
    Var Update(Key expr, Var var, Var on);
    /**
     * utils
     */
    inline Var Update(Map& var, Var on) {
        for (auto& v : var) {
            on = Update(v.first, v.second, on);
        }
        return on;
    }
    inline Var Update(Map&& var, Var on) {
        return Update(var, on);
    }
    /**
     * ------------------------------------------------------------------------
     * Find all branches that match the tree expression
     * ------------------------------------------------------------------------
     */
    Var Find(Key expr, Var on);
    /**
     * ------------------------------------------------------------------------
     * Delete all branches that match the tree expression
     * ------------------------------------------------------------------------
     */
    Var Delete(Key expr, Var on);
    /**
     * --------------------------------------------------------------------------------------------
     * Match Value
     * --------------------------------------------------------------------------------------------
     * Find vars that match expression
     * ------------------------------------------------------------------------
     */
    List Find(Var expr, const List& on);
    Map  Find(Var expr, const Map&  on);
    Var  Find(Var expr,       Var&  on);
    /**
     * ------------------------------------------------------------------------
     * Remove vars from a start location (on)
     * ------------------------------------------------------------------------
     */
    List Remove(Var expr, List& on);
    Map  Remove(Var expr, Map&  on);
    Var  Remove(Var expr, Var&  on);
    /**
     * ------------------------------------------------------------------------
     * Delete match from a start location (on)
     * ------------------------------------------------------------------------
     */
    Var Delete(Var expr, Var on);
    /**
     * ------------------------------------------------------------------------
     * Verify if expression match on var 
     * ------------------------------------------------------------------------
     */
    Boolean Match(Var expr, const List& on);
    Boolean Match(Var expr, const Map&  on);
    Boolean Match(Var expr,       Var&  on);
    /**
     * ------------------------------------------------------------------------
     * Execute function f match 
     * ------------------------------------------------------------------------
     */
    template<typename Function>
    Var Execute(Key expr, Function func, Var on);
    /**
     * --------------------------------------------------------------------------------------------
     * Implementation
     * --------------------------------------------------------------------------------------------
     */
    namespace {
        /**
         * --------------------------------------------------------------------
         * Execute Filter
         * --------------------------------------------------------------------
         */
        template<typename F>
        Var __ExecuteFilter(
            std::sregex_iterator  it, 
            std::sregex_iterator& end, Var var, F func, Var on) {
            //  end case ------------------------------------------------------
            if (it == end) { return func(var); }
            
            // map case -------------------------------------------------------
            if (Var::IsMap(on)) {
                Map& m = Var::Map(on);

                // try to find ------------------------------------------------ 
                auto find = m.find(it->str());
                if (find != m.end()) {
                    find->second = __ExecuteFilter(
                        ++it, end, find->second, func, find->second);
                    return on;
                }
                // try to find by regex ---------------------------------------
                std::regex expr(it->str());
                ++it;
                for (auto mit = m.begin(); mit != m.end(); ++mit) {
                    if (std::regex_match(mit->first, expr)) {
                        mit->second = __ExecuteFilter(
                            it, end, mit->second, func, mit->second);
                    }
                }
                return on;
            }
            // list case ------------------------------------------------------
            if (Var::IsList(on)) {
                List& l = Var::List(on);
                try {
                    // try to find --------------------------------------------
                    auto find = l.begin() + Integer::ValueOf(it->str());
                    if (find != l.end()) {
                        *find = __ExecuteFilter(
                            ++it, end, *find, func, *find);
                        return on;
                    }
                } catch (std::invalid_argument& ex) {
                    // try to find by regex -----------------------------------
                    std::regex expr(it->str());
                    ++it;
                    for (auto lit = l.begin(); lit != l.end(); ++lit) {
                        if (std::regex_match(
                            String::ValueOf(Integer(std::distance(l.begin(), lit))),
                            expr)) {
                            *lit = __ExecuteFilter(it, end, *lit, func, *lit);
                        }
                    }
                }
                return on;
            }
            return on;
        }
    }
    /**
     * ------------------------------------------------------------------------
     * Execute 
     * ------------------------------------------------------------------------
     */
    template<typename Function>
    Var Execute(Key expr, Function func, Var on) {
        const std::regex e("([^/]+)");
        
        // iterator ---------------------------------------
        std::sregex_iterator it(expr.begin(), expr.end(), e);
        std::sregex_iterator end;

        // execute ----------------------------------------
        return __ExecuteFilter(it, end, on, func, on);
    }
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SSEARCH_H */

