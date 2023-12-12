#ifndef __MAIN_H__
#define __MAIN_H__
#define BOARD_PICO_TFT_4KEYS
#include <board.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

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

#endif