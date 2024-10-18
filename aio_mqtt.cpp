/**

  https://github.com/adafruit/Adafruit_MQTT_Library
  https://io.adafruit.com/api/docs/mqtt.html#adafruit-io-mqtt-api

 */

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    IO_USERNAME
#define AIO_KEY         IO_KEY
#define AIO_PUBLISH_INTERVAL_ms  60000

#include "main.h"

#include <stdint.h>
#include "stdio.h"
#include "pico/stdlib.h"
//#include "hardware/uart.h"
#include "secrets.h"
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <TimeLib.h>
#include "aio_mqtt.h"
#include "atask.h"
#include "time_func.h"
#include "log.h"
#include "dashboard.h"

typedef struct 
{
  uint8_t     state;
  uint8_t     aindx;
  int8_t      connected;
  aio_subs_et subs_indx;
  uint16_t    conn_faults;
  uint8_t     at_home;

} aio_mqtt_ctrl_st;

date_time_st date_time ={0};

aio_mqtt_ctrl_st aio_mqtt_ctrl =
{
  .connected = false,
  .subs_indx = AIO_SUBS_VA_OD_TEMP,
  .conn_faults = 0,
  .at_home = 0,
};


// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;
const char PHOTOCELL_FEED[] PROGMEM = AIO_USERNAME "/feeds/photocell";
const char AIO_FEED_TRE_TEMP_ID[] PROGMEM = AIO_USERNAME "/feeds/home-tampere.tampere-indoor-temperature";


// RTC_PCF8563 rtc;
WiFiClient client;

Adafruit_MQTT_Client aio_mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish villa_astrid_home_mode    = Adafruit_MQTT_Publish(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.astrid-mode");
Adafruit_MQTT_Subscribe villa_astrid_od_temp    = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ulko-temp");
Adafruit_MQTT_Subscribe villa_astrid_od_hum     = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ulko-hum");
Adafruit_MQTT_Subscribe tre_id_temp_feed        = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_FEED_TRE_TEMP_ID);
Adafruit_MQTT_Subscribe tre_id_hum_feed         = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/home-tampere.tre-indoor-humidity");
Adafruit_MQTT_Subscribe lilla_astrid_id_temp    = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/lillaastrid.studio-temp");
Adafruit_MQTT_Subscribe ruuvi_e6_temp           = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ruuvi-e6");
Adafruit_MQTT_Subscribe ruuvi_ea_temp           = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ruuvi-ea");
Adafruit_MQTT_Subscribe ruuvi_ed_temp           = Adafruit_MQTT_Subscribe(&aio_mqtt, AIO_USERNAME "/feeds/villaastrid.ruuvi-ed");

Adafruit_MQTT_Subscribe timefeed                = Adafruit_MQTT_Subscribe(&aio_mqtt, "time/seconds");
//Adafruit_MQTT_Subscribe timefeed                = Adafruit_MQTT_Subscribe(&aio_mqtt, "time/ISO-8601");


//infrapale/feeds/home-tampere.tampere-indoor-temperature

Adafruit_MQTT_Subscribe *aio_subs[AIO_SUBS_NBR_OF] =
{
  [AIO_SUBS_TIME]           = &timefeed,
  [AIO_SUBS_TRE_ID_TEMP]    = &tre_id_temp_feed,
  [AIO_SUBS_TRE_ID_HUM]     = &tre_id_hum_feed,
  [AIO_SUBS_LA_ID_TEMP]     = &lilla_astrid_id_temp,
  [AIO_SUBS_VA_OD_TEMP]     = &villa_astrid_od_temp,
  [AIO_SUBS_VA_OD_HUM]      = &villa_astrid_od_hum,
  [AIO_SUBS_RUUVI_E6_TEMP]  = &ruuvi_e6_temp,
  [AIO_SUBS_RUUVI_EA_TEMP]  = &ruuvi_ea_temp,
  [AIO_SUBS_RUUVI_ED_TEMP]  = &ruuvi_ed_temp,
};

Adafruit_MQTT_Publish *aio_publ[AIO_PUBL_NBR_OF] =
{
  [AIO_PUBL_VA_HOME_MODE] = &villa_astrid_home_mode,
  [AIO_PUBL_VA_AC_TEMP]  = &villa_astrid_home_mode
};

void dummy_mqtt_cb(void){}
void cb_time_feed(uint32_t feed_time);

value_st subs_data[AIO_SUBS_NBR_OF]
{
  [AIO_SUBS_TIME]           = {ZONE_TAMPERE, "ID ",  UNIT_TEMPERATURE, 0.0, true, false, 60000, 0},
  [AIO_SUBS_TRE_ID_TEMP]    = {ZONE_TAMPERE, "ID ",  UNIT_TEMPERATURE, 0.0, true, false, 120000, 0},
  [AIO_SUBS_TRE_ID_HUM]     = {ZONE_TAMPERE, "ID ",  UNIT_HUMIDITY, 0.0, true, false, 300000, 0},
  [AIO_SUBS_LA_ID_TEMP]     = {ZONE_LILLA_ASTRID, "ID ",  UNIT_TEMPERATURE, 0.0, true, false, 120000, 0},
  [AIO_SUBS_VA_OD_TEMP]     = {ZONE_VILLA_ASTRID, "OD ",  UNIT_TEMPERATURE, 0.0,true, false, 60000, 0},
  [AIO_SUBS_VA_OD_HUM]      = {ZONE_VILLA_ASTRID, "OD ",  UNIT_HUMIDITY, 0.0, true, false, 120000, 0},
  [AIO_SUBS_RUUVI_E6_TEMP]  = {ZONE_RUUVI, "XX ",  UNIT_TEMPERATURE, 0.0, true, false, 60000, 0},
  [AIO_SUBS_RUUVI_EA_TEMP]  = {ZONE_RUUVI, "XX ",  UNIT_TEMPERATURE, 0.0, true, false, 60000, 0},
  [AIO_SUBS_RUUVI_ED_TEMP]  = {ZONE_RUUVI, "XX ",  UNIT_TEMPERATURE, 0.0, true, false, 60000, 0},
};



extern Adafruit_MQTT_Subscribe *aio_subs[];
extern Adafruit_MQTT_Publish *aio_publ[];
extern Adafruit_MQTT_Client aio_mqtt;
extern char zone_main_label[NBR_MAIN_ZONES][MAIN_ZONE_LABEL_LEN];
extern char unit_label[UNIT_NBR_OF][UNIT_LABEL_LEN];
extern char measure_label[UNIT_NBR_OF][MEASURE_LABEL_LEN];


//                                  123456789012345   ival  next  state  prev  cntr flag  call backup
atask_st aio_mqtt_task        =   {"AIO MQTT SM    ", 1000,   0,     0,  255,    0,   0, aio_mqtt_stm };


Adafruit_MQTT_Subscribe *aio_subscription;

void aio_mqtt_initialize(void)
{
    //aio_mqtt_ctrl.aindx = atask_add_new(&aio_mqtt_task);   // do not actually run as a task
    aio_mqtt_task.state = 0;
    aio_mqtt_task.prev_state = 255;
}



int8_t aio_mqtt_connect() {
    Serial.print(F("Connecting to Adafruit IO… "));
    int8_t ret;
    if ((ret = aio_mqtt.connect()) != 0) 
    {
        switch (ret) 
        {
            case 1: Serial.println(F("Wrong protocol")); break;
            case 2: Serial.println(F("ID rejected")); break;
            case 3: Serial.println(F("Server unavail")); break;
            case 4: Serial.println(F("Bad user/pass")); break;
            case 5: Serial.println(F("Not authed")); break;
            case 6: Serial.println(F("Failed to subscribe")); break;
            default: Serial.println(F("Connection failed")); break;
        }

        if(ret >= 0) aio_mqtt.disconnect();
        Serial.println(F("Retrying connection…"));
        aio_mqtt_ctrl.conn_faults++;
        // Watchdog.reset();
    }
    else 
    {
      Serial.println(F("Adafruit IO Connected!"));
    }
    
    return ret;
}



void print_subs_data(uint8_t subs_indx)
{
    Serial.print(aio_subs[subs_indx]->topic);
    Serial.print(" = ");
    Serial.println((char*)aio_subs[subs_indx]->lastread);
}

void save_subs_float_data(uint8_t subs_indx)
{
    String f_str = String((char*)aio_subs[subs_indx]->lastread);
    subs_data[subs_indx].value = f_str.toFloat();
    //Serial.print("float= "); Serial.print(f_str); Serial.print(" -> "); Serial.println(subs_data[subs_indx].value);
    subs_data[subs_indx].updated = true;
    print_subs_data(subs_indx);
}

void cb_time(uint32_t feed_time) 
{
    uint32_t tz_adjusted;  
    uint32_t offset_days = 3 * 86400;
    uint32_t unix_date2;
    uint32_t epoc_time;

    epoc_time = feed_time;
    unix_date2 = 1564398600 + offset_days;
    Serial.printf("Date1: %4d-%02d-%02d %02d:%02d:%02d\n", 
        year(epoc_time), month(epoc_time), day(epoc_time), 
        hour(epoc_time), minute(epoc_time), second(epoc_time));
    // adjust to local time zone
    tz_adjusted = feed_time + (TIME_ZONE_OFFS * 60 * 60);
    time_set_epoc_time(tz_adjusted);
    uint8_t sindx = AIO_SUBS_TIME;
    subs_data[sindx].updated = true;
    print_subs_data(sindx);    
}

void cb_tre_id_temp(double tmp)
{
    save_subs_float_data(AIO_SUBS_TRE_ID_TEMP);
}

void cb_tre_id_hum(double tmp)
{
    save_subs_float_data(AIO_SUBS_TRE_ID_HUM);
}

void cb_lilla_astrid_id_temp(double tmp)
{
    save_subs_float_data(AIO_SUBS_LA_ID_TEMP);
}

void cb_ruuvi_e6_temp(double tmp)
{
    save_subs_float_data(AIO_SUBS_RUUVI_E6_TEMP);
}

void cb_ruuvi_ea_temp(double tmp)
{
    save_subs_float_data(AIO_SUBS_RUUVI_EA_TEMP);
}

void cb_ruuvi_ed_temp(double tmp)
{
    save_subs_float_data(AIO_SUBS_RUUVI_ED_TEMP);
}


void cb_dummy(double tmp) {}


void activate_subscriptions(void)
{
    // unsubscribe -> subscribe is currently not supported
    for (uint8_t subs_indx= 0; subs_indx <  AIO_SUBS_NBR_OF; subs_indx++)
    {
        if( (subs_data[subs_indx].show_interval_ms > 0) && subs_data[subs_indx].active)
        {
            aio_mqtt.subscribe( aio_subs[subs_indx]);
            Serial.printf("Subscribe to #%d: %s\n", subs_indx, aio_subs[subs_indx]->topic ); 
        }
    }
}

void aio_mqtt_stm(void)
{
    if ( aio_mqtt_task.prev_state != aio_mqtt_task.state)
    {
        Serial.print(F("aio_mqtt_stm state= "));
        Serial.print(aio_mqtt_task.prev_state);
        Serial.print(F(" --> "));
        Serial.println(aio_mqtt_task.state);
        aio_mqtt_task.prev_state = aio_mqtt_task.state;
    }

    switch(aio_mqtt_task.state)
    {
        case 0:
          aio_mqtt_task.state = 10;
          break;
        case 10:
            WiFi.begin(WIFI_SSID, WIFI_PASS);
            Serial.println(F("setup - wifi begin .. "));
            if (WiFi.status() != WL_CONNECTED)  Serial.print(F("."));
            else 
            {
              Serial.println(F("wifi connected"));
              aio_mqtt_task.state = 20;
            }
            break;
        case 20:
            tre_id_temp_feed.setCallback(cb_tre_id_temp);
            tre_id_hum_feed.setCallback(cb_tre_id_hum);
            lilla_astrid_id_temp.setCallback(cb_lilla_astrid_id_temp);
            villa_astrid_od_temp.setCallback(cb_dummy);
            villa_astrid_od_hum.setCallback(cb_dummy);
            ruuvi_e6_temp.setCallback(cb_ruuvi_e6_temp);
            ruuvi_ea_temp.setCallback(cb_ruuvi_ea_temp);
            ruuvi_ed_temp.setCallback(cb_ruuvi_ed_temp);
            timefeed.setCallback(cb_time);
            aio_mqtt_task.state = 30;
            break;
        case 30:
            activate_subscriptions();
            aio_mqtt_task.state = 40;
            break;
        case 40:
            aio_mqtt_ctrl.connected =  aio_mqtt_connect();
            if (aio_mqtt_ctrl.connected == 0) 
            {
              aio_mqtt_task.state = 50;
              aio_mqtt_ctrl.conn_faults = 0;
            }
            break;
        case 50:
            aio_mqtt.processPackets(10000);
            aio_mqtt_task.state = 55;
            break;
        case 55:
            if(! aio_mqtt.ping()) 
            {
              aio_mqtt.disconnect();
              aio_mqtt_task.state = 10;
            }
            else 
            {
              if (subs_data[AIO_SUBS_TIME].updated)
              {
                  aio_mqtt_task.state = 60;
              }
            }
            break;
        case 60:
            aio_mqtt.unsubscribe( aio_subs[AIO_SUBS_TIME]);
            aio_mqtt.disconnect();
            aio_mqtt_task.state = 100;
            break;
        case 100:
            aio_mqtt_ctrl.connected =  aio_mqtt_connect();
            if (aio_mqtt_ctrl.connected == 0) 
            {
              aio_mqtt_task.state = 105;
            }
            break;
        case 105:
            aio_mqtt_task.state = 110;
            aio_mqtt_ctrl.conn_faults = 0;
            break;
        case 110:
            aio_mqtt.processPackets(10000);
            aio_mqtt_task.state = 120;
            break;
        case 120:
            if(! aio_mqtt.ping()) 
            {
              aio_mqtt.disconnect();
              aio_mqtt_task.state = 100;
            }
            else 
            {
              aio_mqtt_task.state = 110;
            }
            break;
    }
}
