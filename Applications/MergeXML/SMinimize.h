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
#include "SMath.h"
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
    inline Var Minimize(Var doc, Var ref, Function find, const List& profile, const String& context={}) {
        /**
         * process map 
         */
        if(Var::IsMap(ref) &&  Var::IsMap(doc)) {
            auto& m_d = Var::Map(doc);
            auto& m_r = Var::Map(ref);
            for (auto it = m_r.begin(); it != m_r.end(); ++it) {
                auto found = m_d.find(it->first);
                if (found != m_d.end()) {
                    found->second = Minimize(found->second, it->second, find, profile, it->first);
                }
            }
            return doc;
        } 
        /**
         * sort list
         */
        if(Var::IsList(ref) && Var::IsList(doc)) {
            // find all paths
            std::list<Key> paths;
            for(auto& r : profile) {
                if(Var::IsList(r)) {
                    auto k = Var::List(r).at(0);
                    auto l = Var::List(r).at(1); 
                    if(Tools::Pattern::Match(context, Var::ToString(k))) {
                        for(Var p : Var::ToList(l)) {
                            paths.emplace_back(Key(p));
                        }
                    }
                }
            }
            // map the similar objects
            auto map   = std::multimap<Link, Link>();
            auto cache = Var::List(ref);
            for(Link d : Var::List(doc)) {
                auto found = find(d, cache, paths), d);
                map.emplace(find(d, cache, paths), d);
            }
            // sort objects
            auto list = List();
            for(Link d : Var::List(ref)) {
                DEBUG("ref", Edit::Find("SHORT-NAME", d));
                auto ret = map.equal_range(d);
                for (auto it= ret.first; it != ret.second; ++it) {
                    DEBUG("doc", Edit::Find("SHORT-NAME", it->second));
                    //list.emplace_back(it->second);
                    list.emplace_back(Minimize(it->second, d, find, profile, context));
                }
            }
            return Obj(list);
        }
        /**
         * others
         */
        return doc;
    }
}
/**
 * ------------------------------------------------------------------------------------------------
 * Interfaces
 * ------------------------------------------------------------------------------------------------
 */
inline int Minimize(const List& files, const List& profile) {
    auto it  = files.begin();
    auto end = files.end();
    if(it == end) {
        ERROR("Minimize", "Files Not Found ...");
        return -1;
    }
    /**
     * normalize current
     */
    auto ref = Convert::FromXML(File::Reader(Var::ToString(*it)));
    if(Var::IsUndefined(ref)) {
        ERROR("Minimize", "File Reference Not Found ...");
        return -2;
    }
    Convert::ToXML(File::Writer(Var::ToString(*it)), ref);
    /**
     * minimize base on ref
     */
    for(++it; it != end; ++it) {
        Convert::ToXML(File::Writer(Var::ToString(*it)),
            Minimize(Convert::FromXML(File::Reader(Var::ToString(*it))), ref,
                [](auto doc, auto beg, auto end, auto paths) {
                    /**
                     * compute the distance
                     */
                    std::map<Link, std::vector<size_t>> map;
                    for(auto it = beg; it != end; ++it) {
                        std::vector<size_t> value;
                        for(auto&p : paths) {
                            value.emplace_back(Tools::Math::LevensteinDistance(
                                String(Edit::Find(p, doc)),
                                String(Edit::Find(p, *it))
                            ));
                        }
                        if(std::accumulate(value.begin(), value.end(), 0) == 0) {
                            return {true, it};
                        }
                        map.emplace(r, std::move(value));
                    }
                    /**
                     * find the minimum distance
                     */
                    return {false, *std::min_element(ref.begin(), ref.end(), [&map](auto cur, auto min) {
                        auto& c = map[cur];
                        auto& m = map[min];
                        for(auto it_c = c.begin(), it_m = m.begin(); it_c != c.end(); ++it_c, ++it_m) {
                            if(*it_c != *it_m) {
                                return *it_c < *it_m;
                            }
                        }
                        return false;
                    })};
                }, profile
            )
        );
    }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Minimize XML Files 
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