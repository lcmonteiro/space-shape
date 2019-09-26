/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * File:   SVariant.h
 * Author: Luis Monteiro
 *
 * Created on Sep 1, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SVARIANT_H
#define SVARIANT_H
/**
 * std
 */
#include <algorithm>
#include <set>
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
namespace Variant {
    /**
     * ------------------------------------------------------------------------
     * Sort
     * ------------------------------------------------------------------------
     */
    template<typename Compare>
    inline List Sort(List&& list, Compare cmp) {
        using Cache = std::multiset<Var, Compare>;
        Cache tmp(list.begin(), list.end(), cmp);
        return List(tmp.begin(), tmp.end());
    }
    template<typename Compare>
    inline List& Sort(List& list, Compare cmp) {
        list = Sort(std::move(list), cmp);
        return list;
    }
    /**
     * ------------------------------------------------------------------------
     * Foreach 
     * ------------------------------------------------------------------------
     */
    template<typename Sequency, typename Function1, typename FunctionN>
    inline FunctionN Foreach(const Sequency& seq, Function1 first, FunctionN next) {
        auto it  = seq.begin();
        auto end = seq.end();
        if(it != end) 
            first(*it);
        for (++it ; it != end; ++it)
		    next(*it);
	    return next;
    }
}}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SVARIANT_H */

