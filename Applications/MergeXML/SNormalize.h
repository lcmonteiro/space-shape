/**
 * ------------------------------------------------------------------------------------------------
 * File:   SNormalize.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef NORMALIZE_H
#define NORMALIZE_H
/**
 * space
 */
#include "SConvertJSON.h"
#include "SConvertXML.h"
#include "SFileStream.h"
#include "SVariable.h"
#include "SPattern.h"
#include "SFind.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Normalize XML - File
 * ------------------------------------------------------------------------------------------------
 */
static inline int Normalize(String file, List profile) {
    /**
     * write normalize data
     */
    Convert::ToXML(File::Writer(file), Logic::ForEach(
        /**
         * read and normalize
         */
        Edit::Normalize(Convert::FromXML(File::Reader(file))), 
        /**
         * sort lists
         */
        [&profile](auto p, Var v) -> Var {
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
                Tools::Basic::Sort(Var::List(v), [&l](Var a, Var b) {
                    /**
                     * join key list as a string
                     */
                    return Convert::ToString(Logic::ForEach(Var::ToList(l), [&a](auto, Var v) {
                        try { return Edit::Find(Key(v), a);} catch (...) { return Var(Obj(nullptr)); }    
                    }), "") < Convert::ToString(Logic::ForEach(Var::ToList(l), [&b](auto, Var v) {
                        try { return Edit::Find(Key(v), b);} catch (...) { return Var(Obj(nullptr)); }
                    }), "");
                });
            }
            return v;
        }
    ));
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Normalize XML - Recursive
 * ------------------------------------------------------------------------------------------------
 */
static inline int Normalize(String in, String filter, Map profiles) {
    /**
     * normalize each file
     */
    // for(Var file : Find(in, filter)) {
    //     INFO("Normalize", "file= " << String(file));
    //     /**
    //      * find a profile
    //      */ 
    //     DEBUG("profiles", profiles);
    //     Var profile = Obj(Map());
    //     Logic::ForEach(profiles, [&](auto k, Var v) {
    //         if(Tools::Pattern::Match(file, k)) { 
    //             profile = Logic::MergeEach(v, profile, [](auto, Var v, Var) {
    //                 return v;
    //             }); 
    //         }
    //         return v;
    //     });
    //     /**
    //      * write normalize data
    //      */
    //     DEBUG("profile", profile);
    //     Normalize(file, Var::Map(profile));
    // }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* NORMALIZE_H */