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
     * minimize function
     */
    template<typename Function>
    inline Var Minimize(Var doc, Var ref, String cache, Function find) {
        if(Var::IsMap(ref) &&  Var::IsMap(doc)) {
            auto& m_d = Var::Map(doc);
            auto& m_r = Var::Map(ref);
            for (auto it = m_r.begin(); it != m_r.end(); ++it) {
                auto found = m_d.find(it->first);
                if (found != m_d.end()) {
                    m_d->second = Minimize(m_d->second, it->first, it->second, organize);
                }
            }
            return doc;
        }
        if(Var::IsList(ref) && Var::IsList(doc)) {
            /**
             * map the similar ones
             */
            auto map = std::multimap<Var, Var>();
            for(Var d : Var::List(doc)) {
                map.insert(find(d, Var::List(ref), d);
            }
            /**
             * 
             */
            auto map = std::multimap<Var, Var>();
            for(Var d : Var::List(doc)) {
                
                map.insert(find(d, Var::List(ref), d);
            }
            l_d = organize(l_d, l_r, cache);
        }
        return doc;
    }
    /**
     * structure extract
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
<<<<<<< HEAD
                Minimize(Convert::FromXML(File::Reader(Var::ToString(*it))), ref,
                    [&profile](Var doc, Var ref, String catch) {

                    }
                )
=======
                Minimize(Convert::FromXML(
                    File::Reader(Var::ToString(*it))
                ), ref, profile)
>>>>>>> 5c5d716dd73d21186f28ad98fb1a062fb0df9595
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