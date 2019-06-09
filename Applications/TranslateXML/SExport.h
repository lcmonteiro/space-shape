/**
 * ------------------------------------------------------------------------------------------------
 * File:   SExport.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef EXPORT_H
#define EXPORT_H
/**
 * space
 */
#include "SConvertJson.h"
#include "SConvertXml.h"
#include "SConvert.h"
#include "SVariable.h"
#include "SFind.h"
/**
 * ----------------------------------------------------------------------------
 * Export text to translate
 * ----------------------------------------------------------------------------
 */
static inline int Export(String in, Var profile, String out) {
    auto container = List();
    /**
     * export each file
     */
    for(Var file : Find(in, profile["file"])) {
        Logic::ForEach(Convert::FromXML(File::Reader(file)), [&](const List& p, Var v) {
            /**
             * select document
             */
            if(std::regex_match(
                Convert::ToPath(p), std::regex(String(profile["select"])))) { 
                /**
                 * match document
                 */
                if(Edit::Match(
                    profile["match"], 
                    Edit::Find(Var::Strip<true>(profile["match"]), v))) {
                    /**
                     * save target
                     */
                    container.emplace_back(Edit::Find(Key(profile["target"]), v));
                }
            }
            return v;
        });
    }
    /**
     * write to file
     */
    Convert::ToJson(File::Writer(out), Obj(std::move(container)));
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* EXPORT_H */