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
     *                                   ___ _____ ___       _________ ___
     * Insert var (A) on other var(B)   |_A_|_A&B_|_B_| ->  |____A____|_B_| 
     * ------------------------------------------------------------------------
     */
    Var&  Insert(const Var& var,  Var& on);
    Map&  Insert(const Map& var,  Map& on);
    List& Insert(const List& var, List& on);
    /**
     * Insert utilities 
     */
    inline Var Insert(List&& var, Var on = nullptr) {
        return Insert(var, on);
    }
    Var Insert(List& var, Var on = nullptr) {
        for (Var v : var) {
            on = Insert(v, on);
        }
        return on;
    }
    /**
     * -------------------------------------------------------------------- 
     *                                   ___ _____ ___       _ _____ ___ 
     * Update var (A) on other var (B)  |_A_|_A&B_|_B_| ->  ~_|__A__|_B_| 
     * --------------------------------------------------------------------
     */
    Var&  Update(const Var&  var, Var   on);
    Map&  Update(const Map&  var, Map&  on);
    List& Update(const List& var, List& on);
    /**
     * Update utilities 
     */
    Var Update(List&& var, Var on = nullptr) {
        return Update(var, on);
    }
    Var Update(List& var, Var on = nullptr) {
        for (Var v : var) {
            on = Update(v, on);
        }
        return on;
    }
    /**
     * --------------------------------------------------------------------
     *                                   ___ _____ ___       _ _____ _
     *  Find var (A) on other var (B)   |_A_|_A&B_|_B_| ->  ~_|_A&B_|_~
     * --------------------------------------------------------------------
     */
    Var&  Find(Var& var,  const Var&  on);
    Map&  Find(Map& var,  const Map&  on);
    List& Find(List& var, const List& on);
    /**
     * --------------------------------------------------------------------
     *                                   ___ _____ ___       _ ___
     *  Remove var (A) on other var (B) |_A_|_A&B_|_B_| ->  ~_|_B_|
     * --------------------------------------------------------------------
     */
    Var   Remove(const Var& var,  Var   on);
    Map&  Remove(const Map& var,  Map&  on);
    List& Remove(const List& var, List& on);
    /**
     * Remove utilities 
     */
    Var Remove(List&& var, Var on = nullptr) {
        return Remove(var, on);
    }
    Var Remove(List& var, Var on = nullptr) {
        for (Var v : var) {
                on = Remove(v, on);
        }
        return on;
    }
    /**
     * -------------------------------------------------------------------
     *                                                            ___ _____ ___       _ ___
     *  Delete var (A) on other var (B) and clear empty branches |_A_|_A&B_|_B_| ->  ~_|_B_|
     * --------------------------------------------------------------------
     */
    Var&  Delete(const Var& var,  Var&  on);
    Map&  Delete(const Map& var,  Map&  on);
    List& Delete(const List& var, List& on);
    /**
     * Remove utilities
     */
    static inline Var Delete(List&& var, Var on = nullptr) {
        return Delete(var, on);
    }
    static inline Var Delete(List& var, Var on = nullptr) {
        for (Var v : var) {
                on = Delete(v, on);
        }
        return on;
    }
    /**
     * --------------------------------------------------------------------
     * verify match from var (A) on (B)
     * --------------------------------------------------------------------
     */
    Boolean Match(const Var&  var, const Var&  on);
    Boolean Match(const Map&  var, const Map&  on);
    Boolean Match(const List& var, const List& on);
    /**
     * --------------------------------------------------------------------
     * Count var (A) on other var (B)
     * --------------------------------------------------------------------
     */
    Integer Count(String var, Var on);
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
        for (auto& v : var) {
                on = Insert(v.first, v.second, on);
        }
        return on;
    }
    inline Var Insert(Map&& var, Var on = nullptr) {
        return Insert(var, on);
    }
    inline List Insert(Map& var, List on) {
        for (auto& e : on) { 
                Insert(var, e);
        }
        return move(on);
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
    inline Var Update(Map&& var, Var on = nullptr) {
        return Update(var, on);
    }
    inline Var Update(Map& var, Var on = nullptr) {
        for (auto& v : var) {
                on = Update(v.first, v.second, on);
        }
        return on;
    }
    inline List Update(Key path, Var var, List on) {
        for (Var v : on) {
                Update(path, var, v);
        }
        return move(on);
    }
    inline List Update(Map&& var, List on) {
        return move(Update(var, on));
    }
    inline List Update(Map& var, List on) {
        for (auto& e : on) { 
                Update(var, e);
        }
        return move(on);
    }
    /**
     * Update all branches deeper than (deep) with (var)
     */
    Var Update(Integer deep, Var var, Var on);
    
    /*-------------------------------------------------------------------------------------------------------------*
     * 
     * Find Key on var 
     *-------------------------------------------------------------------------------------------------------------*/
    static Var Find(Key path, Var on);
    //
    static inline List Find(ListKey paths, Var on) {
        List l;
        for (auto& p : paths) {
                l.push_back(Find(p, on));
        }
        return move(l);
    }
    //
    static inline List Find(Key path, List on) {
        List l;
        for (Var o : on) {
                l.push_back(Find(path, o));
        }
        return move(l);
    }
    /*-------------------------------------------------------------------------------------------------------------*
     * 
     * Remove Key on var 
     *-------------------------------------------------------------------------------------------------------------*/
    static Var Remove(Key path, Var on);
    //
    static inline List Remove(ListKey paths, Var on) {
        List l;
        for (auto& p : paths) {
                l.push_back(Remove(p, on));
        }
        return move(l);
    }
    //
    static inline List Remove(Key path, List on) {
        List l;
        for (auto& o : on) {
                l.push_back(Remove(path, o));
        }
        return move(l);
    }
    /*-------------------------------------------------------------------------------------------------------------*
     * 
     * Delete Key on var 
     *-------------------------------------------------------------------------------------------------------------*/
    static inline Var Delete(Key path, Var on){
        Remove(path, on);
        return on;
    }
    //
    static inline List Delete(ListKey paths, Var on) {
        for (auto& p : paths) {
                Remove(p, on);
        }
        return move(on);
    }
    //
    static inline List Delete(Key path, List on) {
        for (auto& o : on) {
                Remove(path, o);
        }
        return move(on);
    }
    /*-------------------------------------------------------------------------------------------------------------*
     * Rename Key on var 
     *-------------------------------------------------------------------------------------------------------------*/
    static inline Var Rename(Key from, Key to, Var on){
        return Var::Trim(Insert(to, Remove(from, on), on));
    }
    /*-------------------------------------------------------------------------------------------------------------*
     * Move
     *-------------------------------------------------------------------------------------------------------------*/
    static inline Var Move(Var from, MapKey& map, Var to = Obj::Map()){
        for (auto& v : map) {
                to = Insert(v.second, Remove(v.first, from), to);
        }
        return to;        
    }
    //
    static inline Var Move(Var from, MapKey&& map, Var to = Obj::Map()) {
        return Move(from, map, to);
    }
    //
    static inline Var Move(Var from, Var map, Var to = Obj::Map()) {
        return Move(from, ToMapKey(map), to);
    }
    /*-------------------------------------------------------------------------------------------------------------*
     * Link
     *-------------------------------------------------------------------------------------------------------------*/
    static inline Var Link(Var from, MapKey& map, Var to = Obj::Map()){
        for (auto& v : map) {
                to = Insert(v.second, Find(v.first, from), to);
        }
        return to;
    }
    static inline Var Link(Var from, MapKey&& map, Var to = Obj::Map()) {
        return Link(from, map, to);
    }
    static inline Var Link(Var from, Var map, Var to = Obj::Map()) {
        return Link(from, ToMapKey(map), to);
    }
    /**
     * --------------------------------------------------------------------
     * Normalize var ()
     * --------------------------------------------------------------------
     */ 
    static Var Normalize(Var var);
    /**
     * --------------------------------------------------------------------
     * Utilities
     * --------------------------------------------------------------------
     */
    static inline MapKey ToMapKey(Var map) {
        MapKey ret;
        for (auto& v : Var::ToMap(map)) {
                ret[v.first] = Var::ToString(v.second);
        }
        return ret;
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* SEDIT_H */

