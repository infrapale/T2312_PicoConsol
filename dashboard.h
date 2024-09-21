#ifndef __DASHBOARD_H__
#define __DASHBOARD_H__

#include "main.h"
#include <TFT_eSPI.h> 

void dashboard_initialize(void);

void dashboard_draw_box(uint8_t bindx);

void dashboard_update_all(void);

void dashboard_set_text(uint8_t box_indx, char *txt_ptr);

void dashboard_update_task();


#endif