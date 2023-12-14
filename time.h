#ifndef __TIME_H__
#define __TIME_H__

#include "Arduino.h"

bool time_begin(void);
void time_lost_power(bool force_new_time);
uint32_t time_get_epoc_time(void);
void time_to_string(String *Str);
void time_print(void);

#endif