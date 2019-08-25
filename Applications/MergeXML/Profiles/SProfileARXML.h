/**
 * ------------------------------------------------------------------------------------------------
 * File:   profileARXML.h
 * Author: Luis Monteiro
 *
 * Created on Jun 07, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef PROFILE_ARXML_H
#define PROFILE_ARXML_H
/**
 * space
 */
#include "SVariable.h"
/**
 * ----------------------------------------------------------------------------
 * Profile
 * ----------------------------------------------------------------------------
 */
Var PROFILE_ARXML = Obj{
    {Key("*"), Obj{
        Obj("SHORT-NAME"),
        Obj("#/UUID"),
    }},
    {Key("*-SCALE"), Obj{
        Obj("SHORT-LABEL"),
        Obj("COMPU-CONST/VT")
    }},
    {Key("*-REF"), Obj{
        Obj("#/DEST")
    }},
};

Var PROFILE_ARXML_L = Obj{
    Obj{
        Obj("*"), Obj{
            Obj("SHORT-NAME"),
            Obj("#/UUID")
        }
    },
    Obj{
        Obj("*-SCALE"), Obj{ 
            Obj("SHORT-LABEL"),
            Obj("COMPU-CONST/VT")
        }
    },
    Obj{
        Obj("*-REF"), Obj{
            Obj("#/DEST")
        }
    }
};
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* PROFILE_ARXML_H */