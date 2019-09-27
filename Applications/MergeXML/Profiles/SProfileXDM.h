/**
 * ------------------------------------------------------------------------------------------------
 * File:   profileXDM.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef PROFILE_XDM_H
#define PROFILE_XDM_H
/**
 * space
 */
#include "SVariable.h"
/**
 * ----------------------------------------------------------------------------
 * Profile - Sort
 * ----------------------------------------------------------------------------
 */
Var XDM_SORT = Obj{
    Obj{
        Obj("*"), Obj{
            Obj("#/name"),
            Obj("#/GID")
        }
    }
};
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* PROFILE_XDM_H */