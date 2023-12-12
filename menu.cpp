
#include "main.h"
#include "menu.h"

#include "BtnPinOnOff.h"

#define NBR_MENU_KEYS  4

void dummy_cb()
{
   // 
}

void menu_test1(void)
{
   Serial.printf("Test 1\n");
}

menu_row_st menu[MENU_NBR_OF] =
{
  [MENU_MAIN] =
  {
    "Main Menu", 
    {
      {"Main",MENU_MAIN, dummy_cb },
      {"Time",MENU_TIME, menu_test1 },
      {"Astrid", MENU_VILLA_ASTRID, dummy_cb},
      {"Outd", MENU_OUTDOOR, dummy_cb}
    }
  },
  [MENU_TIME] =
  {
    "Time", 
    {
      {"Main",MENU_MAIN, dummy_cb },
      {"Time",MENU_TIME, dummy_cb},
      {"Set", MENU_TIME, dummy_cb},
      {"Main", MENU_MAIN, dummy_cb}
    }
  },
  [MENU_VILLA_ASTRID] =
  {
    "Villa Astrid", 
    {
      {"Main",MENU_MAIN, dummy_cb },
      {"Indoor",MENU_VILLA_ASTRID, dummy_cb},
      {"Outdoor", MENU_VILLA_ASTRID, dummy_cb},
      {"Main", MENU_MAIN, dummy_cb}
    }
  },
  [MENU_OUTDOOR] =
  {
    "Outdoor Sensors", 
    {
      {"Temp",MENU_MAIN, dummy_cb },
      {"Trend",MENU_VILLA_ASTRID, dummy_cb},
      {"Limit", MENU_VILLA_ASTRID, dummy_cb},
      {"Main", MENU_MAIN, dummy_cb}
    }
  }
};

menu_box_st menu_box[4] =
{
    {   5, 180, 312, 16, NULL },
    {   5, 200, 100, 32, NULL },
    { 110, 200, 100, 32, NULL },
    { 216, 200, 100, 32, NULL},  
};

menu_ctrl_st menu_ctrl =
{
    .active = MENU_MAIN
};

BtnPinOnOff  menu_btn[NBR_MENU_KEYS];

extern TFT_eSPI tft;

void menu_initialize(void)
{
  menu_btn[0].Init(PIN_KEY1,'3', true);
  menu_btn[1].Init(PIN_KEY2,'2', true);
  menu_btn[2].Init(PIN_KEY3,'1', true);
  menu_btn[3].Init(PIN_KEY_STATUS,'0', false);

}


void menu_draw(void)
{

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_MAROON, true);
    tft.fillRect(menu_box[0].x_pos, menu_box[0].y_pos, menu_box[0].width, menu_box[0].height, TFT_MAROON);
    tft.drawString( menu[menu_ctrl.active].row_label, menu_box[0].x_pos+4, menu_box[0].y_pos, 2);

    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_DARKCYAN, true);
    for (uint8_t i = 1; i <= 3; i++)
    {
        tft.fillRoundRect(menu_box[i].x_pos, menu_box[i].y_pos, menu_box[i].width, menu_box[i].height, 5, TFT_DARKCYAN);
        tft.drawString( menu[menu_ctrl.active].menu_item[i].label, menu_box[i].x_pos+4, menu_box[i].y_pos+4, 4);
    }

}

void menu_button_scan(void)
{
    for( uint8_t i= 0; i < NBR_MENU_KEYS; i++)
    {
        menu_btn[i].Scan();
    }
}

char menu_read_button(void)
{
    char c = 0x00;
    for( uint8_t i= 0; i < NBR_MENU_KEYS; i++)
    {
        c = menu_btn[i].Read();
        if (c != 0x00) break;
    }
    return c;
}

void menu_btn_pressed(char pressed)
{
    uint8_t bindx = pressed - '0';
    if ((bindx >= 0) && (bindx <=3))
    {
        Serial.printf("active %d - pressed %d\n", menu_ctrl.active, bindx);
        menu[menu_ctrl.active].menu_item[bindx].cb_func();
        menu_ctrl.active = menu[menu_ctrl.active].menu_item[bindx].next;
        menu_draw();
    }
}
