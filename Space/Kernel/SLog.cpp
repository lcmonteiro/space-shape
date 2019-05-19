/**
 * ------------------------------------------------------------------------------------------------
 * File:   SLog.cpp
 * Author: Luis Monteiro
 * 
 * Created on October 14, 2015, 2:29 PM
 * ------------------------------------------------------------------------------------------------
 */
#include "SLog.h"
/**
 * --------------------------------------------------------
 * Initialization
 * --------------------------------------------------------
 * log level
 */
SLog::LEVEL SLog::__level    = SLog::DEBUG;
/**
 * output streams 
 */
static SNullStream __ns;
std::ostream SLog::__debug   = ns;
std::ostream SLog::__info    = ns;
std::ostream SLog::__warning = ns;
std::ostream SLog::__error   = ns;
std::ostream SLog::__null    = ns;
/**
* atomicity
*/
std::mutex __mutex;
/**
 * ------------------------------------------------------------------------------------------------
 * End
 * ------------------------------------------------------------------------------------------------
 */
