/**************************************************************************************************
@title T2312_PicoConsol.ino
@git  	https://github.com/infrapale/T2312_PicoConsol

https://github.com/adafruit/Adafruit_MQTT_Library


https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial-using-semaphore-and-mutex-in-freertos-with-arduino

https://learn.adafruit.com/dvi-io/code-the-dashboard


***************************************************************************************************/

#define PIRPANA
//#define LILLA_ASTRID
//#define VILLA_ASTRID

#define PIN_WIRE_SDA         (12u)
#define PIN_WIRE_SCL         (13u)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>


#include <WiFi.h>
#include <Wire.h>
#include "RTClib.h"

#include "main.h"
#include "my_wifi.h"
#include "aio_mqtt.h"
#include "task.h"
#include "key.h"
#include "menu.h"
#include "dashboard.h"
#include "RTClib.h"
#include "time.h"


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

void dummy_cb(void);

//                        Task               Inter Next Current   Prevs   WD    WD      call
//                        Label              val   ms   State     State   Cntr  Limit   back
task_st menu_btn_handle = {"Scan Menu Butt ",   20,   0,      0,  255,    0,    0,      menu_button_scan };
task_st dashboard_handle = {"Dashboard Task", 1000,   0,      0,  255,    0,    0,      dashboard_update_task };
//task_st scan_key_handle = {"Scan Keypad    ",  500,   0,      0,  255,    0,    0,      key_scan };

extern task_st *task[TASK_RESERVED_NBR];


/*************************** Sketch Code ************************************/
void setup() {
  delay(3000);
  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  delay(10);
  Serial.println(F("T2312_Pico_Consol.ino"));

  pinMode(TFT_BL, OUTPUT);  
  digitalWrite(TFT_BL, HIGH);

  Wire.setSDA(PIN_WIRE_SDA);
  Wire.setSCL(PIN_WIRE_SCL);
  Wire.begin();
  //time_begin();

  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  menu_draw();
  dashboard_draw_box(0);  // clear dashboard

  my_wifi_initialize();

  while (!my_wifi_connect())
  {
    Serial.println(F("Reconnecting to WiFi"));
    delay(5000);
  }
  mqtt_initialize();
  mqtt_subscribe();
}

void setup1()
{
  task_initialize(TASK_NBR_OF);
  task_set_task(TASK_SCAN_KEY, &menu_btn_handle); 
  task_set_task(TASK_DASHBOARD, &dashboard_handle); 
  key_initialize();
  menu_initialize();
}


void loop() {
  mqtt_connect();
  mqtt_read_subscriptions();
  mqtt_check_connection();
}


void loop1()
{
  task_run();
  char c = menu_read_button();
  if (c != 0x00) 
  {
      if ((c & 0b10000000) == 0) 
          Serial.printf("On");
      else 
          Serial.printf("Off");
      Serial.printf(" %c\n",c & 0b01111111);
      c &=  0b01111111;
      menu_btn_pressed(c);
  }


}