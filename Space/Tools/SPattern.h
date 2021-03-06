/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * File:   SPattern.h
 * Author: Luis Monteiro
 *
 * Created on Jun 27, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SPATTERN_H
#define SPATTERN_H
/**
 * std
 */
#include <vector>
/**
 * space
 */
#include "STypes.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Pattern - Tool
 * ------------------------------------------------------------------------------------------------
 */
namespace Tools {
namespace Pattern {
    /**
     * --------------------------------------------------------------------------------------------
     * Glob pattern
     * -----------------------------------------------------------------------
     * Match
     * -----------------------------------------------------------------------
     */
    bool Match(const String& value, const String& pattern);
    /**
     * -----------------------------------------------------------------------
     * Merge
     *      pattern 1 U pattern 2 -> 
     * -----------------------------------------------------------------------
     */
    String Merge(const String& p1, const String& p2);
    /**
     * -----------------------------------------------------------------------
     * Normalize 
     *      e.g: "a**b?cd**" -> "a*b?cd*"
     * -----------------------------------------------------------------------
     */
    String Normalize(String p);
    /**
     * -----------------------------------------------------------------------
     * Compare
     *   -1 = pattern(p1) < pattern(p2)
     *    0 = pattern(p1) = pattern(p2)
     *    1 = pattern(p1) > pattern(p2)
     * -----------------------------------------------------------------------
     */
    int Compare(const String& p1, const String& p2);
    /**
     * -----------------------------------------------------------------------
     * Minimum 
     * -----------------------------------------------------------------------
     */
    inline const String& Minimum(const String& p1, const String& p2) {
        return Compare(p1, p2) < 0 ? p1 : p2;
    }
    /**
     * extended
     */
    template <typename... Args>
    inline const String Minimum(const String& p1, const String& p2, Args... pn) {
        return Minimum(Minimum(p1, p2), pn...);
    }
}}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SPATTERN_H */

