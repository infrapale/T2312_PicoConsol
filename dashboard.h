#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__

#include "main.h"
#include <TFT_eSPI.h> 

#define NBR_MAIN_ZONES      4
#define NBR_SUB_ZONES       20
#define NBR_UNITS           5
#define MAIN_ZONE_LABEL_LEN 16
#define SUB_ZONE_LABEL_LEN  20
#define UNIT_LABEL_LEN      10
#define MEASURE_LABEL_LEN   16

typedef enum
{
    ZONE_VILLA_ASTRID = 0,
    ZONE_LILLA_ASTRID,
    ZONE_LAITURI,
    ZONE_TAMPERE
} main_zone_et;

typedef enum
{
    UNIT_TEMPERATURE = 0,
    UNIT_HUMIDITY,
    UNIT_AIR_PRESSURE,
    UNIT_LIGHT,
    UNIT_LDR
} unit_et;

typedef struct
{
  main_zone_et  main_zone_index;
  char          sub_zone[SUB_ZONE_LABEL_LEN];
  unit_et       unit_index;
  float         value ;
  bool          updated;
} value_st;


void dashboard_draw_box(uint8_t bindx);

void dashboard_update_all(void);

void dashboard_set_text(uint8_t box_indx, char *txt_ptr);

void dashboard_update_task(void *param);


#endif