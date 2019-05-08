/*
 * File:   SKeys.h
 * Author: Luis Monteiro
 *
 * Created on May 18, 2015, 10:40 AM
 */

#ifndef SKEYS_H
#define SKEYS_H
/**
 */
#include "STypes.h"
/**
 * system resources keys
 */
const Key __ZONES_______("zones");
const Key __INSTANCE____("instance");
const Key __MACHINES____("machines");
const Key __MACHINE_____("machine");
const Key __ACTIVITIES__("activities");
const Key __TICKET______("ticket");
const Key __FIRMWARE____("firmware");
const Key __LINKS_______("links");
const Key __RESOURCES___("resources");
const Key __INTERFACE___("interface");
const Key __LOG_________("log");
/**
 */
#define _zones_         __ZONES_______
#define _instance_      __INSTANCE____
#define _machines_      __MACHINES____
#define _machine_       __MACHINE_____
#define _activities_    __ACTIVITIES__
#define _ticket_        __TICKET______
#define _firmware_      __FIRMWARE____
#define _links_         __LINKS_______
#define _resources_     __RESOURCES___
#define _interface_     __INTERFACE___
#define _log_           __LOG_________
/**
 * system properties keys
 */
const Key __ADDRESS_____("address");
const Key __CONFIG______("config");
const Key __COPIES______("copies");
const Key __BACKUPS_____("backups");
const Key __PORT________("port");
const Key __IP__________("ip");
const Key __ID__________("id");
const Key __TYPE________("type");
const Key __SIZE________("size");
const Key __CAPACITY____("capacity");
const Key __FILTER______("filter");
const Key __DATA________("data");
const Key __DOMAIN______("domain");
const Key __PATH________("path");
const Key __URL_________("url");
const Key __MD5_________("md5");
const Key __METHOD______("method");
const Key __TIME________("time");
const Key __RESULT______("result");
const Key __NEW_________("new");
const Key __STATUS______("status");
const Key __STATE_______("state");
const Key __MODEL_______("model");
const Key __REPORT______("report");
const Key __RETRIES_____("retries");
const Key __SLEEP_______("sleep");
const Key __OPTIONS_____("options");
const Key __TRIGGER_____("trigger");
const Key __ACTION______("action");
const Key __LABEL_______("label");
const Key __BASED_______("based");
const Key __PATTERN_____("pattern");
const Key __RANGE_______("range");
const Key __SHARE_______("share");
const Key __PROFILE_____("profile");
const Key __TOLERANCE___("tolerance");
const Key __SIGNATURE___("signature");
const Key __PRIORITY____("priority");
const Key __ERROR_______("error");
/**
 */
#define _address_       __ADDRESS_____
#define _config_        __CONFIG______
#define _copies_        __COPIES______
#define _backups_       __BACKUPS_____
#define _port_          __PORT________
#define _ip_            __IP__________
#define _id_            __ID__________
#define _type_          __TYPE________
#define _size_          __SIZE________
#define _capacity_      __CAPACITY____
#define _filter_        __FILTER______
#define _data_          __DATA________
#define _domain_        __DOMAIN______
#define _path_          __PATH________
#define _url_           __URL_________
#define _md5_           __MD5_________
#define _method_        __METHOD______
#define _time_          __TIME________
#define _result_        __RESULT______
#define _new_           __NEW_________
#define _status_        __STATUS______
#define _state_         __STATE_______
#define _model_         __MODEL_______
#define _report_        __REPORT______
#define _retries_       __RETRIES_____
#define _sleep_         __SLEEP_______
#define _options_       __OPTIONS_____
#define _trigger_       __TRIGGER_____
#define _action_        __ACTION______
#define _label_         __LABEL_______
#define _based_         __BASED_______
#define _pattern_       __PATTERN_____
#define _range_         __RANGE_______
#define _share_         __SHARE_______
#define _profile_       __PROFILE_____
#define _tolerance_     __TOLERANCE___
#define _signature_     __SIGNATURE___
#define _priority_      __PRIORITY____
#define _error_         __ERROR_______
/**
 * system actions keys
 */
const String __EXECUTE__("$execute");
const String __INSERT___("$insert");
const String __UPDATE___("$update");
const String __FIND_____("$find");
const String __VERIFY___("$verify");
const String __REMOVE___("$remove");
const String __COUNT____("$count");
const String __MAP______("$map");
const String __TRIGGER__("$trigger");
const String __PROGRAM__("$program");
const String __MOVE_____("$move");
const String __CLONE____("$clone");
const String __CLEAR____("$clear");
const String __STOP_____("$stop");
const String __SYNC_____("$sync");
const String __START____("$start");
const String __RUN______("$run");
const String __INI______("$ini");
const String __END______("$end");
/**
 */
#define $execute        __EXECUTE__
#define $insert         __INSERT___
#define $update         __UPDATE___
#define $find           __FIND_____
#define $verify         __VERIFY___
#define $remove         __REMOVE___
#define $count          __COUNT____
#define $map            __MAP______
#define $trigger        __TRIGGER__
#define $program        __PROGRAM__
#define $move           __MOVE_____
#define $clone          __CLONE____
#define $clear          __CLEAR____
#define $stop           __STOP_____
#define $sync           __SYNC_____
#define $start          __START____
#define $run            __RUN______
#define $ini            __INI______
#define $end            __END______
/**
 * logic and arithmetic actions keys
 */
const Key __INC______("$inc");
const Key __GT_______("$gt");
const Key __GTE______("$gte");
const Key __LT_______("$lt");
const Key __EQ_______("$eq");
const Key __NE_______("$ne");
const Key __REGEX____("$regex");
const Key __NIN______("$nin");
/**
 */
#define $inc            __INC______
#define $gt             __GT_______
#define $gte            __GTE______
#define $lt             __LT_______
#define $eq             __EQ_______
#define $ne             __NE_______
#define $regex          __REGEX____
#define $nin            __NIN______
/**
 * flow control keys
 */
const Key __IF_______("$if");
const Key __EXCEPT___("$except");
const Key __RETURN___("$return");
const Key __NEXT_____("$next");
const Key __RAISE____("$raise");
const Key __ALL______("$all");
/**
 */
#define $if             __IF_______
#define $except         __EXCEPT___
#define $return         __RETURN___
#define $next           __NEXT_____
#define $raise          __RAISE____
#define $all            __ALL______
/**
 * exception Keys
 */
const Key __EXCEPTION_("exception");
const Key __CODE______("code");
const Key __WHAT______("what");
const Key __OBJECT____("obj");
/**
 */
#define _exception_  "exception"
#define _code_       "code"
#define _what_       "what"
#define _object_     "obj"
/**
 */
#endif	/* SKEYS_H */
