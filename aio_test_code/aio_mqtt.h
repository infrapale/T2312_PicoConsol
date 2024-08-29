#ifndef __AIO_MQTT_H__
#define __AIO_MQTT_H__
#include "Arduino.h"

typedef enum 
{
  AIO_SUBS_TRE_ID_TEMP = 0, 
  AIO_SUBS_TRE_ID_HUM,   
  AIO_SUBS_NBR_OF
} aio_subs_et;

typedef enum 
{
  AIO_PUBL_VA_HOME_MODE = 0,
  AIO_PUBL_VA_AC_TEMP,
  AIO_PUBL_NBR_OF
} aio_publ_et;



void mqtt_initialize(void);

bool mqtt_connect(void);

void mqtt_subscribe(void);

void mqtt_read_subscriptions(void);

void mqtt_check_connection(void);

#endif