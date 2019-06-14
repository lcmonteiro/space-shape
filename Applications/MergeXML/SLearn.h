/**
 * ------------------------------------------------------------------------------------------------
 * File:   SLearn.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef LEARN_H
#define LEARN_H
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
 * Export learn XML file 
 * ----------------------------------------------------------------------------
 */
static inline int Learn(String in, String filter, String out) {
    auto profile = Map();
    /**
     * learn each file
     */
    for(Var file : Find(in, filter)) {
        INFO("Learn", "file= " << String(file));
        /**
         * parse file
         */
        Logic::ForEach(Convert::FromXML(File::Reader(file)), [&](const List& p, Var v) -> Var {
            /**
             * find lists
             */
            if(Var::IsList(v)) {
                profile[Var::ToString(p.back())] = v;
                //DEBUG(Convert::ToPath(p), v);
                return Obj(nullptr);
            }
            return v;
        });
    }
    DEBUG("keys", Utils::GetKeys(profile));
    /**
     * write to file
     */
    Convert::ToPrettyJson(File::Writer(out), Obj(std::move(profile)));
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* LEARN_H */