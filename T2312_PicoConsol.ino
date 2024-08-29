/**
@title T2312_PicoConsol.ino
@git  	https://github.com/infrapale/T2312_PicoConsol

https://github.com/adafruit/Adafruit_MQTT_Library


https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial1-creating-freertos-task-to-blink-led-in-arduino-uno
https://circuitdigest.com/microcontroller-projects/arduino-freertos-tutorial-using-semaphore-and-mutex-in-freertos-with-arduino

https://learn.adafruit.com/dvi-io/code-the-dashboard



 */

//  This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8
 
 
// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 1000
#define PIRPANA
//#define LILLA_ASTRID
//#define VILLA_ASTRID

#define PIN_WIRE_SDA         (12u)
#define PIN_WIRE_SCL         (13u)

#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include "main.h"
#include "menu.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "secrets.h"
#include <Wire.h>
#include "RTClib.h"
#include "time.h"
#include "aio_mqtt.h"
#include "log.h"
#include "dashboard.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

// WiFi parameters
#define WLAN_SSID       WIFI_SSID
#define WLAN_PASS       WIFI_PASS
WiFiClient client;

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    IO_USERNAME
#define AIO_KEY         IO_KEY
#define AIO_PUBLISH_INTERVAL_ms  60000


// RTC_PCF8563 rtc;

Adafruit_MQTT_Client aio_mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish villa_astrid_home_mode    = Adafruit_MQTT_Publish(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.astrid-mode");
Adafruit_MQTT_Subscribe villa_astrid_od_temp    = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ulko-temp");
Adafruit_MQTT_Subscribe villa_astrid_od_hum     = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ulko-hum");
Adafruit_MQTT_Subscribe tre_id_temp             = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/home-tampere.tampere-indoor-temperature");
Adafruit_MQTT_Subscribe tre_id_hum              = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/home-tampere.tre-indoor-humidity");
//infrapale/feeds/home-tampere.tampere-indoor-temperature

Adafruit_MQTT_Subscribe *aio_subs[AIO_SUBS_NBR_OF] =
{
  [AIO_SUBS_VA_OD_TEMP] = &villa_astrid_od_temp,
  [AIO_SUBS_VA_OD_HUM]  = &villa_astrid_od_hum,
  [AIO_SUBS_TRE_ID_TEMP] = &tre_id_temp,
  [AIO_SUBS_TRE_ID_HUM]  = &tre_id_hum,
};

Adafruit_MQTT_Publish *aio_publ[AIO_PUBL_NBR_OF] =
{
  [AIO_PUBL_VA_HOME_MODE] = &villa_astrid_home_mode,
  [AIO_PUBL_VA_AC_TEMP]  = &villa_astrid_home_mode
};



unsigned long     targetTime = 0; // Used for testing draw times
SemaphoreHandle_t sema_v; 
SemaphoreHandle_t mutex_v;

void setup(void) {
  delay(3000);
  while (!Serial);
  Serial.begin(115200); // For debug
  Serial.println(F("T2311_TFT_Menu.ino"));
  pinMode(TFT_BL, OUTPUT);  
  digitalWrite(TFT_BL, HIGH);

  Wire.setSDA(PIN_WIRE_SDA);
  Wire.setSCL(PIN_WIRE_SCL);
  Wire.begin();
  time_begin();

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.println(F("setup - wifi begin .. "));
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay( 100 );
      Serial.print(F("."));
      Serial.flush();
      //Watchdog.reset();
  }
  //Watchdog.reset();


  mutex_v = xSemaphoreCreateMutex(); 
  if (mutex_v == NULL) 
  { 
    Serial.println("Mutex can not be created"); 
  } 
  
  xTaskCreate(aio_mqtt_stm,"AIO_MQTT",4*4096,nullptr,1,nullptr);
  xTaskCreate(dashboard_update_task,"Dashboard",1024,nullptr,1,nullptr);
  sema_v = xSemaphoreCreateBinary();

  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);

  menu_draw();

  dashboard_draw_box(0);  // clear dashboard
  log_initialize();
  // log_fill_test_data(30);   log_fill_test_data(70);
}

void setup1()
{
  menu_initialize();
  targetTime = millis() + 10;

}

void loop() {
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
  delay(100);
}

void loop1()
{
  if (millis() > targetTime)
  {
    targetTime = millis() + 10;
    menu_button_scan();
  }
}