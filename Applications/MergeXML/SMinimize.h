/**
 * ------------------------------------------------------------------------------------------------
 * File:   SNormalize.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SCOMPARE_H
#define SCOMPARE_H
/**
 * space
 */
#include "SConvertJSON.h"
#include "SConvertXML.h"
#include "SFileStream.h"
#include "SVariable.h"
#include "SVariant.h"
#include "SPattern.h"
#include "SFind.h"
#include "SLog.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Minimize - Engine
 * ------------------------------------------------------------------------------------------------
 */
namespace {
    /**
     * --------------------------------------------------------------------------------------------
     * minimize function
     * --------------------------------------------------------------------------------------------
     */
    template<typename Function>
    inline Var Minimize(Var doc, Var ref, Function find, String cache={}) {
        /**
         * process map 
         */
        if(Var::IsMap(ref) &&  Var::IsMap(doc)) {
            auto& m_d = Var::Map(doc);
            auto& m_r = Var::Map(ref);
            for (auto it = m_r.begin(); it != m_r.end(); ++it) {
                auto found = m_d.find(it->first);
                if (found != m_d.end()) {
                    found->second = Minimize(found->second, it->second, find, it->first);
                }
            }
            return doc;
        } 
        /**
         * sort list
         */
        if(Var::IsList(ref) && Var::IsList(doc)) {
            // map the similar objects
            auto map = std::multimap<Link, Link>();
            for(Var d : Var::List(doc)) {
                map.emplace(find(d, Var::List(ref), cache), d);
            }
            // sort objects
            auto list = List();
            for(Var d : Var::List(doc)) {
                for(auto it=map.find(d); it != map.find(d); ++it) {
                    list.push_back(it->second);
                }
            }
            return Obj(list);
        }
        /**
         * others
         */
        return doc;
    }
    /**
     * --------------------------------------------------------------------------------------------
     * structure extract
     * --------------------------------------------------------------------------------------------
     */
    inline Map Extract(Var document, const List& profile) {
        Map out;
        DEBUG("d", document);
        /** 
         */
        Logic::ForEach(document, [&profile, &out](auto p, Var v) -> Var {
            if(!Var::IsList(v)) {
                return v;
            }
            for(auto& r : profile) {
                if(!Var::IsList(r)) {
                    continue;
                }
                auto k = Var::List(r).at(0);
                auto l = Var::List(r).at(1); 
                if(!Tools::Pattern::Match(Var::ToString(p.back()), Var::ToString(k))) {
                    continue;
                }
                DEBUG("1", Convert::ToPath(p));
                out.emplace(
                    // key
                    Convert::ToPath(p), 
                    // value
                    Obj(Tools::Basic::Accumulate(Var::List(v), List(), 
                        [&l](List a, Var e) {
                            return a + Obj(Tools::Basic::Accumulate(Var::ToList(l), List(), 
                                [&e](List b, Var v) {
                                    return b + Edit::Find(Key(v), e);
                                })
                            );
                        })
                    )
                );
            }
            return v;
        });
        return out;
    }
}

inline int Minimize(const List& files, const List& profile) {
    DEBUG("files", files);

    auto it  = files.begin();
    auto end = files.end();
    if(it != end) {
        auto ref = Convert::FromXML(File::Reader(Var::ToString(*it)));
        for(++it; it != end; ++it) {
            DEBUG("minimize", 
                Minimize(Convert::FromXML(File::Reader(Var::ToString(*it))), ref,
                    [&profile](auto doc, auto ref, auto key) {
                        return Obj();
                    }
                )
            );
        }
    }
    /**
     * write normalize data
     */
    for(Var file: files) {
        // DEBUG("extract", Extract(Convert::FromXML(File::Reader(file)), profile));


        // Convert::ToJson(File::Writer(file), Logic::ForEach(
        //     /**
        //      * read and normalize
        //      */
        //     Convert::FromXML(File::Reader(file)), 
        //     /**
        //      * sort lists
        //      */
        //     [&profile](auto p, Var v) -> Var {
        //         if(!Var::IsList(v)) {
        //             return v;
        //         }
        //         for(auto& r : profile) {
        //             if(!Var::IsList(r)) {
        //                 continue;
        //             }
        //             auto k = Var::List(r).at(0);
        //             auto l = Var::List(r).at(1); 
        //             if(!Tools::Pattern::Match(Var::ToString(p.back()), Var::ToString(k))) {
        //                 continue;
        //             }
        //             Tools::Variant::Sort(Var::List(v), [&l](Var a, Var b) {

        //                 for(Var p : Var::ToList(l)) {
        //                     String v_a = Edit::Find(Key(p), a);
        //                     String v_b = Edit::Find(Key(p), b);
        //                     if(v_a != v_b) {
        //                         return v_a < v_b;
        //                     }
        //                 }
        //                 return false;
        //             });
        //         }
        //         return v;
        //     }
        // ));
    }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Compare XML - File 
 * ------------------------------------------------------------------------------------------------
 */
inline int Minimize(List files, Map profiles, String filter) {
    /**
     * write minimize data
     */
    return Minimize(files, Var::ToList(Tools::Basic::Accumulate(files, profiles[filter], 
        [&profiles](Var p, Var f) {
            return Var::IsDefined(p) ? p : profiles[FileSystem::GetExtension(f)];
        })
    ));
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* SCOMPARE_H */