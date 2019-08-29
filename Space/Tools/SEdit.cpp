
/**
 * ------------------------------------------------------------------------------------------------
 * File:   SEdit.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 * std
 */
#include <regex>
#include <iostream>
#include <list>
/**
 * space
 */
#include "SSearch.h"
#include "SEdit.h"
/***
 * ------------------------------------------------------------------------------------------------
 * Match Content
 * ------------------------------------------------------------------------------------------------
 */
static Boolean __MatchContent(const Link& var, const Link& on) {
    if (Var::IsInteger(var) && Var::IsInteger(on)) {  
        return Var::Integer(var) == Var::Integer(on);    
    }
    if (Var::IsString(var) && Var::IsString(on)) {  
        return Var::String(var) == Var::String(on);    
    }
    if (Var::IsBuffer(var) && Var::IsBuffer(on)) {  
        return Var::Buffer(var) == Var::Buffer(on);    
    }
    return Var::ToString(var) == Var::ToString(on);
}
/***
 * ------------------------------------------------------------------------------------------------
 * Match
 * ------------------------------------------------------------------------------------------------
 */
static Boolean __Match(const Link& var, const Link& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline Boolean __Match(const Map& v, const Map& o) {
    if (v.size() != o.size()) {
        return false;
    }
    for (auto it = v.begin(), end = v.end(); it != end; ++it) {
        auto find = o.find(it->first);
        if (find == o.end()) {
            return false;
        }
        if (!__Match(it->second, find->second)){
            return false;
        }
    }
    return true;
}
Boolean Edit::Match(const Map& v, const Map& o) {
    return __Match(v, o);
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline Boolean __Match(const List& v, const List& o) {
    if (v.size() != o.size()) {
        return false;
    }
    for (auto vit = v.begin(), oit = o.begin(); oit != o.end(); ++vit, ++oit) {
        if (!__Match(*vit, *oit)){
            return false;
        }
    }
    return true;
}
Boolean Edit::Match(const List& v, const List& o) {
    return __Match(v, o);
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static Boolean __Match(const Link& var, const Link& on) {
    try {
        if (var == on) {
            return true;
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {    
            return __Match(Var::Map(var), Var::Map(on));
        }
        if (Var::IsList(var) && Var::IsList(on)) {      
            return __Match(Var::List(var), Var::List(on));;
        }
        return __MatchContent(var, on);
    } catch (...) {}
    // not match
    return false;
}
Boolean Edit::Match(const Var& var, const Var& on) {
    return __Match(var, on);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Insert 
 * ------------------------------------------------------------------------------------------------
 **/
static void __Insert(const Link& var, Link& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline void __Insert(const Map& var, Map& on) {
    for (auto& it : var) {
        if (!on.count(it.first)) {
            on[it.first] = it.second;
        } else {
            __Insert(it.second, on[it.first]);
        }
    }   
}
Map& Edit::Insert(const Map& var, Map& on) {
    __Insert(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline void __Insert(const List& var, List& on) {
    for (List::size_type i = 0; i < var.size(); ++i) {
        if (on.size() <= i) {
            on.emplace_back(var[i]);
        } else {
            __Insert(var[i], on[i]);
        }
    }      
}
List& Edit::Insert(const List& var, List& on) {
    __Insert(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * link on link
 * ----------------------------------------------------------------------------
 */
static void __Insert(const Link& var, Link& on) {
    if (var == on) {
        return;
    }
    if (Var::IsMap(var) && Var::IsMap(on)) {
        __Insert(Var::Map(var), Var::Map(on));
        return;
    }
    if (Var::IsList(var) && Var::IsList(on)) {
        __Insert(Var::List(var), Var::List(on));
        return;
    }
    if (Var::IsDefined(var)) {
        on = var; 
    }
}
Var Edit::Insert(const Var var, Var on) {
    if(Var::IsDefined(on)) {
        __Insert(var, on);
    } else {
        on = var;
    }
    return on;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Update
 * ------------------------------------------------------------------------------------------------
 */
static void __Update(const Link& var, Link& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline void __Update(const Map& var, Map& on) {
    for (auto it = var.begin(), end = var.end(); it != end; ++it) {
        auto find = on.find(it->first);
        if (find != on.end()) {
            __Update(it->second, find->second);
        }    
    }      
}
Map& Edit::Update(const Map& var, Map& on) {
    __Update(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline void __Update(const List& var, List& on) {
    for (size_t i = 0; i < var.size() && i < on.size(); ++i) {
        __Update(var[i], on[i]);
    }      
}
List& Edit::Update(const List& var, List& on) {
    __Update(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static void __Update(const Link& var, Link& on) {
    if (var == on) {
        return;
    }
    if (Var::IsMap(var) && Var::IsMap(on)) {    
        __Update(Var::Map(var), Var::Map(on));
    }
    if (Var::IsList(var) && Var::IsList(on)) {
        __Update(Var::List(var), Var::List(on));
    }
    if (Var::IsDefined(var)) {
        on = var;
    }
}
Var Edit::Update(const Var var, Var on) {
    if(Var::IsDefined(on)) {
        __Update(var, on);
    } else {
        on = var;
    }
    return on;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Find
 * ------------------------------------------------------------------------------------------------
 */
static void __Find(Link& var, const Link& on);
/**
 * ---------------------------------------------------------------------------
 * map on map
 * ---------------------------------------------------------------------------
 */
static inline void __Find(Map& var, const Map& on) {
    for (auto it = var.begin(), end = var.end(); it != end;) {
        auto find = on.find(it->first);
        if (find == on.end()) {
            it = var.erase(it);
            continue;
        }
        __Find(it->second, find->second);
        ++it;
    }      
}
Map& Edit::Find(Map& var, const Map& on) {
    __Find(var, on);
    return var;
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline void __Find(List& var, const List& on) {
    List::size_type i = 0;
    for (; i < var.size() && i < on.size(); ++i) {
        __Find(var[i], on[i]);
    }
    for (; i < var.size(); ++i) {
        var.pop_back();
    }
}
List& Edit::Find(List& var, const List& on) {
    __Find(var, on);
    return var;
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static void __Find(Link& var, const Link& on) {
    try {
        if (var == on) {
            return;
        }
        if (Var::IsUndefined(var)) {
            var = on;
            return;
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {
            __Find(Var::Map(var), Var::Map(on));
            return;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
            __Find(Var::List(var), Var::List(on));
            return;
        }
        if (__MatchContent(var, on)) {
            return;
        }
    } catch (...) {}
    // not found
    var = Obj();
}
Var Edit::Find(Var var, const Var on) {
    if(Var::IsDefined(var)) {
        __Find(var, on);
    }
    return var;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Remove
 * ------------------------------------------------------------------------------------------------
 */
static void __Remove(const Link& var, Link& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline void __Remove(const Map& var, Map& on) {
    for (auto it = var.begin(), end = var.end(); it != end; ++it) {
        auto find = on.find(it->first);
        if (find != on.end()) {
            __Remove(it->second, find->second);
        }    
    }      
}
Map& Edit::Remove(const Map& var, Map& on) {
    __Remove(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline void __Remove(const List& var, List& on) {
    for (size_t i = 0; i < var.size() && i < on.size(); ++i) {
        __Remove(var[i], on[i]);
    }      
}
List& Edit::Remove(const List& var, List& on) {
    __Remove(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static void __Remove(const Link& var, Link& on) {
    try {
        if (var == on) {
            on = Obj();
            return;
        } 
        if (Var::IsUndefined(var)) {
            on = Obj();
            return;
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {
            __Remove(Var::Map(var), Var::Map(on));
            return;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
            __Remove(Var::List(var), Var::List(on));
            return;
        }
        if (__MatchContent(var, on)) {
            on = Obj();
            return;
        }
    } catch (...) {}
}
Var Edit::Remove(const Var var, Var on) {
    if(Var::IsDefined(var)) {
        __Remove(var, on);
    }
    return on;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Delete 
 * ------------------------------------------------------------------------------------------------
 */
static void __Delete(const Link& var, Link& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline void __Delete(const Map& var, Map& on) {
    for (auto it = var.begin(), end = var.end(); it != end; ++it) {
        auto find = on.find(it->first);
        if (find != on.end()) {
            __Delete(it->second, find->second);
            if (Var::IsEmpty(find->second)) {
                on.erase(find);
            }
        }
    }
}
Map& Edit::Delete(const Map& var, Map& on) {
    __Delete(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline void __Delete(const List& var, List& on) {
    auto vit = var.begin();
    auto oit = on.begin();
    for (; (vit != var.end()) && (oit != on.end()); ++vit) {
        __Delete(*vit, *oit);
        // check
        if (Var::IsEmpty(*oit)) {
            oit = on.erase(oit);
        } else {
            ++oit;
        }
    }
}
List& Edit::Delete(const List& var, List& on) {
    __Delete(var, on);
    return on;
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static inline void __Delete(const Link& var, Link& on) {
    try {
        if (var == on) {
            on = Obj();
            return;
        }
        if (Var::IsUndefined(var)) {
            on = Obj();
            return;
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {    
            __Delete(Var::Map(var), Var::Map(on));
            return;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
            __Delete(Var::List(var), Var::List(on));
            return;
        }
        if (__MatchContent(var, on)) {
            on = Obj();
            return;
        }
    } catch (...) {}
}
Var Edit::Delete(const Var var, Var on) {
    if(Var::IsDefined(var)) {
        __Delete(var, on);
    }
    return on; 
}
/**
 * ------------------------------------------------------------------------------------------------
 * Count var on var
 * ------------------------------------------------------------------------------------------------
 */
Integer Edit::Count(const Var& var, const Var& on) {
    if (Var::IsMap(on)) {
        Map& o = Var::Map(on);
        {
            Integer c;
            for (auto& v : o) {
                c += Count(var, v.second);
            }
            return c;
        }
    }
    if (Var::IsList(on)) {
        List& o = Var::List(on);
        {
            Integer c;
            for (auto& v : o) {
                c += Count(var, v);
            }
            return c;
        }
    }
    return Match(var , on) ? 1 : 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Insert key:value
 * ------------------------------------------------------------------------------------------------
 */
static void __Insert(String& key, Var var, Var on){
    if (Var::IsMap(on)) {
        /**
         * add key: obj
         */
        Var::Map(on)[key] = var;
    } else if (Var::IsList(on)) {
        /**
         * to index
         */
        int i = Integer::ValueOf(key);
        if(i == -1){
            List& o = Var::List(on);
            i = o.size();
        }
        /**
         * alloc space
         */
        Var::List(on).resize(i + 1);
        /**
         * link
         */
        Var::List(on)[i] = var;
    }
}
Var Edit::Insert(Key path, Var var, Var on) {
    static const std::regex e("([^/]+)");
    /**
     * path iterator
     */
    auto it  = std::sregex_iterator(path.begin(), path.end(), e);
    auto end = std::sregex_iterator();
    /**
     * path end
     */
    if (it == end) {
        return var;
    }
    /**
     * check variable
     */
    if (Var::IsUndefined(on)) {
        on = Obj(Map());
    }
    auto c = on;
    auto k = String(it->str());
    for (++it; it != end; ++it) {
        auto n = c[k];
        if (Var::IsUndefined(n)) {
            n = Obj(Map());
            __Insert(k, n, c);
        }
        c = n;
        k = it->str();
    }
    __Insert(k, var, c);
    return on;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Update key:value
 * ------------------------------------------------------------------------------------------------
 */
Var Edit::Update(Key path, Var var, Var on) {
    /**
     */
    static const std::regex e("([^/]+)");
    /**
     * path iterator
     */
    auto it  = std::sregex_iterator(path.begin(), path.end(), e);
    auto end = std::sregex_iterator();
    /**
     * process
     */
    if (it == end) {
        return var;
    }
    Key key;
    for (Var o = on; Var::IsDefined(o); o = o[key]) {
        key = move(it->str());
        if (++it == end) {
            if (Var::IsMap(o)) {
                Map& m = Var::Map(o);
                // try to find
                auto find = m.find(key);
                if (find != m.end()) {
                    find->second = var;
                }
                return on;
            }
            if (Var::IsList(o)) {
                auto l = Var::List(o);
                auto k = Integer::ValueOf(key);
                // verify position
                if (l.size() > size_t(k)) {
                    l[k] = var;
                }
                return on;
            }
        }
    }
    return on;
}

Var Edit::Update(Integer deep, Var var, Var on) {
    if (Var::IsMap(on)) {
        if (deep == 0) {
            return var;
        }
        --deep;
        auto& m = Var::Map(on);
        for (auto it = m.begin(); it != m.end(); ++it) {
            it->second = Update(deep, var, it->second);
        }
    } else if (Var::IsList(on)) {
        if (deep == 0) {
            return var;
        }
        --deep;
        auto& l = Var::List(on);
        for (auto it = l.begin(); it != l.end(); ++it) {
            *it = Update(deep, var, *it);
        }
    }
    return on;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Find path
 * ------------------------------------------------------------------------------------------------
 */
Var Edit::Find(Key path, Var on) {
    static const std::regex e("([^/]+)");
    /**
     * process
     */
    auto it  = std::sregex_iterator(path.begin(), path.end(), e);
    auto end = std::sregex_iterator();
    auto res = on;
    for (; Var::IsDefined(res) && (it != end); ++it) {
        res = res[it->str()];
    }
    return res;
}
/**---------------------------------------------------------------------------------------------------------------------
 * Remove
 **--------------------------------------------------------------------------------------------------------------------*/
Var Edit::Remove(Key path, Var on) {
    static const std::regex e("([^/]+)");
    /**
     * process
     */
    auto it  = std::sregex_iterator(path.begin(), path.end(), e);
    auto end = std::sregex_iterator();
    /**
     */
    if (it == end) {
        return on;
    }
    for (Var o = on; Var::IsDefined(o);) {
        auto key = String(std::move(it->str()));
        if (++it == end) {
            if (Var::IsMap(o)) {
                auto& m   = Var::Map(o);
                auto find = m.find(key);
                if (find != m.end()) {
                    auto tmp = find->second;
                    m.erase(find);
                    return tmp;
                }
                return Obj();
            }
            if (Var::IsList(o)) {
                auto& l   = Var::List(o);
                auto find = l.begin() + Integer::ValueOf(key);
                if (find != l.end()) {
                    auto tmp = *find;
                    l.erase(find);
                    return tmp;
                }
                return Obj();
            }
        }
        o = o[key];
    }
    return Obj();
}
/**
 * ------------------------------------------------------------------------------------------------
 * Normalize
 * ------------------------------------------------------------------------------------------------
 */
Var Edit::Normalize(Var var) {
    if (Var::IsMap(var)) {
        auto& m = Var::Map(var);
        {
            std::list<String> keys;
            Map out;
            /**
             * read keys
             */
            for (auto&v : m) {
                keys.emplace_back(v.first);
            }
            /** 
             * sort keys 
             */
            keys.sort();
            /** 
             * reinsert data by sort order
             */
            for (auto k : keys) {
                out[k] = Normalize(m[k]);
            }
            return Obj(out);
        }
    }
    if (Var::IsList(var)) {
        auto& l = Var::List(var);
        {
            List out;
            for (Var v : l) {
                out.emplace_back(Normalize(v));
            }
            return Obj(out);
        }
    }
    return var;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */