
/**
 * ------------------------------------------------------------------------------------------------
 * File:   Search.cc
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#include <iostream>
/**
 * space
 */
#include "SSearch.h"
#include "SEdit.h"
#include "SKeys.h"
/**
 * namespaces
 */
using namespace std;
/**
 * ------------------------------------------------------------------------------------------------
 * Helpers
 * ------------------------------------------------------------------------------------------------
 * Match
 * ----------------------------------------------------------------------------
 */
static inline Boolean __Match(Var val, Var on);
/**
 * ----------------------------------------------------------------------------
 * Match Select
 * ----------------------------------------------------------------------------
 */
static inline Boolean __MatchSelect(List& expr, Var on);
static inline Boolean __MatchSelect(Map&  expr, Var on);
/**
 * ----------------------------------------------------------------------------
 * Match Policy
 * ----------------------------------------------------------------------------
 */
static inline Boolean __MatchPolicy(List& expr, Var on);
static inline Boolean __MatchPolicy(Map&  expr, Var on);
/**
 * ----------------------------------------------------------------------------
 * Match Compare
 * ----------------------------------------------------------------------------
 */
static inline Boolean __MatchCompare(List val, const String& op, Var on);
static inline Boolean __MatchCompare(Var  val, const String& op, Var on);
/**
 * ----------------------------------------------------------------------------
 * operators
 * ----------------------------------------------------------------------------
 */
static inline Boolean __MatchGreatThan( Var val, Var on);
static inline Boolean __MatchLessThan ( Var val, Var on);
static inline Boolean __MatchEqual    ( Var val, Var on);
static inline Boolean __MatchRegex    ( Var val, Var on);
/**
 * ----------------------------------------------------------------------------
 * Find path filter
 * ----------------------------------------------------------------------------
 */
static inline Var __FindFilter(
    sregex_iterator it, sregex_iterator& end, Var on); 
/**
 * ----------------------------------------------------------------------------
 * Delete path filter
 * ----------------------------------------------------------------------------
 */
static inline Boolean __DeleteFilter(
    sregex_iterator it, sregex_iterator& end, Var on); 
/**
 * ----------------------------------------------------------------------------
 * Execute path filter
 * ----------------------------------------------------------------------------
 */
static inline Var __ExecuteFilter(
    sregex_iterator  it, 
    sregex_iterator& end, 
    function<Var(const Key&, Var)> func, Key path, Var on);
/**
 * ------------------------------------------------------------------------------------------------
 * Search - Implementation
 * ------------------------------------------------------------------------------------------------
 * Find
 * ----------------------------------------------------------------------------
 */
List Search::Find(Var expr, const List& on) {
    List out;
    for (Var v : on) {
        if (__Match(expr, v)) {
            out.push_back(v);
        }
    }
    return out;
}
Map Search::Find(Var expr, const Map& on) {
    Map out;
    for (auto v : on) {
        if (__Match(expr, v.second)) {
            out[v.first] = v.second;
        }
    }
    return out;
}
Var Search::Find(Var expr, Var on) {
    if (Var::IsMap(on)) {
        return Obj(Find(expr, Var::Map(on)));
    }
    if (Var::IsList(on)) {
        return Obj(Find(expr, Var::List(on)));
    }
    if (__Match(expr, on)) {    
        return on;
    }
    return Obj();
}
/**
 * ----------------------------------------------------------------------------
 * Remove
 * ----------------------------------------------------------------------------
 */
List Search::Remove(Var expr, List& on){
    auto r = List();
    for (auto it = on.begin(); it != on.end();) {
        if (__Match(expr, *it)) {    
            r.push_back(*it);
            it = on.erase(it);
        } else {
            ++it;
        }
    }
    return r;
}
Map Search::Remove(Var expr, Map& on) {
    auto r = Map();
    for (auto it = on.begin(); it != on.end();) {
        if (__Match(expr, it->second)) {            
            r.insert(*it);
            it = on.erase(it);
        } else {
            ++it;
        }
    }
    return r;
}
Var Search::Remove(Var expr, Var on) {
    if (Var::IsMap(on)) {
        return Obj(Remove(expr, Var::Map(on)));
    } 
    if (Var::IsList(on)) {
        return Obj(Remove(expr, Var::List(on)));
    }
    if (__Match(expr, on)) {
        return on;
    }
    return Obj();
}
/**
 * ----------------------------------------------------------------------------
 * Delete
 * ----------------------------------------------------------------------------
 */
List& Search::Delete(Var expr, List& on){
    for (auto it = on.begin(); it != on.end();) {
        if (__Match(expr, *it)) {    
            it = on.erase(it);
        } else {
            ++it;
        }
    }
    return on;
}
Map& Search::Delete(Var expr, Map& on) {
    for (auto it = on.begin(); it != on.end();) {
        if (__Match(expr, it->second)) {            
            it = on.erase(it);
        } else {
            ++it;
        } 
    }
    return on;
}
Var Search::Delete(Var expr, Var on) {
    if (Var::IsMap(on)) {
        Delete(expr, Var::Map(on));
        return on;
    }
    if (Var::IsList(on)) {
        Delete(expr, Var::List(on));
        return on;
    }
    if (__Match(expr, on)) {    
        return on;
    }
    return Obj();
}
/**
 * ----------------------------------------------------------------------------
 * Match
 * ----------------------------------------------------------------------------
 */
Boolean Search::Match(Var expr, const List& on) {
    for (auto it = on.begin(); it != on.end(); ++it) {
        if (__Match(expr, *it)) {    
            return true;
        }
    }
    return false;
}
Boolean Search::Match(Var expr, const Map& on) {
    for (auto it = on.begin(); it != on.end(); ++it) {
        if (__Match(expr, it->second)) {            
            return true;
        }
    }
    return false;
}
Boolean Search::Match(Var expr, Var on) {
    if (Var::IsList(on)) {
        return Match(expr, Var::List(on));
    }
    if (Var::IsMap(on)) {
        return Match(expr, Var::Map(on));
    }
    return __Match(expr, on);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Match Tree
 * ------------------------------------------------------------------------------------------------
 * Execute
 * ----------------------------------------------------------------------------
 **/
Var Search::Execute(Key expr, function<Var(const Key&, Var)> func, Var on) {
    const std::regex e("([^/]+)");
    /**
     * iterators
     */
    auto it   = std::sregex_iterator(expr.begin(), expr.end(), e);
    auto end  = std::sregex_iterator();
    auto path = Key("/");
    /**
     * execute
     */
    return __ExecuteFilter(it, end, func, path, on);
}
/**
 * ----------------------------------------------------------------------------
 * Update
 * ----------------------------------------------------------------------------
 */
Var Search::Update(Key expr, Var var, Var on) {
    /**
     * execute
     */
    return Execute(expr, [&var](Key, Var){ return var; }, on);
}
/**
 * ----------------------------------------------------------------------------
 * Find
 * ----------------------------------------------------------------------------
 */
Var Search::Find(Key expr, Var on) {
    static const regex e("([^/]+)");
    /**
     * iterators
     */
    sregex_iterator it(expr.begin(), expr.end(), e);
    sregex_iterator end;
    /**
     * execute
     */
    return __FindFilter(it, end, on);
}
/**
 * ----------------------------------------------------------------------------
 * Delete
 * ----------------------------------------------------------------------------
 */
Var Search::Delete(Key expr, Var on) {
    static const regex e("([^/]+)");
    /**
     * iterators
     */
    sregex_iterator it(expr.begin(), expr.end(), e);
    sregex_iterator end;
    /**
     * execute
     */
    __DeleteFilter(it, end, on);
    return on;
}
/**
 * ------------------------------------------------------------------------------------------------
 * helpers implementation
 * ------------------------------------------------------------------------------------------------
 * Match
 * ----------------------------------------------------------------------------
 */
Boolean __Match(Var expr, Var on) {
    try {
        if (Var::IsMap(expr)) {
            return __MatchSelect(Var::Map(expr), on);   
        }
        if (Var::IsList(expr)) {
            return __MatchSelect(Var::List(expr), on);
        }
        return __MatchEqual(expr, on);
    } catch(logic_error&) {
        return false;
    }
}
/**
 * ----------------------------------------------------------------------------
 * Match Select
 * ----------------------------------------------------------------------------
 */
Boolean __MatchSelect(List& expr, Var on) {
    for (Var e : expr) {
        if (Var::IsList(e)) {
            if (__MatchSelect(Var::List(e), on)) {
                return true;
            }
            continue;
        }
        if (Var::IsMap(e)) {
            if (__MatchSelect(Var::Map(e), on)) {
                return true;
            }
            continue;
        }
        if (__MatchEqual(e, on)) {
            return true;
        }
    }
    return false;
}
Boolean __MatchSelect(Map& expr, Var on) {
    for (auto& e : expr) {
        if (Var::IsList(e.second)) {
            if (!__MatchPolicy(
                Var::List(e.second), Edit::Find(e.first, on))
                ) {
                return false;
            }
            continue;
        }
        if (Var::IsMap(e.second)) {
            if (!__MatchPolicy(
                Var::Map(e.second), Edit::Find(e.first, on))
                ) {
                return false;
            }
            continue;
        }
        if (!__MatchEqual(
            e.second, Edit::Find(e.first, on))
            ) {
            return false;
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Match Policy
 * ----------------------------------------------------------------------------
 */
Boolean __MatchPolicy(List& expr, Var on) {
    for (Var e : expr) {
        if (Var::IsList(e)) {
            if (__MatchPolicy(Var::List(e), on)) {
                return true;
            }
            continue;
        }
        if (Var::IsMap(e)) {
            if (__MatchPolicy(Var::Map(e), on)) {
                return true;
            }
            continue;
        }
        if (__MatchEqual(e, on)) {
            return true;
        }
    }
    return false;
}
Boolean __MatchPolicy(Map& expr, Var on) {
    for (auto& e : expr) {
        if (Var::IsList(e.second)) {
            if (!__MatchCompare(Var::List(e.second), e.first, on)) {
                return false;
            }
            continue;
        }
        if (!__MatchCompare(e.second, e.first, on)) {
            return false;
        }
    }
    return true;
}
/**
 * ----------------------------------------------------------------------------
 * Match Compare
 * ----------------------------------------------------------------------------
 */
Boolean __MatchCompare(List val, const String& op, Var on) {
    if (Keys::$eq == op) {
        for (Var v : val) {
            if (__MatchEqual(v, on)) {
                return true;
            }
        }
        return false;
    }
    if (Keys::$ne == op) {
        for (Var v : val) {
            if (__MatchEqual(v, on)) {
                return false;
            }
        }
        return true;
    }
    if (Keys::$gt == op) {
        for (Var v : val) {
            if (__MatchGreatThan(v, on)) {
                return true;
            }
        }
        return false;
    }
    if (Keys::$lt == op) {
        for (Var v : val) {
            if (__MatchLessThan(v, on)) {
                return true;
            }
        }
        return false;
    }
    if (Keys::$regex == op) {
        for (Var v : val) {
            if (__MatchRegex(v, on)) {
                return true;
            }
        }
        return false;
    }
    return false;
}
Boolean __MatchCompare(Var val, const String& op, Var on) {
    if (Keys::$eq == op) {
        return __MatchEqual(val, on);
    }
    if (Keys::$ne == op) {
        return !__MatchEqual(val, on);
    }
    if (Keys::$gt == op) {
        return __MatchGreatThan(val, on);
    }
    if (Keys::$lt == op) {
        return __MatchLessThan(val, on);
    }
    if (Keys::$regex == op) {
        return __MatchRegex(val, on);
    }
    return false;
}
/**
 * ----------------------------------------------------------------------------
 * operators
 * ----------------------------------------------------------------------------
 */
Boolean __MatchGreatThan(Var val, Var on) {
    if (
        Var::IsFloat(on) ||
        Var::IsInteger(on)
        ) {
        return Float(on) < Float(val);
    }
    return (String(on).compare(String(val)) > 0);
}
Boolean __MatchLessThan(Var val, Var on) {
    if (
        Var::IsFloat(on) ||
        Var::IsInteger(on)
        ) {
        return Float(on) > Float(val);
    }
    return (String(on).compare(String(val)) < 0);
}
Boolean __MatchEqual(Var val, Var on) {
    if (
        Var::IsFloat(on) ||
        Var::IsInteger(on)
        ) {
        return Float(on) == Float(val);
    }
    return (String(on).compare(String(val)) == 0);
}
Boolean __MatchRegex(Var val, Var on) {
    return regex_match(
        Var::ToString(on), regex(Var::ToString(val)));
}
/**
 * ----------------------------------------------------------------------------
 * Find Filter
 * ----------------------------------------------------------------------------
 */
Var __FindFilter(sregex_iterator it, sregex_iterator& end, Var on) {
    /**
     * end condition
     */
    if (it == end) {
        return on;
    }
    /**
     * get next key and value
     */
    auto key  = it->str();
    auto next = on[key];
    /**
     * ----------------------------------------------------
     * no regex 
     * ----------------------------------------------------
     */ 
    if (Var::IsDefined(next)) {
        // find next
        auto found = __FindFilter(++it, end, next);
        // check result
        if (Var::IsUndefined(found)) {
            return Obj();
        }
        if (Var::IsMap(on)) {
            return Obj{{key, found}};
        }
        if (Var::IsList(on)) {
            return Obj{found};
        }
        return found;
    }
    /**
     * ----------------------------------------------------
     *  regex - map case
     * ----------------------------------------------------
     */
    if (Var::IsMap(on)) {
        // go to next position
        ++it;
        // foreach map element
        auto exp = regex(key);
        auto map = Map();
        for (auto v : Var::Map(on)) {
            if (regex_match(v.first, exp)) {
                // try to find
                auto found = __FindFilter(it, end, v.second);
                // save if found
                if (Var::IsDefined(found)) {
                    map[v.first] = found;
                }
            }
        }
        return map.size() ? Obj(map) : Obj();
    }
    /**
     * ----------------------------------------------------
     *  regex - list case 
     * ----------------------------------------------------
     */
    if (Var::IsList(on)) {
        // go to next position
        ++it;
        // foreach list element
        auto expr = regex(key);
        auto list = List();
        auto pos  = Integer();
        for (Var v : Var::List(on)) {
            if (regex_match(String::ValueOf(pos++), expr)) {
                // try to find
                auto found = __FindFilter(it, end, v);
                // save if found
                if (Var::IsDefined(found)) {
                    list.push_back(found);
                }
            }
        }
        return list.size() ? Obj(list) : Obj();
    }
    return Obj();
}
/**
 * ------------------------------------------------------------------------------------------------
 * Delete Filter
 * ------------------------------------------------------------------------------------------------
 */
Boolean __DeleteFilter(sregex_iterator it, sregex_iterator& end, Var on) {
    /**
     * end condition
     */
    if (it == end) {
        return true;
    }
    /**
     * get key and move to next
     */
    auto key = (it++)->str();
    /**
     * ----------------------------------------------------
     * map case
     * ----------------------------------------------------
     */
    if (Var::IsMap(on)) {
        auto& map = Var::Map(on);
        // try to find
        auto found = map.find(key);
        if (found != map.end()) {
            if (__DeleteFilter(++it, end, found->second)) {
                map.erase(found);
            }
            return map.empty();
        }
        // validate regex
        auto expr = regex(key);
        for (auto mit = map.begin(); mit != map.end();) {
            if (regex_match(mit->first, expr)) {
                if (__DeleteFilter(it, end, mit->second)) {
                    mit = map.erase(mit);
                    continue;
                }
            }
            ++mit;
        }
        return map.empty();
    }
    /**
     * ----------------------------------------------------
     *  list case
     * ----------------------------------------------------
     */
    if (Var::IsList(on)) {
        auto& list = Var::List(on);
        // try to find
        auto found = list.begin() + Integer::ValueOf(key);
        if (found < list.end()) {
            if (__DeleteFilter(++it, end, *found)) {
                list.erase(found);
            }
            return list.empty();
        }
        // validate regex
        auto expr = regex(key);
        for (auto lit = list.begin(); lit != list.end();) {
            // compute position
            auto pos = Integer(distance(list.begin(), lit));
            // try to match
            if (regex_match(String::ValueOf(pos), expr)) {
                if (__DeleteFilter(it, end, *lit)) {
                    lit = list.erase(lit);
                    continue;
                }
            }
            ++lit;
        }
        return list.empty();
    }
    return false;
}
/**
 * ----------------------------------------------------------------------------
 * Execute Filter
 * ----------------------------------------------------------------------------
 */
static inline Var __ExecuteFilter(
    sregex_iterator  it, 
    sregex_iterator& end, 
    function<Var(const Key&, Var)> func, Key path, Var on
    ) {
    /**
     * end condition
     */
    if (it == end) {
        return func(path, on); 
    }
    /**
     * get key and move to next
     */
    auto key = (it++)->str();
    /**
     * ----------------------------------------------------
     * map case 
     * ----------------------------------------------------
     */
    if (Var::IsMap(on)) {
        auto& map = Var::Map(on);
        // try to find 
        auto found = map.find(key);
        if (found != map.end()) {
            found->second = __ExecuteFilter(it, end, 
                func, path + found->first, found->second);
            return on;
        }
        // try to find by regex
        auto expr = regex(key);
        for (auto mit = map.begin(); mit != map.end(); ++mit) {
            if (std::regex_match(mit->first, expr)) {
                mit->second = __ExecuteFilter(it, end, 
                    func, path + mit->first, mit->second);
            }
        }
        return on;
    }
    /**
     * ----------------------------------------------------
     * list case 
     * ----------------------------------------------------
     */
    if (Var::IsList(on)) {
        auto& list = Var::List(on);
        try {
            // try to find
            auto found = list.begin() + Integer::ValueOf(key);
            if (found != list.end()) {
                *found = __ExecuteFilter(
                    it, end, func, path + key, *found);
                return on;
            }
        } catch (std::invalid_argument& ex) {
            // try to find by regex 
            auto expr = regex(key);
            for (auto lit = list.begin(); lit != list.end(); ++lit) {
                // compute position
                auto str = String::ValueOf(
                    Integer(std::distance(list.begin(), lit)));
                // try to match
                if (regex_match(str, expr)) {
                    *lit = __ExecuteFilter(
                        it, end, func, path + str, *lit);
                }
            }
        }
        return on;
    }
    return on;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */