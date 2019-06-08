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
#include "SSearch.h"
#include "SFind.h"
/**
 * namespace
 */
using namespace Keys;
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
    for(Var file : Find(in, profile["pattern"])) {
        Logic::ForEach(Convert::FromXML(File::Reader(file)), [&](const List& p, Var v) {
            if(Search::Match(Obj{{$regex, profile["select"]}}, Obj(Convert::ToPath(p)))){
                DEBUG("v", v);
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