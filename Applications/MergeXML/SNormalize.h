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
 * std
 */
#include <regex>
/**
 * space
 */
#include "SConvertJSON.h"
#include "SConvertXML.h"
#include "SFileStream.h"
#include "SVariable.h"
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
        auto profile = Obj(Map());
        Logic::ForEach(profiles, [&](auto k, Var v) {
            if(std::regex_match(String(file), std::regex(k))) { 
                Logic::MergeEach(v, profile, [](auto, Var v, Var o) {
                    return v;
                }); 
            }
            return v;
        });
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
            [&profile](auto p, Var v) {
                if(Var::IsList(v)) {

                    
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