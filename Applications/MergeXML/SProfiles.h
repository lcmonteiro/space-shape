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
/**
 * ----------------------------------------------------------------------------
 * Get Profile
 * ----------------------------------------------------------------------------
 */
Var GetProfiles(String file) {
    return Edit::Insert(Convert::FromJson(File::Reader(file)), Obj{
        {Key("*.arxml"), PROFILE_ARXML}
    });
}
/**
 * ----------------------------------------------------------------------------
 * Get Profile
 * ----------------------------------------------------------------------------
 */
List GetProfile(String file) {
    return Var::List(Map({
        {Key("arxml"), PROFILE_ARXML_L}
    })[FileSystem::GetExtension(file)]);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* PROFILES_H */