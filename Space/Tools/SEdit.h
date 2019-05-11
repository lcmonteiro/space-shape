/**
 * ------------------------------------------------------------------------------------------------
 * File:   SEdit.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SEDIT_H
#define SEDIT_H
/**
 * space
 */
#include "SVariable.h"
/**
 */
namespace Edit {
    /**
     * ------------------------------------------------------------------------
     * Utilities
     * ------------------------------------------------------------------------
     */
    inline MapKey ToMapKey(Var map) {
        MapKey ret;
        for (auto& v : Var::ToMap(map))
            ret[v.first] = Var::ToString(v.second);
        return ret;
    }
    /**
     * ------------------------------------------------------------------------
     * verify match from var (A) on (B)
     * -------------------------------------------------------------------------
     */
    Boolean Match(const Var&  var, const Var&  on);
    Boolean Match(const Map&  var, const Map&  on);
    Boolean Match(const List& var, const List& on);
    /**
     * ------------------------------------------------------------------------
     *                                   ___ _____ ___       _________ ___
     * Insert var (A) on other var(B)   |_A_|_A&B_|_B_| ->  |____A____|_B_| 
     * ------------------------------------------------------------------------
     */
    Var   Insert(const Var  var,  Var  on);
    Map&  Insert(const Map& var,  Map& on);
    List& Insert(const List& var, List& on);
    /**
     * Insert utilities 
     */
    inline Var Insert(const List& var, Var on = nullptr) {
        for (Var v : var) 
            on = Insert(v, on);
        return on;
    }
    inline Var Insert(const List&& var, Var on = nullptr) {
        return Insert(var, on);
    }
    /**
     * -------------------------------------------------------------------- 
     *                                   ___ _____ ___       _ _____ ___ 
     * Update var (A) on other var (B)  |_A_|_A&B_|_B_| ->  ~_|__A__|_B_| 
     * --------------------------------------------------------------------
     */
    Var   Update(const Var   var,  Var  on);
    Map&  Update(const Map&  var, Map&  on);
    List& Update(const List& var, List& on);
    /**
     * Update utilities 
     */
    Var Update(const List& var, Var on = nullptr) {
        for (Var v : var) {
            on = Update(v, on);
        }
        return on;
    }
    Var Update(const List&& var, Var on = nullptr) {
        return Update(var, on);
    }
    /**
     * --------------------------------------------------------------------
     *                                   ___ _____ ___       _ _____ _
     *  Find var (A) on other var (B)   |_A_|_A&B_|_B_| ->  ~_|_A&B_|_~
     * --------------------------------------------------------------------
     */
    Var   Find(Var  var,  const Var   on);
    Map&  Find(Map& var,  const Map&  on);
    List& Find(List& var, const List& on);
    /**
     * --------------------------------------------------------------------
     *                                   ___ _____ ___       _ ___
     *  Remove var (A) on other var (B) |_A_|_A&B_|_B_| ->  ~_|_B_|
     * --------------------------------------------------------------------
     */
    Var   Remove(const Var var,   Var   on);
    Map&  Remove(const Map& var,  Map&  on);
    List& Remove(const List& var, List& on);
    /**
     * Remove utilities 
     */
    Var Remove(const List& var, Var on = nullptr) {
        for (const Var v : var)
            on = Remove(v, on);
        return on;
    }
    Var Remove(const List&& var, Var on = nullptr) {
        return Remove(var, on);
    }
    /**
     * -------------------------------------------------------------------
     *                                                            ___ _____ ___       _ ___
     *  Delete var (A) on other var (B) and clear empty branches |_A_|_A&B_|_B_| ->  ~_|_B_|
     * --------------------------------------------------------------------
     */
    Var   Delete(const Var  var,  Var   on);
    Map&  Delete(const Map& var,  Map&  on);
    List& Delete(const List& var, List& on);
    /**
     * Delete utilities
     */
    inline Var Delete(List& var, Var on = nullptr) {
        for (Var v : var) 
            on = Delete(v, on);
        return on;
    }
    inline Var Delete(List&& var, Var on = nullptr) {
        return Delete(var, on);
    }
    /**
     * --------------------------------------------------------------------
     * Count var (A) on other var (B)
     * --------------------------------------------------------------------
     */
    Integer Count(const Var& var, const Var& on);
    /**
     * --------------------------------------------------------------------
     * Insert Key:var on other var 
     * --------------------------------------------------------------------
     */
    Var Insert(Key path, Var var, Var on = nullptr);
    /**
     * derive
     */
    inline Var Insert(Map& var, Var on = nullptr) {
        for (auto& v : var) 
            on = Insert(v.first, v.second, on);
        return on;
    }
    inline Var Insert(Map&& var, Var on = nullptr) {
        return Insert(var, on);
    }
    inline List Insert(Map& var, List on) {
        for (auto& e : on) 
            Insert(var, e);
        return std::move(on);
    }
    inline List Insert(Map&& var, List on) {
        return move(Insert(var, on));
    }
    /**
     * --------------------------------------------------------------------
     * Update Key:var on other var 
     * --------------------------------------------------------------------
     */
    Var Update(Key path, Var var, Var on = nullptr);
    /**
     * derived
     */
    inline Var Update(Map& var, Var on = nullptr) {
        for (auto& v : var)
            on = Update(v.first, v.second, on);
        return on;
    }
    inline Var Update(Map&& var, Var on = nullptr) {
        return Update(var, on);
    }
    inline List Update(Key path, Var var, List on) {
        for (Var v : on) 
            Update(path, var, v);
        return move(on);
    }
    inline List Update(Map& var, List on) {
        for (auto& e : on)
            Update(var, e);
        return move(on);
    }
    inline List Update(Map&& var, List on) {
        return std::move(Update(var, on));
    }
    /**
     * ------------------------------------------------------------------------
     * Update all branches deeper than (deep) with (var)
     * ------------------------------------------------------------------------
     */
    Var Update(Integer deep, Var var, Var on);
    /**
     * ------------------------------------------------------------------------
     * Find Key on var 
     * ------------------------------------------------------------------------
     */
    Var Find(Key path, Var on);
    /**
     * derived
     */
    static inline List Find(ListKey paths, Var on) {
        List l;
        for (auto& p : paths) 
            l.push_back(Find(p, on));
        return std::move(l);
    }
    static inline List Find(Key path, List on) {
        List l;
        for (Var o : on) 
            l.push_back(Find(path, o));
        return std::move(l);
    }
    /**
     * ------------------------------------------------------------------------
     * Remove Key on var 
     * ------------------------------------------------------------------------
     */
    Var Remove(Key path, Var on);
    /**
     * derived
     */
    inline List Remove(ListKey paths, Var on) {
        List l;
        for (auto& p : paths) 
            l.push_back(Remove(p, on));
        return move(l);
    }
    inline List Remove(Key path, List on) {
        List l;
        for (auto& o : on) 
            l.push_back(Remove(path, o));
        return std::move(l);
    }
    /**
     * ------------------------------------------------------------------------
     * Delete Key on var 
     * ------------------------------------------------------------------------
     */
    inline Var Delete(Key path, Var on){
        Remove(path, on);
        return on;
    }
    /**
     * derived
     */
    inline List Delete(ListKey paths, Var on) {
        for (auto& p : paths) 
            Remove(p, on);
        return std::move(on);
    }
    inline List Delete(Key path, List on) {
        for (auto& o : on)
            Remove(path, o);
        return std::move(on);
    }
    /**
     * ------------------------------------------------------------------------
     * Rename Key on var 
     * ------------------------------------------------------------------------
     */
    inline Var Rename(Key from, Key to, Var on) {
        return Var::Trim(Insert(to, Remove(from, on), on));
    }
    /**
     * ------------------------------------------------------------------------
     * Move
     * ------------------------------------------------------------------------
     */
    inline Var Move(Var from, MapKey& map, Var to = Obj::Map()) {
        for (auto& v : map) 
            to = Insert(v.second, Remove(v.first, from), to);
        return to;        
    }
    /**
     * derived
     */
    inline Var Move(Var from, MapKey&& map, Var to = Obj::Map()) {
        return Move(from, map, to);
    }
    inline Var Move(Var from, Var map, Var to = Obj::Map()) {
        return Move(from, ToMapKey(map), to);
    }
    /**
     * ------------------------------------------------------------------------
     * Link
     * ------------------------------------------------------------------------
     */
    inline Var Link(Var from, MapKey& map, Var to = Obj::Map()){
        for (auto& v : map) 
            to = Insert(v.second, Find(v.first, from), to);
        return to;
    }
    /**
     * derived
     */
    inline Var Link(Var from, MapKey&& map, Var to = Obj::Map()) {
        return Link(from, map, to);
    }
    inline Var Link(Var from, Var map, Var to = Obj::Map()) {
        return Link(from, ToMapKey(map), to);
    }
    /**
     * ------------------------------------------------------------------------
     * Normalize var 
     * ------------------------------------------------------------------------
     */ 
    Var Normalize(Var var);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SEDIT_H */

