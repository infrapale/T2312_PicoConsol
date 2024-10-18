#include "SerialUSB.h"
#include "main.h"
#include "dashboard.h"
#include "aio_mqtt.h"
#include "time_func.h"
#include "atask.h"

#define NBR_BOXES           7

typedef enum
{
  BOX_UPPER_LARGE = 0,
  BOX_MID_LARGE,
  BOX_ROW_1,
  BOX_ROW_2,
  BOX_ROW_3,
  BOX_ROW_4,
  BOX_LOW_AREA,
  BOX_NBR_OF
} boxes_et;

typedef struct
{
    bool show_sensor_value;
    bool force_show_big_time;
    bool fast_forward;
    uint8_t sensor_indx;
    uint8_t menu_sensor_indx;
} dashboard_ctrl_st;

dashboard_ctrl_st dashboard_ctrl = {false, true, false, AIO_SUBS_TRE_ID_TEMP, 0};
// extern value_st subs_data[];

extern TFT_eSPI tft;
extern value_st subs_data[AIO_SUBS_NBR_OF];

disp_box_st db_box[BOX_NBR_OF] =
{
  // x    y    w    h   label  fon f  fill color  border color  text color
  {  0,   0, 319, 172, "Box 0", 4, 1, TFT_BLACK, TFT_LIGHTGREY, TFT_LIGHTGREY },
  {  0,  72, 319,  80, "Box 1", 8, 1, TFT_BLACK, TFT_GOLD, TFT_GOLD },
  {  0,   0, 319,  32, "Box 2", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE },
  {  0,  32, 319,  32, "Box 3", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE },
  {  0,  64, 319,  32, "Box 4", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0,  96, 319,  32, "Box 5", 4, 1, TFT_BLACK, TFT_GOLD, TFT_WHITE},
  {  0,  90, 319,  90, "Box 6", 8, 1, TFT_BLACK, TFT_VIOLET, TFT_GOLD },
};



char zone_main_label[NBR_MAIN_ZONES][MAIN_ZONE_LABEL_LEN] =
{ 
  // 01234567890123456789
    "Villa Astrid   ",
    "Lilla Astrid   ",
    "Laituri        ",
    "Tampere        ",
    "Ruuvi          "    
};


char unit_label[UNIT_NBR_OF][UNIT_LABEL_LEN] =
{
  // 012345678
    "Celsius ",
    "%       ",
    "kPa     ",
    "Lux     ",
    "LDR     ",
    "V       "
};

char measure_label[UNIT_NBR_OF][MEASURE_LABEL_LEN] =
{
  // 0123456789012345
    "Temperature    ",
    "Humidity       ",
    "Air Pressure   ",
    "Light          ",
    "LDR Value      ",
    "Voltage        "
};


//                                          123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st dashboard_task_handle        =   {"Dashboard SM   ", 1000,   0,     0,  255,    0,   1,  dashboard_update_task };


void dashboard_initialize(void)
{
    atask_add_new(&dashboard_task_handle);
}

void dashboard_draw_box(uint8_t bindx)
{
    tft.setTextSize(db_box[bindx].font_size);
    tft.setTextColor(db_box[bindx].text_color, db_box[bindx].fill_color, false);
    tft.fillRect(db_box[bindx].x_pos, db_box[bindx].y_pos, db_box[bindx].width, db_box[bindx].height, db_box[bindx].fill_color);
    tft.drawString( db_box[bindx].txt , db_box[bindx].x_pos+4, db_box[bindx].y_pos+2, db_box[bindx].font_indx);
    //Serial.print("Box: "); Serial.print(bindx); Serial.print(" = ");Serial.println(db_box[bindx].txt);
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
    static uint8_t prev_minute = 99;
    DateTime *now = time_get_time_now();
    if ((now->minute() != prev_minute) || dashboard_ctrl.force_show_big_time)
    {
        prev_minute = now->minute();
        char s1[4];
        
        db_box[BOX_ROW_3].txt[0] = 0x00;
        dashboard_draw_box(BOX_ROW_3);
    
        sprintf(s1,"%02d",now->hour());
        String time_str = s1;
        time_str += ":";
        sprintf(s1,"%02d",now->minute());
        time_str += s1;
        time_str.toCharArray(db_box[BOX_MID_LARGE].txt, TXT_LEN);
        dashboard_draw_box(BOX_MID_LARGE);
    }
}

void dashboard_show_info(void)
{
    String Str_info = APP_NAME;
    Str_info += "\n";
    Str_info += __DATE__;
    Str_info += __TIME__;

    strcpy(db_box[BOX_UPPER_LARGE].txt, " ");
    dashboard_draw_box(BOX_UPPER_LARGE);
 
    strcpy(db_box[BOX_ROW_1].txt, APP_NAME);
    dashboard_draw_box(BOX_ROW_1);

    strcpy(db_box[BOX_ROW_2].txt, __DATE__);
    dashboard_draw_box(BOX_ROW_2);

    strcpy(db_box[BOX_ROW_3].txt, __TIME__);
    dashboard_draw_box(BOX_ROW_3);

    // Str_info.toCharArray(db_box[0].txt, TXT_LEN);

}

void dashboard_show_common(void)
{
    String time_str;
    if (!dashboard_ctrl.show_sensor_value)  
    {
        strcpy(db_box[BOX_ROW_1].txt, MAIN_TITLE);
        dashboard_draw_box(BOX_ROW_1);
        time_to_string(&time_str);
        time_str.toCharArray(db_box[BOX_ROW_2].txt, TXT_LEN);
        dashboard_draw_box(BOX_ROW_2);
    }
}

void dashboard_clear(void)
{

}
void dashboard_update_task()
{
    static uint32_t next_step_ms;
    bool            update_box;
    String          Str;
    uint8_t         i; 
    
    switch (dashboard_task_handle.state)
    {
        case 0:
            dashboard_show_info();
            dashboard_task_handle.state++;
            break;
        case 1:                
            dashboard_show_common();
            dashboard_big_time();
            dashboard_ctrl.force_show_big_time = false;
            dashboard_task_handle.state++;
            break;
        case 2:
            update_box = false;
            
            i = (uint8_t)dashboard_ctrl.sensor_indx;
            if (millis() > subs_data[i].show_next_ms)
            {
                if ( subs_data[i].updated)
                {
                    dashboard_ctrl.show_sensor_value = true;
                    Serial.print("aio index: "); Serial.print(i); 
                    Serial.println(" = Updated ");
                    //subs_data[i].updated = false;
                    Str = zone_main_label[subs_data[i].main_zone_index];
                    Str += " ";
                    Str += subs_data[i].sub_zone;
                    Str.toCharArray(db_box[BOX_ROW_1].txt,40);

                    Str = measure_label[subs_data[i].unit_index];
                    Str += " ";
                    Str += unit_label[subs_data[i].unit_index];
                    Str.toCharArray(db_box[BOX_ROW_2].txt, TXT_LEN);

                    Str = String(subs_data[i].value);
                    Serial.println(Str);
                    Str.toCharArray(db_box[BOX_MID_LARGE].txt,6);
                    update_box = true;
                    if (update_box)
                    {
                        dashboard_draw_box(BOX_UPPER_LARGE);
                        dashboard_draw_box(BOX_MID_LARGE);
                        dashboard_draw_box(BOX_ROW_1);
                        dashboard_draw_box(BOX_ROW_2);
                    }
                }
                subs_data[i].show_next_ms = millis() + subs_data[i].show_interval_ms;
            }

            if (dashboard_ctrl.sensor_indx < AIO_SUBS_NBR_OF - 1) dashboard_ctrl.sensor_indx++;    
            else dashboard_ctrl.sensor_indx = AIO_SUBS_TRE_ID_TEMP;
            
            if (update_box )
            {
                dashboard_task_handle.state = 3;
                next_step_ms = millis() + 10000;
            }
            else
            {
               dashboard_task_handle.state = 1;
            }
            break;  
        case 3:
            if ((millis() > next_step_ms) || dashboard_ctrl.fast_forward)
            {
                dashboard_ctrl.force_show_big_time = true;
                dashboard_task_handle.state = 1;
                dashboard_ctrl.show_sensor_value = false;
                dashboard_ctrl.fast_forward = false;
            } 
              
            break;
    }
    //Serial.printf("db %d -> %d\n", dashboard_task_handle.prev_state, dashboard_task_handle.state);
}

void dashboard_next_sensor(void)
{
    dashboard_ctrl.menu_sensor_indx++;
    if(dashboard_ctrl.menu_sensor_indx >= AIO_SUBS_NBR_OF) dashboard_ctrl.menu_sensor_indx = AIO_SUBS_TRE_ID_TEMP;
    subs_data[dashboard_ctrl.menu_sensor_indx].show_next_ms = 0              ;
    dashboard_ctrl.sensor_indx = dashboard_ctrl.menu_sensor_indx;
    Serial.printf("dashboard_ctrl.menu_sensor_indx=%d\n",dashboard_ctrl.menu_sensor_indx);
    dashboard_ctrl.fast_forward = true;
}

void dashboard_previous_sensor(void)
{
    if(dashboard_ctrl.menu_sensor_indx <= 1 ) dashboard_ctrl.menu_sensor_indx = AIO_SUBS_NBR_OF -1;
    else dashboard_ctrl.menu_sensor_indx--;
    subs_data[dashboard_ctrl.menu_sensor_indx].show_next_ms = 0;
    dashboard_ctrl.sensor_indx = dashboard_ctrl.menu_sensor_indx;
    Serial.printf("dashboard_ctrl.menu_sensor_indx=%d\n",dashboard_ctrl.menu_sensor_indx);
    dashboard_ctrl.fast_forward = true;
}