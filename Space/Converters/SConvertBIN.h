/**
 * ------------------------------------------------------------------------------------------------
 * File:   SBinConvert.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SBINCONTERT_H
#define SBINCONTERT_H
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
 * Tags
 */
#define __NULL__T__     0x00
#define __LIST__T__     0x01
#define __MAP___T__     0x02
#define __STR___T__     0x0A
#define __INT___T__     0x14
#define __FLOAT_T__     0x18
#define __BUF___T__     0x09
#define __BOOL__T__     0x03
#define __LINK__T__     0xFF
/**
 * ------------------------------------------------------------------------------------------------
 * Bin Converter
 * ------------------------------------------------------------------------------------------------
 */
typedef class __binconverter__ {
public:
    /**
     * ------------------------------------------------------------------------
     * To Bin
     * ------------------------------------------------------------------------
     */
    static inline std::ostream& ToBin(std::ostream&& os, Var v) {
        return ToBin(os, v);
    }
    static inline std::ostream& ToBin(std::ostream& os, Var v) {
        if (Var::IsList(v)) {
            __ToBin(os, Var::List(v));
        } else if (Var::IsMap(v)) {
            __ToBin(os, Var::Map(v));
        } else if (Var::IsString(v)) {
            __ToBin(os, Var::String(v));
        } else if (Var::IsInteger(v)) {
            __ToBin(os, Var::Integer(v));
        } else if (Var::IsFloat(v)) {
            __ToBin(os, Var::Float(v));
        } else if (Var::IsBoolean(v)) {
            __ToBin(os, Var::Boolean(v));
        } else if (Var::IsBuffer(v)) {
            __ToBin(os, Var::Buffer(v));
        } else if (Var::IsLink(v)) {
            __ToBin(os, Var::Link(v));
        } else {
            os.put(__NULL__T__);
        }
        return os;
    }
    /**
     * ------------------------------------------------------------------------
     * From Bin
     * ------------------------------------------------------------------------
     */
    static inline std::istream& FromBin(std::istream& is, Var& data) {
        data = FromBin(is);
        return is;
    }
    static inline Var FromBin(std::istream&& is) {
        return FromBin(is);
    }
    static Var FromBin(std::istream& is) {
        Var data;
        switch (is.get()) {
            case __LIST__T__:
                data = Obj(__FromBin(is, List()));
                break;
            case __MAP___T__:
                data = Obj(__FromBin(is, Map()));
                break;
            case __STR___T__:
                data = Obj(__FromBin(is, String()));
                break;
            case __INT___T__:
                data = Obj(__FromBin(is, Integer()));
                break;
            case __FLOAT_T__:
                data = Obj(__FromBin(is, Float()));
                break;
            case __BOOL__T__:
                data = Obj(__FromBin(is, Boolean()));
                break;
            case __BUF___T__:
                data = Obj(__FromBin(is, Buffer()));
                break;
            case __LINK__T__:
                data = Obj::Link(FromBin(is));
                break;
            case __NULL__T__:
                data = Obj::Null();
                break;
            default:;
        }
        return data;
    }
private:
    /**
     * ------------------------------------------------------------------------
     * To Bin
     * ------------------------------------------------------------------------
     */
    static void __ToBin(std::ostream& os, List l) {
        __Write(os.put(__LIST__T__), Integer(l.size()));
        for (auto& v : l) {
            ToBin(os, v);
        }
    }
    static void __ToBin(std::ostream& os, Map l) {
        __Write(os.put(__MAP___T__), Integer(l.size()));
        for (auto& v : l) {
            ToBin(__Write(__Write(os, Integer(v.first.size())), v.first), v.second);
        }
    }
    static inline std::ostream& __ToBin(std::ostream& os, String s) {
        return __Write(__Write(os.put(__STR___T__), Integer(s.size())), s);
    }
    static inline std::ostream& __ToBin(std::ostream& os, Buffer b) {
        return __Write(__Write(os.put(__BUF___T__), Integer(b.size())), b);
    }
    static inline std::ostream& __ToBin(std::ostream& os, Integer i) {
        return __Write(os.put(__INT___T__), i);
    }
    static inline std::ostream& __ToBin(std::ostream& os, Float f) {
        return __Write(os.put(__FLOAT_T__), f);
    }
    static inline std::ostream& __ToBin(std::ostream& os, Boolean b) {
        return __Write(os.put(__BOOL__T__), b);
    }
    static inline std::ostream& __ToBin(std::ostream& os, Link l) {
        return ToBin(os.put(__LINK__T__), l);
    }
    /**
     * ------------------------------------------------------------------------
     * From Bin
     * ------------------------------------------------------------------------
     */
    static inline List& __FromBin(std::istream& is, List&& l) {
        Integer len(0);
        /**
         * read len
         */
        __Read(is, len);
        /**
         * read elements
         */
        l.reserve(len);
        for (int i = 0; i < len; i++) {
            l.push_back(FromBin(is));
        }
        return l;
    }
    static inline Map& __FromBin(std::istream& is, Map&& m) {
        Integer len(0);
        /**
         * read lenght 
         */
        __Read(is, len);
        /**
         * read elements
         */
        m.reserve(len);
        for (int i = 0; i < len; i++) {
            String key = __FromBin(is, String());
            m[std::move(key)] = FromBin(is);
        }
        return m;
    }
    static inline String& __FromBin(std::istream& is, String&& s) {
        Integer len(0);
        /**
         * read lenght 
         */
        __Read(is, len);
        s.resize(len, 0x00);
        /**
         * read element 
         */
        __Read(is, s);
        return s;
    }
    static inline Buffer& __FromBin(std::istream& is, Buffer&& b) {
        Integer len(0);
        /**
         * read lenght 
         */
        __Read(is, len);
        b.resize(len, 0x00);
        /**
         * read element 
         */
        __Read(is, b);
        return b;
    }
    static inline Integer& __FromBin(std::istream& is, Integer&& i) {
        __Read(is, i);
        return i;
    }
    static inline Float& __FromBin(std::istream& is, Float&& f) {
        __Read(is, f);
        return f;
    }
    static inline Boolean& __FromBin(std::istream& is, Boolean&& b) {
        __Read(is, b);
        return b;
    }
    /**
     * ------------------------------------------------------------------------
     * utils
     * ------------------------------------------------------------------------
     */
     template<class T>
    static inline std::ostream& __Write(std::ostream& os, T t) {
        return os.write((const char*) t.data(), t.size());
    }
    template<class T>
    static inline std::istream& __Read(std::istream& is, T& t) {
        return is.read((char*) t.data(), t.size());
    }
} BinConvert;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif  /* SBINCONTERT_H */

