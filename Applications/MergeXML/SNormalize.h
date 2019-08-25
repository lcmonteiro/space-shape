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
 * ----------------------------------------------------------------------------
 * Normalize XML
 * ----------------------------------------------------------------------------
 */
static inline int Normalize(String in, String filter, Map profiles) {
    /**
     * normalize each file
     */
    for(Var file : Find(in, filter)) {
        INFO("Normalize", "file= " << String(file));
        /**
         * find a profile
         */ 
        DEBUG("profiles", profiles);
        Var profile = Obj(Map());
        Logic::ForEach(profiles, [&](auto k, Var v) {
            if(Tools::Pattern::Match(file, k)) { 
                profile = Logic::MergeEach(v, profile, [](auto, Var v, Var) {
                    return v;
                }); 
            }
            return v;
        });
        DEBUG("profile", profile);
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
                if(Var::IsList(v)) {
                    for(auto& r : Var::ToMap(profile)) {
                        if(Tools::Pattern::Match(Var::ToString(p.back()), r.first)) {
                            Tools::Basic::Sort(Var::List(v), [&r](Var a, Var b) {
                                /**
                                 * join key list as a string
                                 */
                                return Convert::ToString(Logic::ForEach(Var::ToList(r.second), [&a](auto, Var v) {
                                    try { return Edit::Find(Key(v), a);} catch (...) { return Var(Obj(nullptr)); }    
                                }), "") < Convert::ToString(Logic::ForEach(Var::ToList(r.second), [&b](auto, Var v) {
                                    try { return Edit::Find(Key(v), b);} catch (...) { return Var(Obj(nullptr)); }
                                }), "");
                            });
                        }
                    }
                }
                return v;
            }
        ));
    }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* NORMALIZE_H */