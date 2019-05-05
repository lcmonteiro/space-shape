/**
 * ------------------------------------------------------------------------------------------------
 * File:   JsonConverter.h
 * Author: Luis Monteiro
 *
 * Created on May 14, 2015, 12:11 PM
 * -------------------------------------------------------------------------------------------------
 */
#ifndef JSONCONVERT_H
#define JSONCONVERT_H
/**
 * std
 */
#include <iostream>
/**
 * space
 */
#include "SVariable.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Definitions
 * ------------------------------------------------------------------------------------------------
 */
# define __PRECISION__  12
/**
 * tags
 */
#define __LIST_LD__     '['
#define __LIST_RD__     ']'
#define __MAP__LD__     '{'
#define __MAP__RD__     '}'
#define __STR___D__     '\"'
#define __ESC___D__     '\\'
#define __KEY___D__     ':'
#define __ELM___D__     ','
#define __NUM___D__     '0'
#define __1_____D__     '1'
#define __2_____D__     '2'
#define __3_____D__     '3'
#define __4_____D__     '4'
#define __5_____D__     '5'
#define __6_____D__     '6'
#define __7_____D__     '7'
#define __8_____D__     '8'
#define __9_____D__     '9'
#define __NULL__D__     'n'
/**
 */
#define __NULL__S__     "null"
#define __TAB___S__     "    "
/**
 * ------------------------------------------------------------------------------------------------
 * Json Converter
 * ------------------------------------------------------------------------------------------------
 */
typedef class __jsonconvert__ {
    /**
     * ------------------------------------------------------------------------
     * Json ctype
     * ------------------------------------------------------------------------
     */
    class Jsonctype : public std::ctype<char> {
        static mask* make_table() {
            /**
             * static table
             */
            static std::vector<mask> table(
                std::ctype<char>::classic_table(),
                std::ctype<char>::classic_table() + std::ctype<char>::table_size);
            /**
             * update
             */
            table[__LIST_LD__] |= space;
            table[__LIST_RD__] |= space;
            table[__MAP__LD__] |= space;
            table[__MAP__RD__] |= space;
            table[__STR___D__] |= space;
            table[__ESC___D__] |= space;
            table[__KEY___D__] |= space;
            table[' '] &= ~space;
            table['\n'] &= ~space;
            table['\t'] &= ~space;
            return table.data();
        }
    public:
        Jsonctype() : std::ctype<char>(make_table()) {}
    };
public:
    /**
     * ------------------------------------------------------------------------
     *  To Json
     * ------------------------------------------------------------------------
     */    
    static inline std::ostream& ToJson(std::ostream&& os, Var v) {
        return ToJson(os, v);
    }
    static inline std::ostream& ToJson(std::ostream& os, Var v) {
        os << std::setprecision(__PRECISION__);
        if (Var::IsList(v)) {
            os << __LIST_LD__;
            __ToJson(os, Var::List(v));
            os << __LIST_RD__;
        } else if (Var::IsMap(v)) {
            os << __MAP__LD__;
            __ToJson(os, Var::Map(v));
            os << __MAP__RD__;
        } else if (Var::IsString(v)) {
            os << __STR___D__ << Var::String(v) << __STR___D__;
        } else if (Var::IsFloat(v)) {
            os << Var::Float(v);
        } else if (Var::IsInteger(v)) {
            os << Var::Integer(v);
        } else if (Var::IsNull(v)) {
            os << __NULL__S__;
        } else if (Var::IsLink(v)) {
            ToJson(os, Var::Link(v));
        } else {
            os << __STR___D__ << Var::ToString(v) << __STR___D__;
        }
        return os;
    }
    static inline std::ostream& ToJson(std::ostream& os, List l) {
        os << std::setprecision(__PRECISION__);
        os << __LIST_LD__;
        __ToJson(os, l);
        os << __LIST_RD__;
        return os;
    }
    static inline std::ostream& ToJson(std::ostream& os, Map m) {
        os << std::setprecision(__PRECISION__);
        os << __MAP__LD__;
        __ToJson(os, m);
        os << __MAP__RD__;
        return os;
    }
    /**
     * ------------------------------------------------------------------------
     * To Pretty Json
     * ------------------------------------------------------------------------
     */
    static std::ostream& ToPrettyJson(std::ostream& os, Var v, Integer deep = 0) {
        os << std::setprecision(__PRECISION__);
        if (Var::IsList(v)) {
            os << __LIST_LD__ << std::endl;
            __TAB(__ToPrettyJson(os, Var::List(v), deep + Integer(1)) << std::endl, deep);
            os << __LIST_RD__;
        } else if (Var::IsMap(v)) {
            os << __MAP__LD__ << std::endl;
            __TAB(__ToPrettyJson(os, Var::Map(v), deep + Integer(1)) << std::endl, deep);
            os << __MAP__RD__;
        } else if (Var::IsString(v)) {
            os << __STR___D__ << Var::String(v) << __STR___D__;
        } else if (Var::IsFloat(v)) {
            os << Var::Float(v);
        } else if (Var::IsInteger(v)) {
            os << Var::Integer(v);
        } else if (Var::IsNull(v)) {
            os << __NULL__S__;
        } else if (Var::IsLink(v)) {
            ToPrettyJson(os, Var::Link(v), deep);
        } else {
            os << __STR___D__ << Var::ToString(v) << __STR___D__;
        }
        return os;
    }
    static inline std::ostream& ToPrettyJson(std::ostream& os, List data) {
        Integer deep(1);
        os << std::setprecision(__PRECISION__);
        os << __LIST_LD__ << std::endl;
        __ToPrettyJson(os, data, deep) << std::endl;
        os << __LIST_RD__;
        return os;
    }
    static inline std::ostream& ToPrettyJson(std::ostream& os, Map data) {
        Integer deep(1);
        os << std::setprecision(__PRECISION__);
        os << __MAP__LD__ << std::endl;
        __ToPrettyJson(os, data, deep) << std::endl;
        os << __LIST_RD__;
        return os;
    }
    /**
     * ------------------------------------------------------------------------
     *  From Json
     * ------------------------------------------------------------------------
     */
    static inline std::istream& FromJson(std::istream& is, Var& data) {
        data = FromJson(is);
        return is;
    }
    static inline Var FromJson(std::istream&& is) {
        return FromJson(is); 
    }
    static inline Var FromJson(std::istream& is) {
        Var data;
        is.imbue(std::locale(is.getloc(), new Jsonctype()));
        switch (__Get(is)) {
            case __LIST_LD__:
                data = Obj(__FromJson(is, List()));
                break;
            case __MAP__LD__:
                data = Obj(__FromJson(is, Map()));
                break;
            case __STR___D__:
                data = Obj(__FromJson(is, String()));
                break;
            case __NUM___D__:
                data = Obj(__FromJson(is, Float()));
                break;
            case __NULL__D__:
                data = Obj::Null();
                break;
            default:;
        }
        return data;
    }
private:
    /**
     * ------------------------------------------------------------------------
     * To Json
     * ------------------------------------------------------------------------
     */
    static void __ToJson(std::ostream& os, List l) {
        auto it = l.begin();
        if(it != l.end()) {
            ToJson(os, *it);
            for(++it; it != l.end(); ++it){
                ToJson(os << __ELM___D__, *it);    
            }    
        }
    }
    static void __ToJson(std::ostream& os, Map m) {
        auto it = m.begin();
        if(it != m.end()) {
            os << __STR___D__ << it->first << __STR___D__ << __KEY___D__;
            ToJson(os, it->second);
            for(++it; it != m.end(); ++it) {
                os << __ELM___D__;
                os << __STR___D__ << it->first << __STR___D__ << __KEY___D__;
                ToJson(os, it->second);    
            }    
        }
    }
    /**
     * ------------------------------------------------------------------------
     * To Pretty Json
     * ------------------------------------------------------------------------
     */
    static std::ostream& __ToPrettyJson(std::ostream& os, List l, Integer deep) {
        auto it = l.begin();
        if(it != l.end()) {
            ToPrettyJson(__TAB(os, deep), *it);
            for(++it; it != l.end(); ++it){
                ToPrettyJson(__TAB(os, deep) << __ELM___D__, *it);    
            }    
        }
    }
    static std::ostream& __ToPrettyJson(std::ostream& os, Map m, Integer deep) {
        auto it = m.begin();
        if(it != m.end()) {
            ToPrettyJson(
                __TAB(os, deep) << __STR___D__ << it->first << __STR___D__ << __KEY___D__,
                 it->second
            );
            for(++it; it != m.end(); ++it) {
                os << __ELM___D__;
                ToPrettyJson(
                    __TAB(os, deep) << __STR___D__ << it->first << __STR___D__ << __KEY___D__,
                    it->second
                );  
            }    
        }
    }
    /**
     * ------------------------------------------------------------------------
     * From Json
     * ------------------------------------------------------------------------
     */
    static inline List& __FromJson(std::istream& is, List&& l) {
        while (is.good()) {
            switch (__Get(is)) {
                case __LIST_LD__: {
                    l.push_back(Obj(__FromJson(is, List())));
                    break;
                }
                case __MAP__LD__: {
                    l.push_back(Obj(__FromJson(is, Map())));
                    break;
                }
                case __STR___D__: {
                    l.push_back(Obj(__FromJson(is, String())));
                    break;
                }
                case __NUM___D__: {
                    l.push_back(Obj(__FromJson(is, Float())));
                    break;
                }    
                case __NULL__D__: {
                    l.push_back(Obj::Null());
                    break;
                }
                case __LIST_RD__: {
                    return l;
                }
                default:;
            }
        }
        return l;
    }
    static inline Map& __FromJson(std::istream& is, Map&& m) {
        String key;
        while (is.good()) {
            switch (__Get(is)) {
                case __LIST_LD__: {
                    m[std::move(key)] = Obj(__FromJson(is, List()));
                    break;
                }
                case __MAP__LD__: {
                    m[std::move(key)] = Obj(__FromJson(is, Map()));
                    break;
                }
                case __STR___D__: {
                    if (key.empty()) {
                        key = __FromJson(is, String());
                    } else {
                        m[std::move(key)] = Obj(__FromJson(is, String()));
                    }
                    break;
                }
                case __NUM___D__: {
                    if (key.empty()) {
                        key = String::ValueOf(__FromJson(is, Float()));
                    } else {
                        m[std::move(key)] = Obj(__FromJson(is, Float()));
                    }
                    break;
                }
                case __NULL__D__: {
                    if (!key.empty()) {
                        m[std::move(key)] = Obj::Null();
                    }
                    break;
                }
                case __MAP__RD__: {
                    return m;
                }
                default:;
            }
        }
        return m;
    }
    static inline String& __FromJson(std::istream& is, String&& s) {
        /**
         * null string - remove __STR___D__
         */
        if (is.peek() == __STR___D__) {
            is.get();
            return s;
        }
        String tmp;
        while (is.good()) {
            is >> tmp;
            switch (__Get(is)) {
                case __KEY___D__: {
                    tmp.push_back(__KEY___D__);
                    break;
                }
                case __ESC___D__: {
                    /**
                     * process escape character
                     */
                    tmp.push_back(is.get());
                    break;
                }
                case __STR___D__: {
                    /**
                     * finish string
                     */
                    s += move(tmp);
                    return s;
                }
                default:;
            }
            s += move(tmp);
        }
        return s;
    }
    static inline Float& __FromJson(std::istream& is, Float&& f) {
        is >> f.ref();
        return f;
    }
    /**
     * ------------------------------------------------------------------------
     * Utilities
     * ------------------------------------------------------------------------
     */
    static std::ostream& __TAB(std::ostream& os, Integer deep) {
        for (Integer i = 0; i < deep; ++i) {
            os << __TAB___S__;
        }
        return os;
    }
    static inline std::istream::int_type __Get(std::istream& is) {
        while (is.good()) {
            switch (is.peek()) {
                case __KEY___D__:
                case __LIST_LD__:
                case __LIST_RD__:
                case __MAP__LD__:
                case __MAP__RD__:
                case __ESC___D__:
                case __STR___D__: {
                    return is.get();
                }
                case __NUM___D__:
                case __1_____D__:
                case __2_____D__:
                case __3_____D__:
                case __4_____D__:
                case __5_____D__:
                case __6_____D__:
                case __7_____D__:
                case __8_____D__:
                case __9_____D__: {
                    return __NUM___D__;
                }
                case __NULL__D__: {
                    String tmp;
                    is >> tmp;
                    return __NULL__D__;
                }
                default: {
                    is.get();
                    break;
                }
            }
        }
        return is.get();
    }
} JsonConvert;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif  /* JSONCONVERT_H */

