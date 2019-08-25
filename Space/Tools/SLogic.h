/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   SLogic.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SLOGIC_H
#define SLOGIC_H
/**
 * std
 */
#include <functional>
/**
 * space
 */
#include "SEdit.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Logic - Space
 * ------------------------------------------------------------------------------------------------
 */
namespace Logic {
    /**
     * ------------------------------------------------------------------------
     * Types
     * ------------------------------------------------------------------------
     */
    using UpdateFunction = std::function<Var(const List&, Var)>; 
    using MergeFunction  = std::function<Var(const List&, Var, Var)>; 
    /**
     * --------------------------------------------------------------------------------------------
     * Exceptions 
     * --------------------------------------------------------------------------------------------
     */
    typedef std::runtime_error SLogicException;
    /**
     * --------------------------------------------------------------------------------------------
     * Conditions 
     * --------------------------------------------------------------------------------------------
     */
    static inline Var IfnDefined(Var var, Var def) {
        return Var::IsDefined(var) ? var : def;
    }
    template<typename Function>
    static inline Var IfnDefined(Var var, Function func) {
        return Var::IsDefined(var) ? var : func();
    }
    static inline Var IfnEmpty(Var var, Var def) {
        return Var::IsEmpty(var) ? def : var;
    }
    template<typename Function>
    static inline Var IfDefined(Var var, Function func) {
        return Var::IsDefined(var) ? func(var) : var;
    }
    template<typename Function>
    static inline Var IfException(Function func, Obj except) {
        try { return func(); } catch(...) { return except; }
    }
    template<typename Function>
    static inline Var IfException(Function func, Var except) {
        try { return func(); } catch(...) { return except; }
    }
    template<typename Function1, typename Function2>
    static inline Var IfException(Function1 func, Function2 except) {
        try { return func(); } catch(...) { return except(); }
    }
    template<typename Function1, typename Function2>
    static inline Var IfList(Var v, Function1 succ, Function2 fail) {
        return Var::IsList(v) ? succ(v) : fail(v);
    }
    template<typename Function1, typename Function2>
    static inline Var IfMap(Var v, Function1 succ, Function2 fail) {
        return Var::IsMap(v) ? succ(v) : fail(v);
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Call function [update(const List& path, Var )] for all elements
     * --------------------------------------------------------------------------------------------
     * Var
     * ------------------------------------------------------------------------
     */
    Var ForEach(Var var, UpdateFunction update);
    /**
     * ------------------------------------------------------------------------
     * List
     * ------------------------------------------------------------------------
     */ 
    inline List& ForEach(List& list, std::function<Var(size_t, Var)> update) {
        size_t i = 0;
        for(auto& v: list) {
            v = update(i, v); ++i;
        }
        return list;
    }
    inline List ForEach(List&& list, std::function<Var(size_t, Var)> update) {
        size_t i = 0;
        for(auto& v: list) {
            v = update(i, v); ++i;
        }
        return std::move(list);
    }
    /**
     * ------------------------------------------------------------------------
     * Map
     * ------------------------------------------------------------------------
     */ 
    inline Map& ForEach(Map& map, std::function<Var(const Key&, Var)> update) {
        for(auto& v: map) {
            v.second = update(v.first, v.second);
        }
        return map;
    }
    inline Map ForEach(Map&& map, std::function<Var(const Key&, Var)> update) {
        for(auto& v: map) {
            v.second = update(v.first, v.second);
        }
        return std::move(map);
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Merge A (var) into B (on) with F (merge)
     *   - Call function [merge(const List& path, Var var, Var on)] for all elements
     * --------------------------------------------------------------------------------------------
     */
    Var MergeEach(const Var var, Var on, MergeFunction merge);
    /**
     * --------------------------------------------------------------------------------------------
     * Logic Converters
     * --------------------------------------------------------------------------------------------
     * To List
     * ------------------------------------------------------------------------
     */
    inline List ToList(Map data) {
        List lst; 
        for (auto& v : data) 
            lst.emplace_back(v.second); 
        return std::move(lst);
    } 
    inline List ToList(Var data) {
        return Var::IsList(data) ? 
            Var::List(data) : Var::IsDefined(data) ? List{data}: List{};
    }
    /**
     * ------------------------------------------------------------------------
     * To Map
     * ------------------------------------------------------------------------
     */ 
    inline Map ToMap(Var data) {
        return Var::IsMap(data) ? 
            Var::Map(data) : Var::IsDefined(data) ? Map{{Key(), data}}: Map{};
    }
    /**
     * ------------------------------------------------------------------------
     * To String
     * ------------------------------------------------------------------------
     */ 
    inline String ToString(Var data) {
        return Var::IsString(data) ? 
            Var::String(data) : Var::IsDefined(data) ? String(data): String();
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Logic Update
     * --------------------------------------------------------------------------------------------
     */
    template<typename Function>
    inline Var Update(Key path, Function func, Var data) {
        return Edit::Insert(path, func(Edit::Remove(path, data)), data);
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Logic Find
     * --------------------------------------------------------------------------------------------
     */
    static inline Var Find(Key path, Var on, Var def) {
        return IfnDefined(Edit::Find(path, on), [&]() {
            Edit::Insert(path, def, on);
            return def;
        });
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Logic Get
     * --------------------------------------------------------------------------------------------
     */
    static inline Var Get(Key key, Map& on, Var def) {
        return IfnDefined(on[key], [&]() {
            on[key] = def;
            return def;
        });
    }
    /**
     * --------------------------------------------------------------------------------------------
     * assert
     * --------------------------------------------------------------------------------------------
     */
    template<class T>  
    static inline T Assert(T obj, Boolean(T::*method)()) {
        if(!((obj.*method)())) {
            throw SLogicException("assert");
        }
        return obj;
    }
    static inline void Assert(bool test) {
        if(!test) {
            throw SLogicException("assert");
        }
    }
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SLOGIC_H */

