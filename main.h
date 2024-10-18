#ifndef __MAIN_H__
#define __MAIN_H__


#define BOARD_PICO_TFT_4KEYS
#define PIRPANA
//#define LILLA_ASTRID
//#define VILLA_ASTRID

#include <board.h>
//#include <FreeRTOS.h>
#include "atask.h"
// #include <semphr.h>

#define LABEL_LEN           12
#define UNIT_LEN            6
#define TXT_LEN             40
#define TIME_ZONE_OFFS      3

#define NBR_MAIN_ZONES      5
#define NBR_SUB_ZONES       20
#define MAIN_ZONE_LABEL_LEN 16
#define SUB_ZONE_LABEL_LEN  20
#define UNIT_LABEL_LEN      10
#define MEASURE_LABEL_LEN   16



#define APP_NAME   "T2312_PicoConsole"
#define MAIN_TITLE "Villa Astrid Console"


typedef enum 
{
  AIO_PUBL_VA_HOME_MODE = 0,
  AIO_PUBL_VA_AC_TEMP,
  AIO_PUBL_NBR_OF
} aio_publ_et;


typedef enum
{
    ZONE_VILLA_ASTRID = 0,
    ZONE_LILLA_ASTRID,
    ZONE_LAITURI,
    ZONE_TAMPERE,
    ZONE_RUUVI
} main_zone_et;

typedef enum
{
    UNIT_TEMPERATURE = 0,
    UNIT_HUMIDITY,
    UNIT_AIR_PRESSURE,
    UNIT_LIGHT,
    UNIT_LDR,
    UNIT_VOLTAGE,
    UNIT_NBR_OF
} unit_et;


typedef struct
{
    uint16_t x_pos;
    uint16_t y_pos;
    uint16_t width;
    uint16_t height;
    char     txt[TXT_LEN];
    uint8_t  font_indx;
    uint8_t  font_size;
    uint16_t fill_color;
    uint16_t border_color;
    uint16_t text_color;

} disp_box_st;

typedef struct date_time
{
    uint16_t  year;
    uint8_t   month;
    uint8_t   day;
    uint8_t   hour;
    uint8_t   minute;
    uint8_t   second;
 } date_time_st;

#endif