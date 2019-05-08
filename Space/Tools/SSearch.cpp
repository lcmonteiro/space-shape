
/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   Search.cc
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#include <iostream>
/**
 * space
 */
#include "SSearch.h"
#include "SEdit.h"
/**
 * ---------------------------------------------------------------------------------------------------------------------
 * Helpers
 * ---------------------------------------------------------------------------------------------------------------------
 * Match
 * ------------------------------------------------------------------------------------------------
 */
static inline Boolean __Match(Var val, Var on);
/**
 * ------------------------------------------------------------------------------------------------
 * Match Select
 * ------------------------------------------------------------------------------------------------
 */
static inline Boolean __MatchSelect(List& expr, Var on);
static inline Boolean __MatchSelect(Map&  expr, Var on);
/**
 * ------------------------------------------------------------------------------------------------
 * Match Policy
 * ------------------------------------------------------------------------------------------------
 */
static inline Boolean __MatchPolicy(List& expr, Var on);
static inline Boolean __MatchPolicy(Map&  expr, Var on);
/**
 * ------------------------------------------------------------------------------------------------
 * Match Compare
 * ------------------------------------------------------------------------------------------------
 */
static inline Boolean __MatchCompare(List val, const String& op, Var on);
static inline Boolean __MatchCompare(Var  val, const String& op, Var on);
/**
 * ------------------------------------------------------------------------------------------------
 * operators
 * ------------------------------------------------------------------------------------------------
 */
static inline Boolean __MatchGreatThan( Var val, Var on);
static inline Boolean __MatchLessThan ( Var val, Var on);
static inline Boolean __MatchEqual    ( Var val, Var on);
static inline Boolean __MatchRegex    ( Var val, Var on);
/**
 * ------------------------------------------------------------------------------------------------
 * Find path filter
 * ------------------------------------------------------------------------------------------------
 */
static inline Var __FindFilter(sregex_iterator it, sregex_iterator& end, Var on); 
/**
 * ------------------------------------------------------------------------------------------------
 * Delete path filter
 * ------------------------------------------------------------------------------------------------
 */
static inline Boolean __DeleteFilter(sregex_iterator it, sregex_iterator& end, Var on); 
/**
 * ---------------------------------------------------------------------------------------------------------------------
 * Search - Implementation
 * ---------------------------------------------------------------------------------------------------------------------
 * Find
 * ------------------------------------------------------------------------------------------------
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
Var Search::Find(Var expr, Var& on) {
    if (Var::IsMap(on)) {
        return Obj(Find(expr, Var::Map(on)));
    }
    if (Var::IsList(on)) {
        return Obj(Find(expr, Var::List(on)));
    }
    if (__Match(expr, on)) {    
        return on;
    }
    return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Remove
 * ------------------------------------------------------------------------------------------------
 */
List Search::Remove(Var expr, List& on){
    ::List r;
    for (List::iterator it = on.begin(); it != on.end();) {
        if (__Match(expr, *it)) {    
            r.push_back(*it);
            it = on.erase(it);
            continue;
        }
        ++it;
    }
    return r;
}
Map Search::Remove(Var expr, Map& on) {
    ::Map r;
    for (Map::iterator it = on.begin(); it != on.end();) {
        if (__Match(expr, it->second)) {            
            r.insert(*it);
            it = on.erase(it);
            continue;
        }
        ++it;
    }
    return r;
}
Var Search::Remove(Var expr, Var& on) {
    if (Var::IsMap(on)) {
        return Obj(Remove(expr, Var::Map(on)));
    } 
    if (Var::IsList(on)) {
        return Obj(Remove(expr, Var::List(on)));
    }
    if (__Match(expr, on)) {
        return on;
    }
    return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Delete
 * ------------------------------------------------------------------------------------------------
 */
Var Search::Delete(Var expr, Var on) {
    if (Var::IsMap(on)) {
        ::Map& m = Var::Map(on);
        for (Map::iterator it = m.begin(); it != m.end();) {
            if (__Match(expr, it->second)) {            
                it = m.erase(it);
                continue;
            }
            ++it;
        }
        return on;
    } else if (Var::IsList(on)) {
        ::List& l = Var::List(on);
        for (List::iterator it = l.begin(); it != l.end();) {
            if (__Match(expr, *it)) {    
                it = l.erase(it);
                continue;
            }
            ++it;
        }
        return on;
    }
    if (__Match(expr, on)) {    
        return on;
    }
    return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Match
 * ------------------------------------------------------------------------------------------------
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
Boolean Search::Match(Var expr, Var& on) {
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
 * Update
 * ----------------------------------------------------------------------------
 **/
Var Search::Update(Key expr, Var var, Var on) {
    static const std::regex e("([^/]+)");

    // iterator ---------------------------------------
    sregex_iterator it(expr.begin(), expr.end(), e);
    sregex_iterator end;
    
    // execute ----------------------------------------
    return __ExecuteFilter(
            it, end, on, [&](Var v){ return var; }, on);
}
/**
 * ----------------------------------------------------------------------------
 * Find
 * ----------------------------------------------------------------------------
 */
Var Search::Find(Key expr, Var on) {
    static const std::regex e("([^/]+)");
    
    // iterator ---------------------------------------
    sregex_iterator it(expr.begin(), expr.end(), e);
    sregex_iterator end;
    
    // execute ----------------------------------------
    return __FindFilter(it, end, on);
}
/**
 * ----------------------------------------------------------------------------
 * Delete
 * ----------------------------------------------------------------------------
 */
Var Search::Delete(Key expr, Var on) {
    static const std::regex e("([^/]+)");

    // iterator ---------------------------------------
    sregex_iterator it(expr.begin(), expr.end(), e);
    sregex_iterator end;
    
    // execute ----------------------------------------
    __DeleteFilter(it, end, on);

    return on;
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * helpers implementation
 * --------------------------------------------------------------------------------------------------------------------
 * Match
 * ------------------------------------------------------------------------------------------------
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
 * ------------------------------------------------------------------------------------------------
 * Match Select
 * ------------------------------------------------------------------------------------------------
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
            if (!__MatchPolicy(Var::List(e.second), Edit::Find(e.first, on))) {
                return false;
            }
            continue;
        }
        if (Var::IsMap(e.second)) {
            if (!__MatchPolicy(Var::Map(e.second), Edit::Find(e.first, on))) {
                return false;
            }
            continue;
        }
        if (!__MatchEqual(e.second, Edit::Find(e.first, on))) {
            return false;
        }
    }
    return true;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Match Policy
 * ------------------------------------------------------------------------------------------------
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
 * ------------------------------------------------------------------------------------------------
 * Match Compare
 * ------------------------------------------------------------------------------------------------
 */
Boolean __MatchCompare(List val, const String& op, Var on) {
    if (Search::$eq == op) {
        for (Var v : val) {
            if (__MatchEqual(v, on)) {
                return true;
            }
        }
        return false;
    }
    if (Search::$ne == op) {
        for (Var v : val) {
            if (__MatchEqual(v, on)) {
                return false;
            }
        }
        return true;
    }
    if (Search::$gt == op) {
        for (Var v : val) {
            if (__MatchGreatThan(v, on)) {
                return true;
            }
        }
        return false;
    }
    if (Search::$lt == op) {
        for (Var v : val) {
            if (__MatchLessThan(v, on)) {
                return true;
            }
        }
        return false;
    }
    if (Search::$regex == op) {
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
    if (Search::$eq == op) {
        return __MatchEqual(val, on);
    }
    if (Search::$ne == op) {
        return !__MatchEqual(val, on);
    }
    if (Search::$gt == op) {
        return __MatchGreatThan(val, on);
    }
    if (Search::$lt == op) {
        return __MatchLessThan(val, on);
    }
    if (Search::$regex == op) {
        return __MatchRegex(val, on);
    }
    return false;
}
/**
 * ------------------------------------------------------------------------------------------------
 * operators
 * ------------------------------------------------------------------------------------------------
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
    return std::regex_match(Var::ToString(on), std::regex(Var::ToString(val)));
}
/**
 * ------------------------------------------------------------------------------------------------
 * Find Filter
 * ------------------------------------------------------------------------------------------------
 */
Var __FindFilter(sregex_iterator it, sregex_iterator& end, Var on) {
    // end condition --------------------------------------
    if (it == end) {
        return on;
    }
    // get next key ---------------------------------------
    Key k = it->str();

    // get next value -------------------------------------
    Var n = on[k];

    // no regex -------------------------------------------
    if (Var::IsDefined(n)) {
        // find next
        Var f = __FindFilter(++it, end, n);
        // check result
        if (Var::IsUndefined(f)) {
            return Obj(nullptr);
        }
        if (Var::IsMap(on)) {
            return Obj{{k, f}};
        }
        if (Var::IsList(on)) {
            return Obj{f};
        }
    }
    // regex - map case -----------------------------------
    if (Var::IsMap(on)) {
        std::regex expr(k);
        // go to next position
        ++it;
        // iterate
        Map m;
        for (auto v : Var::Map(on)) {
            if (std::regex_match(v.first, expr)) {
                // try to find
                Var f = __FindFilter(it, end, v.second);
                // save if found
                if (Var::IsDefined(f)) {
                    m[v.first] = f;
                }
            }
        }
        return m.size() ? Obj(m) : Obj(nullptr);
    }
    // regex - list case -----------------------------------
    if (Var::IsList(on)) {
        // validate regex
        std::regex expr(k);
        // go to next position
        ++it;
        // iterate
        List    l;
        Integer i;
        for (Var v : Var::List(on)) {
            if (std::regex_match(String::ValueOf(i++), expr)) {
                // try to find
                Var f = __FindFilter(it, end, v);
                // save if found
                if (Var::IsDefined(f)) {
                    l.push_back(f);
                }
            }
        }
        return l.size() ? Obj(l) : Obj(nullptr);
    }
    return Obj(nullptr);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Delete Filter
 * ------------------------------------------------------------------------------------------------
 */
Boolean __DeleteFilter(sregex_iterator it, sregex_iterator& end, Var on) {
    // end condition --------------------------------------
    if (it == end) {
        return true;
    }

    // map case -------------------------------------------
    if (Var::IsMap(on)) {
        ::Map& m = Var::Map(on);
        // try to find
        auto find = m.find(it->str());
        if (find != m.end()) {
            if (__DeleteFilter(++it, end, find->second)) {
                m.erase(find);
            }
            return m.empty();
        }
        // validate regex
        std::regex expr(it->str());
        ++it;
        for (auto mit = m.begin(); mit != m.end();) {
            if (std::regex_match(mit->first, expr)) {
                if (__DeleteFilter(it, end, mit->second)) {
                    mit = m.erase(mit);
                    continue;
                }
            }
            ++mit;
        }
        return m.empty();
    }
    // list case ------------------------------------------
    if (Var::IsList(on)) {
        ::List& l = Var::List(on);
        // try to find
        auto find = l.begin() + Integer::ValueOf(it->str());
        if (find < l.end()) {
            if (__DeleteFilter(++it, end, *find)) {
                l.erase(find);
            }
            return l.empty();
        }
        // validate regex
        std::regex expr(it->str());
        ++it;
        for (auto lit = l.begin(); lit != l.end();) {
            if (std::regex_match(String::ValueOf(Integer(std::distance(l.begin(), lit))), expr)) {
                if (__DeleteFilter(it, end, *lit)) {
                    lit = l.erase(lit);
                    continue;
                }
            }
            ++lit;
        }
        return l.empty();
    }
    return false;
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */