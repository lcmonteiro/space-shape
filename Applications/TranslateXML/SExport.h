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
        INFO("Export", "file= " << String(file));
        /**
         * parse file
         */
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
                     * verify target
                     */
                    auto target = Edit::Find(Key(profile["target"]), v);
                    if(Var::IsString(target)) {
                        auto& s = Var::String(target);
                        /**
                         * replaces
                         */
                        std::replace(s.begin(), s.end(), '"', '\'');
                        /**
                         * save target
                         */
                        container.emplace_back(target);
                    }
                }
            }
            return v;
        });
    }
    /**
     * write to file
     */
    Convert::ToPrettyJson(File::Writer(out), Obj(std::move(container)));
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* EXPORT_H */