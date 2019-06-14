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
#include <functional>
/**
 * space
 */
#include "SKeys.h"
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
    Var Execute(Key expr, std::function<Var(const Key&, Var)> func, Var on);
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
    Var  Find(Var expr,       Var   on);
    /**
     * ------------------------------------------------------------------------
     * Remove vars from a start location (on)
     * ------------------------------------------------------------------------
     */
    List Remove(Var expr, List& on);
    Map  Remove(Var expr, Map&  on);
    Var  Remove(Var expr, Var   on);
    /**
     * ------------------------------------------------------------------------
     * Delete match from a start location (on)
     * ------------------------------------------------------------------------
     */
    List& Delete(Var expr, List& on);
    Map&  Delete(Var expr, Map&  on);
    Var   Delete(Var expr, Var   on);
    /**
     * ------------------------------------------------------------------------
     * Verify if expression match on var 
     * ------------------------------------------------------------------------
     */
    Boolean Match(Var expr, const List& on);
    Boolean Match(Var expr, const Map&  on);
    Boolean Match(Var expr,       Var   on);
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SSEARCH_H */

