#ifndef __TIME_FUNC_H__
#define __TIME_FUNC_H__

#include "Arduino.h"
#include "RTClib.h"

#define TIME_ZONE_OFFS        3
#define TIME_ZONE_OFSET_SEC   (TIME_ZONE_OFFS * 3600)

bool time_begin(void);
void time_lost_power(bool force_new_time);
DateTime *time_get_time_now(void);
uint32_t time_get_epoc_time(void);
void time_set_epoc_time(uint32_t epoc_time);
void time_to_string(String *Str);
void time_print(void);

#endif