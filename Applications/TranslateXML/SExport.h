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
#include "SConvertCSV.h"
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
    for(auto file : Find(in, profile["p"])) {

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