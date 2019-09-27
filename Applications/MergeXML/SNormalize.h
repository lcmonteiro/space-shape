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
#include "SVariant.h"
#include "SPattern.h"
#include "SFind.h"
#include "SLog.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Normalize - Engine
 * ------------------------------------------------------------------------------------------------
 */
static inline int Normalize(const String& in, const String& out, const List& profile) {
    /**
     * write normalize data
     */
    Convert::ToXML(File::Writer(out), Logic::ForEach(
        /**
         * read and normalize
         */
        Convert::FromXML(File::Reader(in)), 
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
                Tools::Variant::Sort(Var::List(v), [&l](Var a, Var b) {
                    try {
                        for(Var p : Var::ToList(l)) {
                            String v_a = Edit::Find(Key(p), a);
                            String v_b = Edit::Find(Key(p), b);
                            if(v_a != v_b) {
                                return v_a < v_b;
                            }
                        }
                    } catch(...) { }
                    /**
                     * fallback
                     */
                    return false;
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
static inline int NormalizeRecursive(String in, Map profiles, String filter) {
    /**
     * normalize each file
     */
    for(Var file : Find(in, filter)) {
        INFO("Normalize", "file= " << String(file));
        /**
         * write normalize data
         */
        Var profile = profiles[
            filter.empty() ? FileSystem::GetExtension(in) : filter
        ];
        Normalize(file, file, Var::ToList(profile[KEY_SORT]));
    }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Normalize XML - File 
 * ------------------------------------------------------------------------------------------------
 */
static inline int Normalize(String in, Map profiles, String filter="") {
    /**
     * write normalize data
     */
    Var profile = profiles[
        filter.empty() ? FileSystem::GetExtension(in) : filter
    ];
    Normalize(in, in, Var::ToList(profile[KEY_SORT]));
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* NORMALIZE_H */