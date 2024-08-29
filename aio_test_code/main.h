#ifndef __MAIN_H__
#define __MAIN_H__
#define BOARD_PICO_TFT_4KEYS

#include <board.h>
//#include <FreeRTOS.h>
#include <task.h>
//#include <semphr.h>

// the on off button feed turns this LED on/off
//#define LED_BUILTIN 25  
// the slider feed sets the PWM output of this pin

#define LABEL_LEN   12
#define UNIT_LEN    6
#define TXT_LEN     40

#define APP_NAME   "T2312_PicoConsole"
#define MAIN_TITLE "Villa Astrid Console"

typedef enum
{
    TASK_SCAN_KEY   = 0,
    TASK_DASHBOARD,
    TASK_NBR_OF
} task_et;


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