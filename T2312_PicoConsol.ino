/************************************************************************************************************************************
@title T2312_PicoConsol.ino
@git  	https://github.com/infrapale/T2312_PicoConsol
************************************************************************************************************************************
@HW Raspberry Pi Pico W on a pico TFT classig board

************************************************************************************************************************************
https://github.com/adafruit/Adafruit_MQTT_Library
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial-using-semaphore-and-mutex-in-freertos-with-arduino
https://learn.adafruit.com/dvi-io/code-the-dashboard
***********************************************************************************************************************************/


#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include "main.h"
#include "io.h"
#include "menu.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "secrets.h"
#include <Wire.h>
#include "RTClib.h"
#include "time_func.h"
#include "atask.h"
#include "aio_mqtt.h"
#include "log.h"
#include "dashboard.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


void print_debug_task(void)
{
  atask_print_status(true);
}

//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st debug_task_handle    =   {"Debug Task     ", 30000,    0,     0,  255,    0,  1,  print_debug_task };


uint32_t  targetTime = 0; 

void setup(void) {
  delay(3000);
  // while (!Serial);
  Serial.begin(115200); // For debug
  Serial.println(F("T2312_PicoConsol.ino"));
  pinMode(TFT_BL, OUTPUT);  
  digitalWrite(TFT_BL, HIGH);

  Wire.setSDA(PIN_WIRE_SDA);
  Wire.setSCL(PIN_WIRE_SCL);
  Wire.begin();
  time_begin();

  
  atask_initialize();
  atask_add_new(&debug_task_handle);
  
  dashboard_initialize();   // start dashboard task
  menu_initialize();        // starting scan and read tasks
  aio_mqtt_initialize();    // task is stopped - for debug purpose only
}

void setup1()
{
  //Watchdog.reset();
  aio_mqtt_initialize();
  targetTime = millis() + 100;
}

// Fast running loop
void loop() 
{
  atask_run();
}

// Slow running loop: WiFI an MQTT
void loop1()
{
  if (millis() > targetTime)
  {
    aio_mqtt_stm();
    targetTime = millis() + 100;
  }
}