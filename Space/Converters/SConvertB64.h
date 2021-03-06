/**
 * ------------------------------------------------------------------------------------------------
 * File:   B64Convert.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef B64CONVERTER_H
#define B64CONVERTER_H
/**
 * std
 */
#include <string>
/**
 * space
 */
#include "STypes.h"
/**
 * ------------------------------------------------------------------------------------------------
 * Base64 converter
 * ------------------------------------------------------------------------------------------------
 */
typedef class __b64convert__ {
public:
        /**
         * Encode string to Base64
         *
         * @param String op
         *
         * @return
         */
        static String ToBase64(String const& str);
        /**
         * Decode string from Base64
         *
         * @param String str
         *
         * @return
         */
        static String FromBase64(String const& str);
} B64Convert;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif /* B64CONVERTER_H */

