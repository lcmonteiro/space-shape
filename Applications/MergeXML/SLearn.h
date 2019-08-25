/**
 * ------------------------------------------------------------------------------------------------
 * File:   SLearn.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef LEARN_H
#define LEARN_H
/**
 * std
 */
#include <map>
#include <set>
/**
 * space
 */
#include "SConvertJson.h"
#include "SConvertXml.h"
#include "SConvert.h"
#include "SVariable.h"
#include "SFind.h"
/**
 * namespaces
 */
using namespace Tools;
/**
 * ----------------------------------------------------------------------------
 * Export learn XML file 
 * ----------------------------------------------------------------------------
 */
static inline int Learn(String in, String filter, String out) {
    auto profile = Map();
    /**
     * learn each file
     */
    for(Var file : Find(in, filter)) {
        INFO("Learn", "file= " << String(file));
        /**
         * parse file
         */
        Logic::ForEach(Convert::FromXML(File::Reader(file)), [&](const List& p, Var v) -> Var {
            /**
             * find lists
             */
            if(Var::IsList(v)) {
                auto path = Var::ToString(p.back());
                /**
                 * process document
                 */
                auto cache = std::map<Key, std::map<Key, std::set<Key>>>();
                for(Var d : Var::List(v)) {
                    for (auto& p : Convert::ToSimpleMap(Var::Trim(d))){
                        try {
                            cache.at(path).at(p.first).emplace(Var::ToString(p.second));
                        } catch (...) {
                            auto found = cache.find(path);
                            if(found != cache.end()) { 
                                found->second.emplace(p.first, std::set<Key>{
                                    Var::ToString(p.second)
                                });
                            } else {
                                cache.emplace(path, std::map<Key, std::set<Key>>{
                                    {p.first, std::set<Key>{
                                        Var::ToString(p.second)
                                    }}
                                });
                            }
                        }
                    }
                }
                /**
                 * update profile
                 */
                for(auto& elem : cache) {
                    Var type = Logic::Get(elem.first, profile, Obj(Map()));
                    for(auto& path : elem.second) {
                        Var::Integer(
                            Logic::Get(path.first, Var::Map(type), Obj(Integer()))
                        ) += path.second.size();   
                    }
                }
                return Obj(nullptr);
            }
            return v;
        });
    }
    /**
     * Select and write to file
     */
    Convert::ToPrettyJson(File::Writer(out), Obj{
        {Key(filter), Obj(Logic::ForEach(std::move(profile), [](auto, Var v) {
        return Obj(Logic::ForEach(Basic::Select(Basic::Sort(Convert::ToList(v), [](Var a, Var b) {
            if(a[1] == b[1]) {
                if(Basic::Count(Var::ToString(a[0]), '#') > 0) {
                    return true;
                }  
                return Basic::Count(Var::ToString(a[0]), '/') < Basic::Count(Var::ToString(b[0]), '/');
            }
            return a[1] > b[1];
        }), 0, 3), [](auto, Var v){
            return v[0];
        }));
    }))}});
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* LEARN_H */