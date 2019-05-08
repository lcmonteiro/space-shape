
/*
 * File:   SEdit.cc
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 */
#include <regex>
#include <iostream>
#include <list>
/**
 */
#include "SSearch.h"
#include "SEdit.h"
/**---------------------------------------------------------------------------------------------------------------------
 * Insert
 **--------------------------------------------------------------------------------------------------------------------*/
static Var __insert(Var var, Var on);
/**
 * map on map
 */
static inline Map& __insert(const Map& var, Map& on) {
        for (auto& it : var) {
                if (!on.count(it.first)) {
                        on[it.first] = it.second;
                } else {
                        on[it.first] = __insert(it.second, on[it.first]);
                }
        }
        return on;          
}
Map& Sedit::Insert(const Map& var, Map& on) {
        return __insert(var, on);
}
/**
 * list on list
 */
static inline  List& __insert(const List& var, List& on) {
        for (List::size_type i = 0; i < var.size(); ++i) {
                if (on.size() <= i) {
                        on.push_back(var[i]);
                } else {
                        on[i] = __insert(var[i], on[i]);
                }
        }
        return on;          
}
List& Sedit::Insert(const List& var, List& on) {
        return __insert(var, on);
}
/**
 * var on var
 */
static Var __insert(Var var, Var on) {
        /**
         */
        if (var == on) {
                return var;
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {
                __insert(Var::Map(var), Var::Map(on));
                return on;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
                __insert(Var::List(var), Var::List(on));
                return on;
        }
        if (Var::IsDefined(var)) {
                if(Var::IsList(on)){
                        Edit::Insert("-1", var, on);
                }
                return var;
        }
        return on;
}
Var Sedit::Insert(Var var, Var on) {
        return Var::IsUndefined(on) ? var: __insert(var, on);
}
/**---------------------------------------------------------------------------------------------------------------------
 * Update
 **--------------------------------------------------------------------------------------------------------------------*/
static Var __update(Var var, Var on);
/**
 * map on map
 */
static inline Map& __update(const Map& var, Map& on) {
        for (auto it = var.begin(), end = var.end(); it != end; ++it) {
                auto find = on.find(it->first);
                if (find != on.end()) {
                        find->second = __update(it->second, find->second);
                }        
        }
        return on;          
}
Map& Sedit::Update(const Map& var, Map& on) {
        return __update(var, on);
}
/**
 * list on list
 */
static inline  List& __update(const List& var, List& on) {
        for (List::size_type i = 0; i < var.size() && i < on.size(); ++i) {
                on[i] = __update(var[i], on[i]);
        }
        return on;          
}
List& Sedit::Update(const List& var, List& on) {
        return __update(var, on);
}
/**
 * var on var
 */
static Var __update(Var var, Var on) {
        /**
         */
        if (var == on) {
                return var;
        }
        if (Var::IsMap(var) && Var::IsMap(on)) {        
                __update(Var::Map(var), Var::Map(on));
                return on;
        }
        if (Var::IsList(var) && Var::IsList(on)) {
                __update(Var::List(var), Var::List(on));
                return on;
        }
        if (Var::IsDefined(var)) {
                return var;
        }
        return on;
}
Var Sedit::Update(Var var, Var on) {
        return Var::IsUndefined(on) ? var : __update(var, on);
}
/**---------------------------------------------------------------------------------------------------------------------
 * Find
 **--------------------------------------------------------------------------------------------------------------------*/
static Var __find(Var var, Var on);
/**
 * map on map
 */
static inline Map& __find(Map& var, const Map& on) {
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
Map& Sedit::Find(Map& var, const Map& on) {
        return __find(var, on);
}
/**
 * list on list
 */
static inline  List& __find(List& var, const List& on) {
        List::size_type i = 0;
        for (; i < var.size() && i < on.size(); ++i) {
                var[i] = __find(var[i], on[i]);
        }
        for (; i < var.size(); ++i) {
                var.pop_back();
        }
        return var;
}
List& Sedit::Find(List& var, const List& on) {
        return __find(var, on);
}
/**
 * var on var
 */
static Var __find(Var var, Var on) {
        try {
                /**
                 */
                if (var == on) {
                        return on;
                }
                if (Var::IsUndefined(var)) {
                        return on;
                }
                if (Var::IsMap(var) && Var::IsMap(on)) {
                        __find(Var::Map(var), Var::Map(on));
                        return var;
                }
                if (Var::IsList(var) && Var::IsList(on)) {
                        __find(Var::List(var), Var::List(on));
                        return var;
                }
                if (String(var) == String(on)) {
                        return on;
                }
        } catch (...) {
        }
        return Obj::Null();
}
Var Sedit::Find(Var var, Var on) {
        return Var::IsNull(var) ? var : __find(var, on);
}
/**---------------------------------------------------------------------------------------------------------------------
 * Remove
 **--------------------------------------------------------------------------------------------------------------------*/
static Var __remove(const Var& var, const Var& on);
/**
 * map on map
 */
static inline Map& __remove(const Map& var, Map& on) {
        for (auto it = var.begin(), end = var.end(); it != end; ++it) {
                auto find = on.find(it->first);
                if (find != on.end()) {
                        find->second = __remove(it->second, find->second);
                }        
        }
        return on;          
}
Map& Sedit::Remove(const Map& var, Map& on) {
        return __remove(var, on);
}
/**
 * list on list
 */
static inline  List& __remove(const List& var, List& on) {
        for (List::size_type i = 0; i < var.size() && i < on.size(); ++i) {
                on[i] = __remove(var[i], on[i]);
        }
        return on;          
}
List& Sedit::Remove(const List& var, List& on) {
        return __remove(var, on);
}
/**
 * var on var
 */
static Var __remove(const Var& var, const Var& on) {
        try {
                /**
                 */
                if (var == on) {
                        return Obj::Null();
                }
                if (Var::IsUndefined(var)) {
                        return Obj::Null();
                }
                if (Var::IsMap(var) && Var::IsMap(on)) {
                        __remove(Var::Map(var), Var::Map(on));
                        return on;
                }
                if (Var::IsList(var) && Var::IsList(on)) {
                        __remove(Var::List(var), Var::List(on));
                        return on;
                }
                if (String(var) == String(on)) {
                        return Obj::Null();
                }
        } catch (...) {
        }
        return on;
}
Var Sedit::Remove(Var var, Var on) {
        return Var::IsNull(var) ? on : __remove(var, on);
}
/**---------------------------------------------------------------------------------------------------------------------
 * Delete 
 **--------------------------------------------------------------------------------------------------------------------*/
static Var __delete(const Var& var, const Var& on);
/**
 * map on map
 */
static inline Map& __delete(const Map& v, Map& o) {
        for (auto it = v.begin(), end = v.end(); it != end; ++it) {
                auto find = o.find(it->first);
                if (find != o.end()) {
                        find->second = __delete(it->second, find->second);
                        if (Var::IsEmpty(find->second)) {
                                o.erase(find);
                        }
                }
        }
        return o;
}
Map& Sedit::Delete(const Map& v, Map& o) {
        return __delete(v, o);
}
/**
 * list on list
 */
static inline List& __delete(const List& v, List& o) {
        auto vit = v.begin();
        auto oit = o.begin();
        for (; (vit != v.end()) && (oit != o.end()); ++vit) {
                *oit = __delete(*vit, *oit);
                // check
                if (Var::IsEmpty(*oit)) {
                        oit = o.erase(oit);
                } else {
                        ++oit;
                }
        }
        return o;
}
List& Sedit::Delete(const List& v, List& o) {
        return __delete(v, o);
}
/**
 * var on var
 */
static Var __delete(const Var& var, const Var& on) {
        try {
                /**
                 */
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
Var Sedit::Delete(Var var, Var on) {
        return Var::IsNull(var) ? on : __delete(var, on);
}
/**---------------------------------------------------------------------------------------------------------------------
 * match
 **--------------------------------------------------------------------------------------------------------------------*/
static Boolean __match(Var var, Var on);
/**
 * map on map
 */
static inline Boolean __match(const Map& v, const Map& o) {
        if (v.size() != o.size()) {
                return false;
        }
        for (auto it = v.begin(), end = v.end(); it != end; ++it) {
                auto find = o.find(it->first);
                //
                if (find == o.end()) {
                        return false;
                }
                if (!__match(it->second, find->second)){
                        return false;
                }
        }
        return true;
}
Boolean Sedit::Match(const Map& v, const Map& o) {
        return __match(v, o);
}
/**
 * list on list
 */
static inline Boolean __match(const List& v, const List& o) {
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
Boolean Sedit::Match(const List& v, const List& o) {
        return __match(v, o);
}
/**
 * var on var
 */
static Boolean __match(Var var, Var on) {
        try {
                /**
                 */
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
Boolean Sedit::Match(Var var, Var on) {
        return __match(var, on);
}
/**---------------------------------------------------------------------------------------------------------------------
 * count var on var
 **--------------------------------------------------------------------------------------------------------------------*/
Integer Sedit::Count(String var, Var on) {
        if (Var::IsMap(on)) {
                Map& o = Var::Map(on);
                /**/
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
                /**/
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
/**---------------------------------------------------------------------------------------------------------------------
 * Insert key:value
 **--------------------------------------------------------------------------------------------------------------------*/
static void __Insert (String& key, Var var, Var on){
        if (Var::IsMap(on)) {
                /**
                 * add key: obj
                 */
                Var::Map(on)[key] = var;
                /**/
        } else if (Var::IsList(on)) {
                /**
                 * to index
                 */
                int i = Integer::ValueOf(key);
                /**
                 */
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
/**
 */
Var Sedit::Insert(Key path, Var var, Var on) {
        /**
         */
        static const std::regex e("([^/]+)");
        /**
         */
        sregex_iterator it(path.begin(), path.end(), e), end;
        /**
         */
        if (it == end) {
                return var;
        }
        /**
         * check variable
         */
        if (Var::IsNull(on)){
                on = Obj::Map();
        }
        /**/
        for (Var c = on;;) {
                /**/
                String k = move(it->str());
                /**/
                if (++it == end) {
                        __Insert(k, var, c);
                        break;
                }
                /**/
                Var n = c[k];
                /**/
                if (Var::IsNull(n)) {
                        /**/
                        n = Obj::Map();
                        /**/
                        __Insert(k, n, c);
                }
                /**/
                c = n;
        }
        return on;
}
/**---------------------------------------------------------------------------------------------------------------------
 * Update key:value
 **--------------------------------------------------------------------------------------------------------------------*/
Var Sedit::Update(Key path, Var var, Var on) {
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

Var Sedit::Update(Integer deep, Var var, Var on) {
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
Var Sedit::Find(Key path, Var on) {
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
Var Sedit::Remove(Key path, Var on) {
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
Var Sedit::Normalize(Var var) {
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