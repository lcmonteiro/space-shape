/**
 * --------------------------------------------------------------------------------------------------------------------
 * File:   SMininize.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * --------------------------------------------------------------------------------------------------------------------
 */
#ifndef SMINIMIZE_H
#define SMINIMIZE_H
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
 * --------------------------------------------------------------------------------------------------------------------
 * Minimize - Engine
 * --------------------------------------------------------------------------------------------------------------------
 */
namespace {
    /**
     * ------------------------------------------------------------------------------------------------------
     * minimize function
     * ------------------------------------------------------------------------------------------------------
     */
    template<typename Function>
    inline Var Minimize(Var doc, Var ref, Function find, const List& profile, const String& context={}) {
        /**
         * ----------------------------------------------------------------------------------------
         * tools 
         * ----------------------------------------------------------------------------------------
         * remainig 
         * --------------------------------------------------------------------
         */
        struct remaining : public std::list<Link> {
            using base = std::list<Link>;
            remaining(const List& list) : base(list.begin(), list.end()) {
            }
            inline void update(base::const_iterator it) {
                erase(it);
            }
        };
        /**
         * --------------------------------------------------------------------
         * mapping
         * --------------------------------------------------------------------
         */
        struct mapping : public std::multimap<Link, Link> {
            using base = std::multimap<Link, Link>;
            mapping(): base() {
            }
            inline void update(Link key, Link val) {
                emplace(key, val);
            }
        };
        /**
         * ----------------------------------------------------------------------------------------
         * process map 
         * ----------------------------------------------------------------------------------------
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
         * ----------------------------------------------------------------------------------------
         * sort list
         * ----------------------------------------------------------------------------------------
         */
        if(Var::IsList(ref) && Var::IsList(doc)) {
            auto key = Var::List(ref);
            auto val = Var::List(doc);
            /**
             * --------------------------------------------------------------------------
             * find all paths
             * --------------------------------------------------------------------------
             */
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
            /**
             * --------------------------------------------------------------------------
             * match elements
             * --------------------------------------------------------------------------
             */
            auto map = mapping();
            auto rem = remaining(key);        
            auto end = std::remove_if(val.begin(), val.end(), [&rem, &map, &paths](auto v) {
                auto found = std::find_if(rem.begin(), rem.end(), [&paths, &v](auto k) {
                    return std::all_of(paths.begin(), paths.end(), [&k, &v](auto p) {
                        try {
                            return String(Edit::Find(p, k))==String(Edit::Find(p, v));
                        } catch (...) {
                            return false;
                        }
                    });
                });
                if(found != rem.end()) {
                    map.update(*found, v);
                    rem.update(found);
                    return true;
                }
                return false;
            });
            /**
             * --------------------------------------------------------------------------
             * map the similar objects
             * --------------------------------------------------------------------------
             */
            for(auto it = val.begin(); it != end; ++it) {
                try {
                    auto found = find(key.begin(), key.end(), *it, paths);
                    if(found != key.end()) {
                        map.update(*found, *it);
                    }
                } catch(std::logic_error&) {
                    map.update(Obj(), *it);
                }
            }
            /**
             * --------------------------------------------------------------------------
             * sort objects
             * --------------------------------------------------------------------------
             */
            auto list = List();
            for(Link d : Var::List(ref)) {
                auto ret = map.equal_range(d);
                for (auto it= ret.first; it != ret.second; ++it) {
                    list.emplace_back(Minimize(it->second, d, find, profile, context));
                }
            }
            return Obj(list);
        }
        /**
         * ----------------------------------------------------------------------------------------
         * others
         * ----------------------------------------------------------------------------------------
         */
        return doc;
    }
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * Interfaces
 * --------------------------------------------------------------------------------------------------------------------
 */
inline int Minimize(const List& files, const Link& sort, const Link& schema) {
    auto it  = files.begin();
    auto end = files.end();
    if(it == end) {
        ERROR("Minimize", "Files Not Found ...");
        return -1;
    }
    /**
     * --------------------------------------------------------------------------------------------
     * normalize current
     * --------------------------------------------------------------------------------------------
     */
    auto ref = Convert::FromXML(File::Reader(Var::ToString(*it)));
    if(Var::IsUndefined(ref)) {
        ERROR("Minimize", "File Reference Not Found ...");
        return -2;
    }
    Convert::ToXML(File::Writer(Var::ToString(*it)), ref, schema);
    /**
     * --------------------------------------------------------------------------------------------
     * minimize base on ref
     * --------------------------------------------------------------------------------------------
     */
    for(++it; it != end; ++it) {
        Convert::ToXML(File::Writer(Var::ToString(*it)),
            Minimize(Convert::FromXML(File::Reader(Var::ToString(*it))), ref,
                [](auto begin, auto end, auto doc, auto paths) {
                    /**
                     * compute the distance
                     */
                    std::map<Link, std::vector<size_t>> map;
                    std::for_each(begin, end, [&doc, &paths, &map](auto ref) {
                        std::vector<size_t> value;
                        for(auto&p : paths) {
                            value.emplace_back(Tools::Math::LevensteinDistance(
                                String(Edit::Find(p, doc)),
                                String(Edit::Find(p, ref))
                            ));
                        }
                        map.emplace(ref, std::move(value));
                    });
                    /**
                     * find the minimum distance
                     */
                    return std::min_element(begin, end, [&map](auto cur, auto min) {
                        auto& c = map[cur];
                        auto& m = map[min];
                        for(auto it_c = c.begin(), it_m = m.begin(); it_c != c.end(); ++it_c, ++it_m) {
                            if(*it_c != *it_m) {
                                return *it_c <= *it_m;
                            }
                        }
                        return false;
                    });
                }, Var::ToList(sort)
            ), schema
        );
    }
    return 0;
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * Minimize XML Files 
 * --------------------------------------------------------------------------------------------------------------------
 */
inline int Minimize(List files, Map profiles, String filter) {
    /**
     * write minimize data
     */
    Var profile = Tools::Basic::Accumulate(files, profiles[filter], 
        [&profiles](Var p, Var f) {
            return Var::IsDefined(p) ? p : profiles[FileSystem::GetExtension(f)];
        }
    );
    return Minimize(files, profile[KEY_SORT], profile[KEY_SCHEMA]);
}
/**
 * --------------------------------------------------------------------------------------------------------------------
 * End
 * --------------------------------------------------------------------------------------------------------------------
 */
#endif	/* SMINIMIZE_H */