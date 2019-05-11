/**
 * ------------------------------------------------------------------------------------------------
 * File:   SKeys.h
 * Author: Luis Monteiro
 *
 * Created on Apr 10, 2019, 12:11 PM
 * ------------------------------------------------------------------------------------------------
 */
#ifndef SKEYS_H
#define SKEYS_H
/**
 */
#include "STypes.h"
namespace Keys {
/**
 * ----------------------------------------------------------------------------
 * system resources keys
 * ----------------------------------------------------------------------------
 */
/**
 * ----------------------------------------------------------------------------
 * Objects Properties Keys
 * ----------------------------------------------------------------------------
 */
const auto $path      = Key("$size");
const auto $type      = Key("$size");
const auto $size      = Key("$size");
/**
 * ----------------------------------------------------------------------------
 * Objects Type Keys
 * ----------------------------------------------------------------------------
 */
const auto $time      = Key("$time");
/**
 * ----------------------------------------------------------------------------
 * system actions keys
 * ----------------------------------------------------------------------------
 */
/**
 * ----------------------------------------------------------------------------
 * Logic and Arithmetic Actions Keys
 * ----------------------------------------------------------------------------
 */
const auto $inc         = Key("$inc");
const auto $gt          = Key("$gt");
const auto $gte         = Key("$gte");
const auto $lt          = Key("$lt");
const auto $eq          = Key("$eq");
const auto $ne          = Key("$ne");
const auto $regex       = Key("$regex");
const auto $nin         = Key("$nin");
/**
 * ----------------------------------------------------------------------------
 * Flow Control Keys
 * ----------------------------------------------------------------------------
 */
const auto $select    = Key("$select");
const auto $except    = Key("$except");
const auto $result    = Key("$result");
const auto $next      = Key("$next");
const auto $raise     = Key("$raise");
const auto $all       = Key("$all");
/**
 * ----------------------------------------------------------------------------
 * Exception Keys
 * ----------------------------------------------------------------------------
 */
const auto $exception = Key("$exception");
const auto $code      = Key("$code");
const auto $what      = Key("$what");
const auto &object    = Key("$obj");
}
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
#endif	/* SKEYS_H */
