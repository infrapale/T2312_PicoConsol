#ifndef __MAIN_H__
#define __MAIN_H__
#define BOARD_PICO_TFT_4KEYS
#include <board.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#define LABEL_LEN   12
#define UNIT_LEN    6
#define TXT_LEN     40

#define APP_NAME   "T2312_PicoConsol"

typedef enum 
{
  AIO_SUBS_VA_OD_TEMP = 0,
  AIO_SUBS_VA_OD_HUM,
  AIO_SUBS_NBR_OF
} aio_subs_et;

typedef enum 
{
  AIO_PUBL_VA_HOME_MODE = 0,
  AIO_PUBL_VA_AC_TEMP,
  AIO_PUBL_NBR_OF
} aio_publ_et;

typedef struct
{
  char label[LABEL_LEN];
  char unit[UNIT_LEN];
  float value ;
  bool  updated;
} value_st;

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


#endif