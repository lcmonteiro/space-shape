
/**
 * ------------------------------------------------------------------------------------------------
 * File:   Edit.cc
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#include <regex>
#include <iostream>
#include <list>
/**
 */
#include "SSearch.h"
#include "Edit.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Insert 
 * ------------------------------------------------------------------------------------------------
 **/
static Var __Insert(Var var, Var on);
/**
 * map on map
 */
static inline Map& __Insert(const Map& var, Map& on) {
    for (auto& it : var) {
        if (!on.count(it.first)) {
            on[it.first] = it.second;
        } else {
            on[it.first] = __Insert(it.second, on[it.first]);
        }
    }
    return on;      
}
Map& Edit::Insert(const Map& var, Map& on) {
    return __insert(var, on);
}
/**
 * list on list
 */
static inline  List& __Insert(const List& var, List& on) {
    for (List::size_type i = 0; i < var.size(); ++i) {
        if (on.size() <= i) {
            on.push_back(var[i]);
        } else {
            on[i] = __Insert(var[i], on[i]);
        }
    }
    return on;      
}
List& Edit::Insert(const List& var, List& on) {
    return __Insert(var, on);
}
/**
 * var on var
 */
static Var& __Insert(const Var& var, Var& on) {
    if (var == on) {
        return var;
    }
    if (Var::IsMap(var) && Var::IsMap(on)) {
        __Insert(Var::Map(var), Var::Map(on));
        return on;
    }
    if (Var::IsList(var) && Var::IsList(on)) {
        __Insert(Var::List(var), Var::List(on));
        return on;
    }
    if (Var::IsDefined(var)) {
        if(Var::IsList(on)) {
            Edit::Insert("-1", var, on);
        }
        return var;
    }
    return on;
}
Var& Edit::Insert(const Var& var, Var& on) {
    return Var::IsUndefined(on) ? var: __Insert(var, on);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Update
 * ------------------------------------------------------------------------------------------------
 */
static Var& __Update(const Var& var, Var& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline Map& __Update(const Map& var, Map& on) {
    for (auto it = var.begin(), end = var.end(); it != end; ++it) {
        auto find = on.find(it->first);
        if (find != on.end()) {
            find->second = __Update(it->second, find->second);
        }    
    }
    return on;      
}
Map& Edit::Update(const Map& var, Map& on) {
    return __update(var, on);
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline  List& __Update(const List& var, List& on) {
    for (List::size_type i = 0; i < var.size() && i < on.size(); ++i) {
        on[i] = __Update(var[i], on[i]);
    }
    return on;      
}
List& Edit::Update(const List& var, List& on) {
    return __Update(var, on);
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static Var& __Update(const Var& var, Var& on) {
    if (var == on) {
        return var;
    }
    if (Var::IsMap(var) && Var::IsMap(on)) {    
        __Update(Var::Map(var), Var::Map(on));
        return on;
    }
    if (Var::IsList(var) && Var::IsList(on)) {
        __Update(Var::List(var), Var::List(on));
        return on;
    }
    if (Var::IsDefined(var)) {
        return var;
    }
    return on;
}
Var Edit::Update(Var var, Var on) {
    return Var::IsUndefined(on) ? var : __Update(var, on);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Find
 * ------------------------------------------------------------------------------------------------
 */
static Var& __Find(Var& var, const Var& on);
/**
 * ---------------------------------------------------------------------------
 * map on map
 * ---------------------------------------------------------------------------
 */
static inline Map& __Find(Map& var, const Map& on) {
    for (auto it = var.begin(), end = var.end(); it != end;) {
        auto find = on.find(it->first);
        if (find == on.end()) {
            it = var.erase(it);
            continue;
        }
        it->second = __find(it->second, find->second);
        ++it;
    }
    return var;      
}
Map& Edit::Find(Map& var, const Map& on) {
    return __find(var, on);
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline  List& __Find(List& var, const List& on) {
    List::size_type i = 0;
    for (; i < var.size() && i < on.size(); ++i) {
        var[i] = __find(var[i], on[i]);
    }
    for (; i < var.size(); ++i) {
        var.pop_back();
    }
    return var;
}
List& Edit::Find(List& var, const List& on) {
    return __find(var, on);
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static Var& __Find(Var& var, const Var& on) {
    try {
        if (var == on) {
            return var;
        }
        if (Var::IsUndefined(var)) {
            return (var = on);
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {
            __Find(Var::Map(var), Var::Map(on));
            return var;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
            __Find(Var::List(var), Var::List(on));
            return var;
        }
        if (String(var) == String(on)) {
            return var;
        }
    } catch (...) {
    }
    return Obj::Null();
}
Var Edit::Find(Var var, const Var& on) {
    return Var::IsNull(var) ? var : __Find(var, on);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Remove
 * ------------------------------------------------------------------------------------------------
 */
static Var& __Remove(const Var& var, const Var& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline Map& __Remove(const Map& var, Map& on) {
    for (auto it = var.begin(), end = var.end(); it != end; ++it) {
        auto find = on.find(it->first);
        if (find != on.end()) {
            find->second = __Remove(it->second, find->second);
        }    
    }
    return on;      
}
Map& Edit::Remove(const Map& var, Map& on) {
    return __Remove(var, on);
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline List& __Remove(const List& var, List& on) {
    for (size_t i = 0; i < var.size() && i < on.size(); ++i) {
        on[i] = __Remove(var[i], on[i]);
    }
    return on;      
}
List& Edit::Remove(const List& var, List& on) {
    return __Remove(var, on);
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static Var& __Remove(const Var& var, const Var& on) {
    try {
        if (var == on) {
            return Obj::Null();
        }
        if (Var::IsUndefined(var)) {
            return Obj::Null();
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {
            __Remove(Var::Map(var), Var::Map(on));
            return on;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
            __Remove(Var::List(var), Var::List(on));
            return on;
        }
        if (String(var) == String(on)) {
            return Obj::Null();
        }
    } catch (...) {
    }
    return on;
}
Var Edit::Remove(Var var, Var on) {
    return Var::IsNull(var) ? on : __Remove(var, on);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Delete 
 * ------------------------------------------------------------------------------------------------
 */
static Var& __Delete(const Var& var, const Var& on);
/**
 * ----------------------------------------------------------------------------
 * map on map
 * ----------------------------------------------------------------------------
 */
static inline Map& __Delete(const Map& v, Map& o) {
    for (auto it = v.begin(), end = v.end(); it != end; ++it) {
        auto find = o.find(it->first);
        if (find != o.end()) {
            find->second = __Delete(it->second, find->second);
            if (Var::IsEmpty(find->second)) {
                o.erase(find);
            }
        }
    }
    return o;
}
Map& Edit::Delete(const Map& v, Map& o) {
    return __Delete(v, o);
}
/**
 * ----------------------------------------------------------------------------
 * list on list
 * ----------------------------------------------------------------------------
 */
static inline List& __delete(const List& v, List& o) {
    auto vit = v.begin();
    auto oit = o.begin();
    for (; (vit != v.end()) && (oit != o.end()); ++vit) {
        *oit = __Delete(*vit, *oit);
        // check
        if (Var::IsEmpty(*oit)) {
            oit = o.erase(oit);
        } else {
            ++oit;
        }
    }
    return o;
}
List& Edit::Delete(const List& v, List& o) {
    return __delete(v, o);
}
/**
 * ----------------------------------------------------------------------------
 * var on var
 * ----------------------------------------------------------------------------
 */
static Var& __Delete(const Var& var, const Var& on) {
    try {
        if (var == on) {
            return Obj::Null();
        }
        if (Var::IsUndefined(var)) {
            return Obj::Null();
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {    
            __delete(Var::Map(var), Var::Map(on));
            return on;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
            __delete(Var::List(var), Var::List(on));
            return on;
        }
        if (String(var) == String(on)) {
            return Obj::Null();
        }
    } catch (...) {
    }
    return on;
}
Var& Edit::Delete(const Var& var, Var& on) {
    return Var::IsNull(var) ? on : __Delete(var, on);
}
/***
 * ------------------------------------------------------------------------------------------------
 * Match
 * ------------------------------------------------------------------------------------------------
 */
static Boolean __Match(Var var, Var on);
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
        if (!__match(*vit, *oit)){
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
static Boolean __Match(const Var& var, const Var& on) {
    try {
        if (var == on) {
            return true;
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {    
            return __match(Var::Map(var), Var::Map(on));
        }
        if (Var::IsList(var) && Var::IsList(on)) {      
            return __match(Var::List(var), Var::List(on));;
        }
        return String(var) == String(on);
    } catch (...) {
    }
    return false;
}
Boolean Edit::Match(const Var& var, const Var& on) {
    return __Match(var, on);
}
/**
 * ------------------------------------------------------------------------------------------------
 * Count var on var
 * ------------------------------------------------------------------------------------------------
 */
Integer Edit::Count(String var, Var on) {
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
    return (var == String(on)) ? 1 : 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Insert key:value
 * ------------------------------------------------------------------------------------------------
 */
static void __Insert (String& key, Var var, Var on){
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
    sregex_iterator it(path.begin(), path.end(), e), end;
    /**
     * path end
     */
    if (it == end) {
        return var;
    }
    /**
     * check variable
     */
    if (Var::IsNull(on)) {
        on = Obj::Map();
    }
    for (Var c = on;;) {
        String k = it->str();
        if (++it == end) {
            __Insert(k, var, c);
            break;
        }
        Var n = c[k];
        if (Var::IsNull(n)) {
            n = Obj::Map();
            __Insert(k, n, c);
        }
        c = n;
    }
    return on;
}
/**---------------------------------------------------------------------------------------------------------------------
 * Update key:value
 **--------------------------------------------------------------------------------------------------------------------*/
Var Edit::Update(Key path, Var var, Var on) {
    /**
     */
    static const regex e("([^/]+)");
    /**
     */
    sregex_iterator it(path.begin(), path.end(), e), end;
    /**
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
                List& l = Var::List(o);
                Integer k = Integer::ValueOf(key);
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
        Map& m = Var::Map(on);
        for (Map::iterator it = m.begin(); it != m.end(); ++it) {
            /**/
            it->second = Update(deep, var, it->second);
        }
    } else if (Var::IsList(on)) {
        if (deep == 0) {
            return var;
        }
        --deep;
        List& l = Var::List(on);
        for (List::iterator it = l.begin(); it != l.end(); ++it) {
            /**/
            *it = Update(deep, var, *it);
        }
    }
    return on;
}
/**---------------------------------------------------------------------------------------------------------------------
 * Find path
 **--------------------------------------------------------------------------------------------------------------------*/
Var Edit::Find(Key path, Var on) {
    /**
     */
    static const std::regex e("([^/]+)");
    /**
     */
    Var r = on;
    for (sregex_iterator it(path.begin(), path.end(), e), end; (!Var::IsNull(r)) && (it != end); ++it) {
        r = r[it->str()];
    }
    return r;
}
/**---------------------------------------------------------------------------------------------------------------------
 * Remove
 **--------------------------------------------------------------------------------------------------------------------*/
Var Edit::Remove(Key path, Var on) {
    /**
     */
    static const std::regex e("([^/]+)");
    /**
     */
    sregex_iterator it(path.begin(), path.end(), e), end;
    /**
     */
    if (it == end) {
        return on;
    }
    for (Var o = on; Var::IsDefined(o);) {
        /**/
        String key = move(it->str());
        /**/
        if (++it == end) {
            if (Var::IsMap(o)) {
                Map& m = Var::Map(o);
                // try to find
                auto find = m.find(key);
                if (find != m.end()) {
                    auto tmp = find->second;
                    /**/
                    m.erase(find);
                    /**/
                    return tmp;
                }
                return Obj::Null();
            }
            if (Var::IsList(o)) {
                List& l = Var::List(o);
                // try to find
                auto find = l.begin() + Integer::ValueOf(key);
                if (find != l.end()) {
                    auto tmp = *find;
                    //
                    l.erase(find);
                    //
                    return tmp;
                }
                return Obj::Null();
            }
        }
        /**/
        o = o[key];
    }
    return Obj(nullptr);
}
/**---------------------------------------------------------------------------------------------------------------------
 * Normalize
 **--------------------------------------------------------------------------------------------------------------------*/
Var Edit::Normalize(Var var) {
    if (Var::IsMap(var)) {
        Map& m = Var::Map(var);
        {
            list<String> keys;
            Map out;
            /**
             * read keys
             */
            for (auto&v : m) {
                keys.push_back(v.first);
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
        List& l = Var::List(var);
        {
            List out;
            for (Var v : l) {
                out.push_back(Normalize(v));
            }
            return Obj(out);
        }
    }
    return var;
}