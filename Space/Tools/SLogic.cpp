
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
template<typename Function>
Var __ForEach(List& path, Var var, Function update) {
    switch(Var::Type(var)){
        case Obj::MAP: {
            for (auto& v : Var::Map(var)) {
                path.emplace_back(Obj(v.first));
                v.second = __ForEach(path, v.second, update);
                path.pop_back();
            }
            break;
        }
        case Obj::LIST: {
            auto i = 0; 
            for (auto& v : Var::List(var)) {
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
Var Logic::ForEach(Var var, UpdateFunction update) {
    List path;
    return __ForEach(path, var, update);
}
/**
 * ------------------------------------------------------------------------------------------------
 * MergeEach - Implementation
 * ------------------------------------------------------------------------------------------------
 * Private
 * --------------------------------------------------------------------
 */
template<typename Function>
Var __MergeEach(List& path, const Var var, Var on, Function merge) {
    switch(Var::Type(var)) {
        case Obj::MAP: {
            if(Var::Type(on) == Obj::MAP) {
                auto m_v = Var::Map(var);
                auto m_o = Var::Map(on);
                for (auto it = m_v.begin(); it != m_v.end(); ++it) {
                    path.emplace_back(Obj(it->first));
                    // check if exist 
                    auto found = m_o.find(it->first);
                    if (found != m_o.end()) {
                        found->second = __MergeEach(
                            path, it->second, found->second, merge
                        );
                    } else {
                        auto merged = merge(
                            const_cast<List&>(path), it->second, Obj()
                        );
                        if(Var::IsDefined(merged)) {
                            m_o[it->first] = merged;
                        }
                    }
                    path.pop_back();   
                }
            }
            break;
        }
        case Obj::LIST: {
            if(Var::Type(on) == Obj::LIST) {
                auto l_v = Var::List(var);
                auto l_o = Var::List(on);
                for (size_t i = 0; i <= l_v.size(); ++i) {
                    path.emplace_back(Obj(int(i)));
                    // helpers
                    auto& ref_v = l_v[i];
                    auto& ref_o = l_o[i];
                    // check if exist
                    if(i < l_o.size()) {
                        ref_v = __MergeEach(path, ref_v, ref_o, merge);
                    } else {
                        auto merged = merge(
                            const_cast<List&>(path), ref_v, Obj()
                        );
                        if(Var::IsDefined(merged)) {
                            l_o.emplace_back(merged);
                        }
                    }
                    path.pop_back();   
                }
            }
            break;    
        }
    }
    return merge(const_cast<List&>(path), var, on);
}
/**
 * ----------------------------------------------------------------------------
 * Public
 * ----------------------------------------------------------------------------
 **
 * var
 */
Var Logic::MergeEach(const Var var, Var on, MergeFunction merge) {
    List path;
    return __MergeEach(path, var, on, merge);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */