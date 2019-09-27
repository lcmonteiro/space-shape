/**
 * ------------------------------------------------------------------------------------------------
 * File:   profiles.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef PROFILES_H
#define PROFILES_H
/**
 * space
 */
#include "SConvertJson.h"
#include "SFileStream.h"
#include "SFileSystem.h"
#include "SEdit.h"
/**
 * profiles
 */
#include "Profiles/SProfileARXML.h"
#include "Profiles/SProfileXDM.h"
/**
 * ----------------------------------------------------------------------------
 * Definitions
 * ----------------------------------------------------------------------------
 */
auto KEY_SORT   = Key("sort");
auto KEY_SCHEMA = Key("schema");
/**
 * ----------------------------------------------------------------------------
 * Get Profile
 * ----------------------------------------------------------------------------
 */
Map GetProfiles(String file) {
    return Edit::Insert(Convert::FromJson(File::Reader(file)), Obj{
        {Key("arxml"), Obj{
            {KEY_SORT,   ARXML_SORT  },
            {KEY_SCHEMA, ARXML_SCHEMA},
        }}, 
        {Key("xdm"), Obj{
            {KEY_SORT,   XDM_SORT}
        }}
    });
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* PROFILES_H */