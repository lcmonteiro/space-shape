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
    /**
     * inport each file
     */
    for(auto file : Find(out, profile["p"])) {

    }
    return 0;
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* IMPORT_H */