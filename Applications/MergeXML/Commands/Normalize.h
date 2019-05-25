/**
 * ------------------------------------------------------------------------------------------------
 * File:   Normalize.h
 * Author: Luis Monteiro
 *
 * Created on Mai 25, 2019, 14:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef NORMALIZE_H
#define	NORMALIZE_H
/**
 * space
 */
#include "SVariable.h"
/**
 * -----------------------------------------------------------------------------------------------
 * Normalize
 * -----------------------------------------------------------------------------------------------
 */
namespace Command {
    /**
     * Normalize
     * @doc      - document to normalize
     * @settings - normalize settings
     * @return normalized document 
     */
    Var Normalize(Var doc, Var settings);
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* NORMALIZE_H */

