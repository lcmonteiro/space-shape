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
Var GetProfile(String file) {
    return Edit::Insert(Convert::FromJson(File::Reader(file)), Obj{
        {Key("*.arxml"), PROFILE_ARXML}
    });
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* PROFILES_H */