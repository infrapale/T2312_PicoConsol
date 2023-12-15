#include "main.h"
#include "dashboard.h"
#include "time.h"

#define NBR_BOXES  6
extern value_st subs_data[];

extern TFT_eSPI tft;

disp_box_st db_box[NBR_BOXES] =
{
  {  5,   0, 310, 172, "Box 0", 4, 1, TFT_BLACK, TFT_LIGHTGREY, TFT_LIGHTGREY },
  {  5,   0, 310, 172, "Box 1", 7, 2, TFT_NAVY, TFT_GOLD, TFT_RED },
  {  5,   5, 310,  28, "Box 2", 4, 1, TFT_DARKGREY, TFT_GOLD, TFT_WHITE },
  {  5,  50, 310,  28, "Box 3", 4, 1, TFT_BLUE, TFT_SILVER, TFT_SKYBLUE },
  {  5,  95, 310,  28, "Box 4", 4, 1, TFT_DARKGREEN, TFT_GREENYELLOW, TFT_GREENYELLOW },
  {  5, 140, 310,  28, "Box 5", 4, 1, TFT_DARKCYAN, TFT_VIOLET, TFT_ORANGE },
};


void dashboard_draw_box(uint8_t bindx)
{
    tft.setTextSize(db_box[bindx].font_size);
    tft.setTextColor(db_box[bindx].text_color, db_box[bindx].fill_color, false);
    tft.fillRect(db_box[bindx].x_pos, db_box[bindx].y_pos, db_box[bindx].width, db_box[bindx].height, db_box[bindx].fill_color);
    tft.drawString( db_box[bindx].txt , db_box[bindx].x_pos+4, db_box[bindx].y_pos+2, db_box[bindx].font_indx);
}

void dashboard_update_all(void)
{
    for (uint8_t i = 0; i < NBR_BOXES; i++)
    {
        dashboard_draw_box(i);
    }
}

void dashboard_set_text(uint8_t box_indx, char *txt_ptr)
{
    strcpy(db_box[box_indx].txt, txt_ptr);
}

void dashboard_big_time(void)
{
    DateTime *now = time_get_time_now();
    
    String time_str = String(now->hour());
    time_str += now->minute();
    time_str.toCharArray(db_box[1].txt, TXT_LEN);
    Serial.println(db_box[1].txt);
    dashboard_draw_box(1);
}

void dashboard_show_info(void)
{
    String Str_info = APP_NAME;
    Str_info += "\n";
    Str_info += __DATE__;
    Str_info += __TIME__;

    strcpy(db_box[0].txt, " ");
    dashboard_draw_box(0);
 
    strcpy(db_box[2].txt, APP_NAME);
    dashboard_draw_box(2);

    strcpy(db_box[3].txt, __DATE__);
    dashboard_draw_box(3);

    strcpy(db_box[4].txt, __TIME__);
    dashboard_draw_box(4);

    // Str_info.toCharArray(db_box[0].txt, TXT_LEN);

}

void dashboard_clear(void)
{

}
void dashboard_update_task(void *param)
{
    (void) param;
    uint8_t state = 0;
    uint16_t v_delay_ms = 1000;
    uint8_t box_indx;
    String  Str;

    for (;;)
    {

        switch (state)
        {
            case 0:
                dashboard_show_info();
                v_delay_ms = 5000;
                state++;
                break;
            case 1:
                
                dashboard_big_time();
                v_delay_ms = 5000;
                state++;
                break;
            case 2:
                box_indx = 255;
                for (uint8_t i = AIO_SUBS_VA_OD_TEMP; i < AIO_SUBS_NBR_OF; i++ )
                {
                    if (subs_data[i].updated)
                    {
                        subs_data[i].updated = false;
                        switch(i)
                        {
                            case AIO_SUBS_VA_OD_TEMP:
                              box_indx = 2;
                              break;
                            case AIO_SUBS_VA_OD_HUM:
                              box_indx = 3;
                              break;
                        }
                        if (box_indx < NBR_BOXES)
                        {
                            Str = subs_data[i].label;
                            Str += String(subs_data[i].value);
                            Str += subs_data[i].unit;
                            Str.toCharArray(db_box[box_indx].txt, TXT_LEN);
                            dashboard_draw_box(box_indx);

                        }
                    }
                }
                break;

        }
        vTaskDelay( v_delay_ms / portTICK_PERIOD_MS );
    }


}