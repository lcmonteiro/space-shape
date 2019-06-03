/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   SConvert.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */

#ifndef SCONVERT_H
#define SCONVERT_H
/**
 * std
 */
#include <cmath>
#include <regex>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <functional>
/**
 * space
 */
#include "SConvertJSON.h"
#include "SVariable.h"
#include "SEdit.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Convert - Space
 * ------------------------------------------------------------------------------------------------
 */
namespace Convert {
    /**
     * ------------------------------------------------------------------------
     * convert String (format a/b/c to [a,b,c]) to list
     * ------------------------------------------------------------------------
     */
    static inline List FromString(String p, String pattern) {
        using filter_iterator = std::regex_iterator<std::string::iterator>;
        // implementation
        const auto reg  = std::regex(pattern);
        auto list = List();
        for (filter_iterator it(p.begin(), p.end(), reg), end; (it != end); ++it) {
            list.push_back(Obj(it->str()));
        }
        return list;
    }
    /**
     * utilities
     */
    static inline List FromPath(String p) {
        return FromString(p, "([^/]+)"); 
    }
    /**
     * ------------------------------------------------------------------------
     * convert list (format a/b/c to [a,b,c]) to string
     * ------------------------------------------------------------------------
     */
    static inline String ToString(List l, String delim) {
        auto os = std::ostringstream();  
        auto it  = l.begin();
        auto end = l.end();
        if (it != end) {
            os << Var::ToString(*it);
            for (++it; it != end; ++it) {
                os << delim << Var::ToString(*it);
            }
        }
        return os.str();
    }
    static inline String ToString(KeyList l, String delim) {
        auto os = std::ostringstream();  
        auto it  = l.begin();
        auto end = l.end();
        if (it != end) {
            os << *it;
            for (++it; it != end; ++it) {
                os << delim << *it;
            }
        }
        return os.str();
    }
    /**
     * utilities
     */
    static inline String ToPath(List l) {
       return ToString(l, "/");         
    }
    static inline String ToPath(KeyList l) {
       return ToString(l, "/");         
    }
    /**
     * ------------------------------------------------------------------------
     * Simple Map
     * ------------------------------------------------------------------------
     */
    namespace {
        static void __ToSimpleMap(String k, Map& b, String& d, List& l);
        static void __ToSimpleMap(String k, Map& b, String& d, Map&  m);
        void __ToSimpleMap(String k, Map& b, String& d, Map& m) {
            for (auto& v : m) {
                if (Var::IsMap(v.second)) {
                    __ToSimpleMap(k + v.first + d, b, d, Var::Map(v.second));
                } else if (Var::IsList(v.second)) {
                    __ToSimpleMap(k + v.first + d, b, d, Var::List(v.second));
                } else {
                    b[k + v.first] = v.second;
                }
            }
        }
        void __ToSimpleMap(String k, Map& b, String& d, List& l) {
            for (Integer i(0); size_t(i) < l.size(); ++i) {
                Var v = l[i];
                if (Var::IsMap(v)) {
                    __ToSimpleMap(k + String::ValueOf(i) + d, b, d, Var::Map(v));
                } else if (Var::IsList(v)) {
                    __ToSimpleMap(k + String::ValueOf(i) + d, b, d, Var::List(v));
                } else {
                    b[k + String::ValueOf(i)] = v;
                }
            }
        }
    }
    static inline Map ToSimpleMap(Var var, String delim) {
        Map base;
        if (Var::IsMap(var)) {
            __ToSimpleMap("", base, delim, Var::Map(var));
        } else if (Var::IsList(var)) {
            __ToSimpleMap("", base, delim, Var::List(var));
        }
        return base;
    }
    /**
     * ------------------------------------------------------------------------
     * To Map
     * ------------------------------------------------------------------------
     */
    namespace {
        static void __ToMap(String k, Map& b, String& d, Map&  m);
        static void __ToMap(String k, Map& b, String& d, List& l);
        void __ToMap(String k, Map& b, String& d, Map& m) {
            for (auto& v : m) {
                if (Var::IsMap(v.second)) {
                    __ToMap(k + v.first + d, b, d, Var::Map(v.second));
                } else if (Var::IsList(v.second)) {
                    __ToMap(k + v.first + d, b, d, Var::List(v.second));
                }
                b[k + v.first] = v.second;
            }
        }
        void __ToMap(String k, Map& b, String& d, List& l) {
            for (Integer i(0); size_t(i) < l.size(); ++i) {
                Var v = l[i];
                if (Var::IsMap(v)) {
                    __ToMap(k + String::ValueOf(i) + d, b, d, Var::Map(v));
                } else if (Var::IsList(v)) {
                    __ToMap(k + String::ValueOf(i) + d, b, d, Var::List(v));
                }
                b[k + String::ValueOf(i)] = v;
            }
        }
    }
    static inline Map ToMap(Var var, String delim) {
        Map base;
        if (Var::IsMap(var)) {
            __ToMap("", base, delim, Var::Map(var));
        } else if (Var::IsList(var)) {
            __ToMap("", base, delim, Var::List(var));
        }
        return base;
    }
    /**
     * ------------------------------------------------------------------------
     * Convert Var to Number
     * ------------------------------------------------------------------------
     */
    static inline Integer ToHash(Var v) {
        std::ostringstream os;
        std::hash<std::string> h;
        /**
         * serialize and execute hash function
         */
        Convert::ToJson(os, v);
        return h(os.str());
    }

    // /**
    //  * convert Var to MD5 hash value
    //  */
    // static Buffer toMD5(const String& s) {
    //     /**
    //      * execute md5 hash function
    //      */
    //     Buffer digest(MD5_DIGEST_LENGTH, 0);
    //     MD5((unsigned char*) s.c_str(), s.size(), digest.data());
    //     return digest;
    // }

    // static Buffer toMD5(Var v) {
    //     /**
    //      * serialize
    //      */
    //     std::ostringstream os;
    //     Serialize::Json(os, Edit::Normalize(v));
    //     return toMD5(os.str());
    // }

    // /**
    //  * convert list to MD5 hash value
    //  */
    // static Buffer toMD5(List& l) {
    //     Buffer out(MD5_DIGEST_LENGTH, 0);
    //     for (Var v : l) {
    //         out ^= toMD5(v);
    //     }
    //     return out;
    // }
    /**
     * ------------------------------------------------------------------------
     * convert Var to Hex format
     * ------------------------------------------------------------------------
     */
    static inline Var ToHex(Integer v) {        
        auto result = std::string();
        auto stream = std::stringstream();
        auto number = int(log2(int(v)));
        auto offcut = number >> 3;
        while (offcut > 0) {
            offcut = ++number & 0x0F;
        }
        stream << std::setfill('0') 
            << std::setw(number >> 2) << std::hex << v;
        stream >> result;
        return Obj(result);
    }
}
/**
 * -------------------------------------------------------------------------------------------------------------------- 
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SCONVERT_H */

