/**

  https://github.com/adafruit/Adafruit_MQTT_Library


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
#include "aio_mqtt.h"
#include "atask.h"
#include "time.h"
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
// Adafruit_MQTT_Subscribe timefeed                = Adafruit_MQTT_Subscribe(&aio_mqtt, "time/seconds");
Adafruit_MQTT_Subscribe timefeed                = Adafruit_MQTT_Subscribe(&aio_mqtt, "time/ISO-8601");




//infrapale/feeds/home-tampere.tampere-indoor-temperature

Adafruit_MQTT_Subscribe *aio_subs[AIO_SUBS_NBR_OF] =
{
  [AIO_SUBS_TIME]         = &timefeed,
  [AIO_SUBS_TRE_ID_TEMP]  = &tre_id_temp_feed,
  [AIO_SUBS_TRE_ID_HUM]   = &tre_id_hum_feed,
  [AIO_SUBS_VA_OD_TEMP] = &villa_astrid_od_temp,
  [AIO_SUBS_VA_OD_HUM]  = &villa_astrid_od_hum,
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
  [AIO_SUBS_TIME]         = {ZONE_TAMPERE, "ID ",  UNIT_TEMPERATURE, 0.0, true, false, 0, 0},
  [AIO_SUBS_TRE_ID_TEMP]  = {ZONE_TAMPERE, "ID ",  UNIT_TEMPERATURE, 0.0, true, false, 0, 0},
  [AIO_SUBS_TRE_ID_HUM]   = {ZONE_TAMPERE, "ID ",  UNIT_HUMIDITY, 0.0, true, false, 0, 0},
  [AIO_SUBS_VA_OD_TEMP]   = {ZONE_VILLA_ASTRID, "OD ",  UNIT_TEMPERATURE, 0.0,true, false, 0, 0},
  [AIO_SUBS_VA_OD_HUM]    = {ZONE_VILLA_ASTRID, "OD ",  UNIT_HUMIDITY, 0.0, true, false, 0, 0},
};



extern Adafruit_MQTT_Subscribe *aio_subs[];
extern Adafruit_MQTT_Publish *aio_publ[];
extern Adafruit_MQTT_Client aio_mqtt;
extern char zone_main_label[NBR_MAIN_ZONES][MAIN_ZONE_LABEL_LEN];
extern char unit_label[NBR_UNITS][UNIT_LABEL_LEN];
extern char measure_label[NBR_UNITS][MEASURE_LABEL_LEN];


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

void subs_delay(uint8_t subs_indx)
{
    if( (subs_data[subs_indx].delay > 0) && subs_data[subs_indx].active )
    {
        subs_data[subs_indx].next_subscribe = millis() + subs_data[subs_indx].delay;
        aio_mqtt.unsubscribe( aio_subs[subs_indx]);
        subs_data[subs_indx].active = false;
        Serial.print("Delay #: ");Serial.println(subs_indx);
        

    }
}


void cb_time(uint32_t feed_time) 
{
    uint32_t tz_adjusted;  
    // adjust to local time zone
    tz_adjusted = feed_time + (TIME_ZONE_OFFS * 60 * 60);

    uint8_t sindx = AIO_SUBS_TIME;
    print_subs_data(sindx);
}

void xxcb_time(uint32_t feed_time) 
{
    uint32_t tz_adjusted;  
    // adjust to local time zone
    tz_adjusted = feed_time + (TIME_ZONE_OFFS * 60 * 60);

    // calculate current time
    date_time.second = tz_adjusted % 60;
    tz_adjusted /= 60;
    date_time.minute = tz_adjusted % 60;
    tz_adjusted /= 60;
    date_time.hour = tz_adjusted % 24;

    // print hour
    if(date_time.hour == 0 || date_time.hour == 12)
      Serial.print("12");
    if(date_time.hour < 12)
      Serial.print(date_time.hour);
    else
      Serial.print(date_time.hour - 12);

    // print mins
    Serial.print(":");
    if(date_time.minute < 10) Serial.print("0");
    Serial.print(date_time.minute);

    // print seconds
    Serial.print(":");
    if(date_time.second < 10) Serial.print("0");
    Serial.print(date_time.second);

    if(date_time.hour < 12)
      Serial.println(" am");
    else
      Serial.println(" pm");

  subs_delay(AIO_SUBS_TIME);
}

void cb_tre_id_temp(double tmp)
{
    uint8_t sindx = AIO_SUBS_TRE_ID_TEMP;
    print_subs_data(sindx);
}

void cb_tre_id_hum(double tmp)
{
    uint8_t sindx = AIO_SUBS_TRE_ID_HUM;
    print_subs_data(sindx);
}

void renew_subscriptions( bool force)
{
    for (uint8_t subs_indx= 0; subs_indx <  AIO_SUBS_NBR_OF; subs_indx++)
    {
        bool do_subscribe = false;
        if( (subs_data[subs_indx].delay > 0) && !subs_data[subs_indx].active)
        {
            if(subs_data[subs_indx].next_subscribe < millis()) do_subscribe = true;
        }
        if (do_subscribe || force )
        {
            aio_mqtt.subscribe( aio_subs[subs_indx]);
            subs_data[subs_indx].next_subscribe = millis() + subs_data[subs_indx].delay;
            subs_data[subs_indx].active = true;
            aio_mqtt.subscribe(aio_subs[subs_indx]);
            Serial.print("Subscribe to #: ");Serial.println(subs_indx);
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
            timefeed.setCallback(cb_time);
            aio_mqtt_task.state = 30;
            break;
        case 30:
            renew_subscriptions(true);
            // aio_mqtt.subscribe(&tre_id_temp_feed);
            // aio_mqtt.subscribe(&tre_id_hum_feed);
            // aio_mqtt.subscribe(&timefeed);
            aio_mqtt_task.state = 100;
            break;
        case 100:
            aio_mqtt_ctrl.connected =  aio_mqtt_connect();
            if (aio_mqtt_ctrl.connected == 0) 
            {
              aio_mqtt_task.state = 110;
              aio_mqtt_ctrl.conn_faults = 0;
            }
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
              renew_subscriptions(false);
              aio_mqtt_task.state = 110;
            }

            Serial.print("!!! Time feed: ");
            Serial.print(subs_data[0].active);
            Serial.print(" - ");
            Serial.println(subs_data[0].next_subscribe);
            break;
    }
}

/*
void xxaio_mqtt_stm(void)
{
    String time_str; 
    String value_str;
    float  value;
    uint32_t unix_time;

    // Serial.println(F("aio_mqtt_stm - init"));
    //while(true)
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
          //unix_time = time_get_epoc_time();
          aio_mqtt_task.state = 10;
          break;
        case 10:  
          Serial.println(F("Initializing AIO MQTT"));
          // Serial.println(F("\nWiFi connected"));
          // Serial.println(F("IP address: "));
          // Serial.println(WiFi.localIP());
          aio_mqtt_ctrl.subs_indx = AIO_SUBS_TRE_ID_TEMP;
          aio_mqtt_task.state = 20;
          break;
        case 20:
          aio_mqtt_ctrl.connected =  aio_mqtt_connect();
          if (aio_mqtt_ctrl.connected == 0) 
          {
            aio_mqtt_task.state = 30;
            aio_mqtt_ctrl.conn_faults = 0;
          }
          break;
        case 30:   
          Serial.print(F("Subscribe: "));
          Serial.println(aio_subs[aio_mqtt_ctrl.subs_indx]->topic);
          aio_mqtt.subscribe(aio_subs[AIO_SUBS_TRE_ID_TEMP]);
          aio_mqtt_task.state = 40;
          break;
        case 40:
          Serial.print(F("Read Subscription - "));
          time_to_string(&time_str);
          Serial.print(time_str);
          Serial.print(F(" : "));
          Serial.println(aio_subs[aio_mqtt_ctrl.subs_indx]->topic);
          while ((aio_subscription = aio_mqtt.readSubscription(5000))) 
          {
              Serial.print("O");
              Serial.println(aio_subscription->topic);
              uint8_t sindx = AIO_SUBS_TRE_ID_TEMP;
              // for (uint8_t sindx = AIO_SUBS_TRE_ID_TEMP; sindx < AIO_SUBS_NBR_OF; sindx++ )
              {

                  //if (aio_subscription == aio_subs[sindx]) 
                  {
                      Serial.print(F("!!! Match !!!  aio subs topic: "));
                      Serial.print(aio_subs[sindx]->topic);
                      Serial.print(F(": "));
                      value_str = (char*)aio_subs[sindx]->lastread;
                      Serial.println(value_str);
                      value = value_str.toFloat();
                      log_add_subs_data((aio_subs_et)sindx, unix_time, value);

                      subs_data[sindx].value = value;
                      subs_data[sindx].updated = true;

                      //ctrl.set_temp = atoi((char *)set_temperature.lastread);
                      //Serial.println(ctrl.set_temp);
                  }                  
              }
          }
          /// TODO
          aio_mqtt_ctrl.subs_indx = AIO_SUBS_TRE_ID_TEMP;
          if(! aio_mqtt.ping()) 
          {
            Serial.println("No ping");
            aio_mqtt.disconnect();
            aio_mqtt_task.state = 10;
          }
          else Serial.println("Ping OK");

          // aio_mqtt_ctrl.state++;
          break;
        case 100:
            if (! aio_publ[AIO_PUBL_VA_HOME_MODE]->publish((float)aio_mqtt_ctrl.at_home)) 
            {
              Serial.println(F("Publish Failed"));
            } else 
            {
              Serial.println(F("Publish OK!"));
            }
            if (aio_mqtt_ctrl.at_home > 0) aio_mqtt_ctrl.at_home = 0;
            else aio_mqtt_ctrl.at_home = 1;
            aio_mqtt_task.state = 10;
          break;
        case 500:
          break;
        case 600:
          break;
        default:
          Serial.print(F("AIO_MQTT incorrect state:"));
          Serial.println(aio_mqtt_task.state);
      }
      
      //vTaskDelay( v_delay_ms / portTICK_PERIOD_MS );
    }
}
*/
bool aio_mqtt_is_updated(uint8_t sindx)
{
    return subs_data[sindx].updated;
}
  

void aio_mqtt_get_main_zone(char *carr, uint8_t indx)
{
    size_t  clen = sizeof(carr);
    strcpy(carr, zone_main_label[subs_data[indx].main_zone_index]);
  
  
}

