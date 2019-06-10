/**
 * ------------------------------------------------------------------------------------------------
 * File:   SImport.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef IMPORT_H
#define IMPORT_H
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
 * Import translated text
 * ----------------------------------------------------------------------------
 */
static inline int Import(String in, Var profile, String out) {
    auto list = Var::ToList(Convert::FromJson(File::Reader(in)));
    auto it   = list.begin();
    auto end  = list.end();
    /**
     * import each file
     */
    for(Var file : Find(out, profile["file"])) {
        INFO("Import", "file= " << String(file));  
        /**
         * read data
         */
        auto data = Convert::FromXML(File::Reader(file));
        /**
         * translate data
         */
        Logic::ForEach(data, [&](const List& p, Var v) {
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
                     * check target
                     */
                    auto target = Edit::Find(Key(profile["target"]), v);
                    if(Var::IsDefined(target)) {
                        /**
                         * update target
                         */
                        if(it == end) {
                            throw std::runtime_error(String::Build(
                                "mismatch between input(", in, ") and output (", out, ")"));
                        }
                        return Edit::Update(Key(profile["target"]), *it++, v);
                    }
                }
            }
            return v;
        });
        /**
         * write data
         */
        Convert::ToXML(File::Writer(file), data);
    }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* IMPORT_H */