/**
 * ------------------------------------------------------------------------------------------------
 * File:   SConvertBIN.cpp
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#include "SConvertBIN.h"
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
 *  To Bin
 * ------------------------------------------------------------------------------------------------
 *  Private
 * ----------------------------------------------------------------------------
 * definitions
 */
static inline void __ToBin(std::ostream&, List    );
static inline void __ToBin(std::ostream&, Map     );
static inline void __ToBin(std::ostream&, String  );
static inline void __ToBin(std::ostream&, Buffer  );
static inline void __ToBin(std::ostream&, Integer );
static inline void __ToBin(std::ostream&, Float   );
static inline void __ToBin(std::ostream&, Boolean );
static inline void __ToBin(std::ostream&, Link    );
template<class T>
static inline void __Write(std::ostream&, T        );
/**
 * implementation
 */
template<class T>
void __Write(std::ostream& os, T t) {
    os.write((const char*) t.data(), t.size());
}
void __ToBin(std::ostream& os,  Var v) {
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
}
void __ToBin(std::ostream& os, List l) {
    __Write(os.put(__LIST__T__), Integer(l.size()));
    for (auto& v : l) {
        __ToBin(os, v);
    }
}
void __ToBin(std::ostream& os, Map l) {
    __Write(os.put(__MAP___T__), Integer(l.size()));
    for (auto& v : l) {
        __Write(os, Integer(v.first.size()));
        __Write(os, v.first);
        __ToBin(os, v.second);
    }
}
void __ToBin(std::ostream& os, String s) {
    __Write(os.put(__STR___T__), Integer(s.size()));
    __Write(os, s);
}
void __ToBin(std::ostream& os, Buffer b) {
    __Write(os.put(__BUF___T__), Integer(b.size()));
    __Write(os, b);
}
void __ToBin(std::ostream& os, Integer i) {
    __Write(os.put(__INT___T__), i);
}
void __ToBin(std::ostream& os, Float f) {
    __Write(os.put(__FLOAT_T__), f);
}
void __ToBin(std::ostream& os, Boolean b) {
    __Write(os.put(__BOOL__T__), b);
}
void __ToBin(std::ostream& os, Link l) {
    __ToBin(os.put(__LINK__T__), Var(l));
}
/**
 * ----------------------------------------------------------------------------
 * Public
 * ----------------------------------------------------------------------------
 */
std::ostream& Convert::ToBin(std::ostream&& os, Var v) {
    __ToBin(os, v);
    return os;
}
std::ostream& Convert::ToBin(std::ostream& os,  Var v) {
    __ToBin(os, v);
    return os;
}
/**
 * ------------------------------------------------------------------------------------------------
 *  From Bin
 * ------------------------------------------------------------------------------------------------
 *  Private
 * ----------------------------------------------------------------------------
 * definitions
 */
static inline Var&&     __FromBin(std::istream&, Var&&     );
static inline List&&    __FromBin(std::istream&, List&&    );
static inline Map&&     __FromBin(std::istream&, Map&&     );
static inline String&&  __FromBin(std::istream&, String&&  );
static inline Buffer&&  __FromBin(std::istream&, Buffer&&  );
static inline Integer&& __FromBin(std::istream&, Integer&& );
static inline Float&&   __FromBin(std::istream&, Float&&   );
static inline Boolean&& __FromBin(std::istream&, Boolean&& );
template<class T>
static inline void __Read(std::istream&, T&);
/**
 * implementation
 */
template<class T>
void __Read(std::istream& is, T& t) {
    is.read((char*) t.data(), t.size());
}
Var&& __FromBin(std::istream& is, Var&& d) {
    switch (is.get()) {
        case __LIST__T__:
            d = Obj(__FromBin(is, List()));
            break;
        case __MAP___T__:
            d = Obj(__FromBin(is, Map()));
            break;
        case __STR___T__:
            d = Obj(__FromBin(is, String()));
            break;
        case __INT___T__:
            d = Obj(__FromBin(is, Integer()));
            break;
        case __FLOAT_T__:
            d = Obj(__FromBin(is, Float()));
            break;
        case __BOOL__T__:
            d = Obj(__FromBin(is, Boolean()));
            break;
        case __BUF___T__:
            d = Obj(__FromBin(is, Buffer()));
            break;
        case __LINK__T__:
            d = Obj::Link(__FromBin(is, Var()));
            break;
        case __NULL__T__:
            d = Obj();
            break;
        default:;
    }
    return std::move(d);
}
List&& __FromBin(std::istream& is, List&& l) {
    Integer len(0);
    __Read(is, len);
    l.reserve(len);
    for (int i = 0; i < len; i++) {
        l.push_back(__FromBin(is, Var()));
    }
    return std::move(l);
}
Map&& __FromBin(std::istream& is, Map&& m) {
    Integer len(0);
    __Read(is, len);
#ifdef VAR_UNORDERED_MAP  
    m.reserve(len);
#endif
    for (int i = 0; i < len; i++) {
        String key = __FromBin(is, String());
        m[std::move(key)] = __FromBin(is, Var());
    }
    return std::move(m);
}
String&& __FromBin(std::istream& is, String&& s) {
    Integer len(0);
    __Read(is, len);
    s.resize(len, 0x00);
    __Read(is, s);
    return std::move(s);
}
Buffer&& __FromBin(std::istream& is, Buffer&& b) {
    Integer len(0);
    __Read(is, len);
    b.resize(len, 0x00);
    __Read(is, b);
    return std::move(b);
}
Integer&& __FromBin(std::istream& is, Integer&& i) {
    __Read(is, i);
    return std::move(i);
}
Float&& __FromBin(std::istream& is, Float&& f) {
    __Read(is, f);
    return std::move(f);
}
Boolean&& __FromBin(std::istream& is, Boolean&& b) {
    __Read(is, b);
    return std::move(b);
}
/**
 * ----------------------------------------------------------------------------
 * Public
 * ----------------------------------------------------------------------------
 */
Var Convert::FromBin(std::istream&& is) {
    return __FromBin(is, Var());
}
Var Convert::FromBin(std::istream&  is) {
    return __FromBin(is, Var());
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */


