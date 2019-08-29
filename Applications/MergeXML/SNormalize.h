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
        Edit::Normalize(Convert::FromXML(File::Reader(in))), 
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
                        try { return Edit::Find(Key(v), a);} catch (...) { return Var(Obj()); }    
                    }), "") < Convert::ToString(Logic::ForEach(Var::ToList(l), [&b](auto, Var v) {
                        try { return Edit::Find(Key(v), b);} catch (...) { return Var(Obj()); }
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
    for(Var file : Find(in, filter)) {
        INFO("Normalize", "file= " << String(file));
        /**
         * write normalize data
         */
        Normalize(file, file, Var::ToList(profiles[FileSystem::GetExtension(file)]));
    }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * Normalize XML - File 
 * ------------------------------------------------------------------------------------------------
 */
static inline int Normalize(String in, Map profiles) {
    /**
     * write normalize data
     */
    Normalize(in, in, Var::ToList(profiles[FileSystem::GetExtension(in)]));
    /**
     */
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* NORMALIZE_H */