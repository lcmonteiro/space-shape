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
#include "SFind.h"
/**
 * ----------------------------------------------------------------------------
 * Normalize XML
 * ----------------------------------------------------------------------------
 */
static inline int Normalize(String in, String filter, Var profile) {
    /**
     * normalize each file
     */
    for(Var file : Find(in, filter)) {
        INFO("Normalize", "file= " << String(file));  
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
#endif	/* NORMALIZE_H */