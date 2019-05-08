/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   SLogic.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SLOGIC_H
#define SLOGIC_H
/**
 * space
 */
#include "SEdit.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Logic - Space
 * ------------------------------------------------------------------------------------------------
 */
namespace Logic {
    /**
     * --------------------------------------------------------------------------------------------
     * Exceptions 
     * --------------------------------------------------------------------------------------------
     */
    typedef std::runtime_error SLogicException;
    /**
     * --------------------------------------------------------------------------------------------
     * Conditions 
     * --------------------------------------------------------------------------------------------
     */
    template<typename Function1, typename Function2>
    static inline Var Select(bool test, Function1 func_true, Function2 func_false) {
        return test ? func_true() : func_false();
    }
    static inline Var IfnDefined(Var var, Obj def) {
        return Var::IsDefined(var) ? var : def;
    }
    static inline Var IfnDefined(Var var, Var def) {
        return Var::IsDefined(var) ? var : def;
    }
    template<typename Function>
    static inline Var IfnDefined(Var var, Function func) {
        return Var::IsDefined(var) ? var : func();
    }
    static inline Var IfnEmpty(Var var, Var def) {
        return Var::IsEmpty(var) ? def : var;
    }
    template<typename Function>
    static inline Var IfDefined(Var var, Function func) {
        return Var::IsDefined(var) ? func(var) : var;
    }
    template<typename Function>
    static inline Var IfException(Function func, Obj except) {
        try {
            return func();
        } catch(...) {
            return except;
        }
    }
    template<typename Function>
    static inline Var IfException(Function func, Var except) {
        try {
            return func();
        } catch(...) {
            return except;
        }
    }
    template<typename Function1, typename Function2>
    static inline Var IfException(Function1 func, Function2 except) {
        try {
            return func();
        } catch(...) {
            return except();
        }
    }
    template<typename Function1, typename Function2>
    static inline Var IfList(Var v, Function1 succ, Function2 fail) {
        return Var::IsList(v) ? succ(v) : fail(v);
    }
    template<typename Function1, typename Function2>
    static inline Var IfMap(Var v, Function1 succ, Function2 fail) {
        return Var::IsMap(v) ? succ(v) : fail(v);
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Call function (func) for all elements
     * --------------------------------------------------------------------------------------------
     */
    template<typename Function>
    static List ForEach(List list, Function func);
    template<typename Function>
    static Map  ForEach(Map map,   Function func);
    template<typename Function>
    static Var  ForEach(Var var,   Function func, false_type);
    template<typename Function>
    static Var  ForEach(Var var,   Function func, true_type);
    template<bool LINKS = false, typename Function>
    static Var ForEach(Var var,    Function func)
    {
        return ForEach(var, func,
            typename integral_constant<bool, LINKS>::type()
        );
    }

    /**
     * ForEach
     * @example Logic::ForEach<Map>(vector<T> in, [](Map& o, T& i){
     *      o[i.a()] = Obj(i.a());
     * })
     */
    template<typename Output, typename Input, typename Function>
    static Output ForEach(Input& in, Function func);
    /**
     * ForEachPair
     * @example
     */
    template<typename Function>
    static Map ForEachPair(Map map, Function func);
    
    /**
     * --------------------------------------------------------------------------------------------
     * Logic Converters
     * --------------------------------------------------------------------------------------------
     */
    static inline List ToList(Map data) {
        List l; for (auto& v : data) { l.push_back(v.second); } return l;
    }
    static inline List ToList(Var data) {
        return Var::IsList(data) ? Var::List(data) : Var::IsDefined(data) ? List{data}: List{};
    }
    static inline Map ToMap(Var data) {
        return Var::IsMap(data) ? Var::Map(data) : Var::IsDefined(data) ? Map{{Key(""), data}}: Map{};
    }
    static inline String ToString(Var data) {
        return Var::IsString(data) ? Var::String(data) : Var::IsDefined(data) ? String(data): String();
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Logic Update
     * --------------------------------------------------------------------------------------------
     */
    template<typename Function>
    static inline Var Update(Key path, Function func, Var data) {
        return Edit::Insert(path, func(Edit::Remove(path, data)), data);
    }
    /**
     * --------------------------------------------------------------------------------------------
     * Logic Link
     * --------------------------------------------------------------------------------------------
     */
    static inline Var Link(Var from, Var map, Var to = Obj::Map()) {
        for(auto& m : ToList(map)){
            to = Edit::Link(from, m, to);
        }
        return to;
    }
    /**
     * --------------------------------------------------------------------------------------------
     * assert
     * --------------------------------------------------------------------------------------------
     */
    template<class T>  
    static inline T Assert(T obj, Boolean(T::*method)()) {
        if(!((obj.*method)())) {
            throw SLogicException("assert");
        }
        return obj;
    }
    static inline void Assert(bool test) {
        if(!test) {
            throw SLogicException("assert");
        }
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * Implementation 
 * ------------------------------------------------------------------------------------------------
 */
template<typename Function>
Var Logic::ForEach(Var var, Function func, true_type) {
    if (Var::IsMap(var)) {
        ::Map& map = Var::Map(var);
        for (auto& v : map) {
            v.second = ForEach(v.second, func, true_type());
        }
        return var;
    }
    if (Var::IsList(var)) {
        ::List& list = Var::List(var);
        for (auto& v : list) {
            v = ForEach(v, func, true_type());
        }
        return var;
    }
    if (Var::IsLink(var)) {
        ::Link& link = Var::Link(var);
        link = ForEach(link, func, true_type());
        return var;
    }
    return func(var);
}
template<typename Function>
Var Logic::ForEach(Var var, Function func, false_type) {
    if (Var::IsMap(var)) {
        ::Map& map = Var::Map(var);
        for (auto& v : map) {
            v.second = ForEach(v.second, func, false_type());
        }
        return var;
    }
    if (Var::IsList(var)) {
        ::List& list = Var::List(var);
        for (auto& v : list) {
            v = ForEach(v, func, false_type());
        }
        return var;
    }
    return func(var);
}
template<typename Function>
List Logic::ForEach(List list, Function func) {
    for (auto& v : list) {
        v = func(v);
    }
    return list;
}
template<typename Function>
Map Logic::ForEach(Map map, Function func) {
    for (auto& v : map) {
        v.second = func(v.second);
    }
    return map;
}
template<typename Function>
Map Logic::ForEachPair(Map map, Function func) {
    for (auto& v : map) {
        v.second = func(v.first, v.second);
    }
    return map;
}
template<typename Output, typename Input, typename Function>
Output Logic::ForEach(Input& in, Function func) {
    Output out; 
    for (auto& i : in) {
        func(out, i); 
    } 
    return out;
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif /* SLOGIC_H */

