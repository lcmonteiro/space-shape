
/**
 * ------------------------------------------------------------------------------------------------
 * File:   Search.cc
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 **
 * space
 */
#include "SLogic.h"
/**
 * namespaces
 */
using namespace std; 
/**
 * ------------------------------------------------------------------------------------------------
 * Foreach - Implementation
 * ------------------------------------------------------------------------------------------------
 * Private
 * --------------------------------------------------------------------
 */
Var __ForEach(List& path, Var var, function<Var(const List&, Var)> update) {
    switch(Var::Type(var)){
        case Var::MAP: {
            for (auto& v : Var::Map(m)) {
                path.emplace_back(Obj(v.first));
                v.second = __ForEach(path, v.second, update);
                path.pop_back();
            }
            break;
        }
        case Var::LIST: {
            auto i = 0; 
            for (auto& v : Var::List(l)) {
                path.emplace_back(Obj(i++));
                v = __ForEach(path, v, update);
                path.pop_back();
            }
            break;    
        }
    }
    return update(const_cast<List&>(path), var);
}
/**
 * ----------------------------------------------------------------------------
 * Public
 * ----------------------------------------------------------------------------
 **
 * var
 */
Var Logic::ForEach(Var var, function<Var(const List&, Var)> update) {
    List path;
    return __ForEach(path, var, update);
}
/**
 * list 
 */
List Logic::ForEach(List list, function<Var(const size_t&, Var)> update) {
    size_t i = 0;
    for(auto& v: list) {
        v = update(i, v); ++i;
    }
    return move(list);
}
/**
 * map
 */
Map Logic::ForEach(Map map, function<Var(const Key&, Var)> update) {
    for(auto& v: map) {
        v.second = update(v.first, v.second);
    }
    return move(map);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */