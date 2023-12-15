/**

  https://github.com/adafruit/Adafruit_MQTT_Library

 */

#include "aio_mqtt.h"

#include <stdint.h>
#include "stdio.h"
#include "pico/stdlib.h"
//#include "hardware/uart.h"
#include "main.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "time.h"
#include "log.h"
#include "dashboard.h"

typedef struct 
{
  int8_t  connected;
  uint8_t state;
  uint16_t conn_faults;
  uint8_t at_home;

} aio_mqtt_ctrl_st;

aio_mqtt_ctrl_st aio_mqtt_ctrl =
{
  .connected = false,
  .state = 0,
  .conn_faults = 0,
  .at_home = 0,
};

extern Adafruit_MQTT_Subscribe *aio_subs[];
extern Adafruit_MQTT_Publish *aio_publ[];
extern Adafruit_MQTT_Client aio_mqtt;

extern value_st subs_data[];

Adafruit_MQTT_Subscribe *aio_subscription;



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




void aio_mqtt_stm(void *param)
{
    (void) param;
    uint16_t v_delay_ms = 5000 ;
    String time_str; 
    String value_str;
    float  value;
    uint32_t unix_time;

    // Serial.println(F("aio_mqtt_stm - init"));
    while(true)
    {
      unix_time = time_get_epoc_time();
      // Serial.print(F("aio_mqtt_stm - while "));
      // Serial.println(state);
      switch(aio_mqtt_ctrl.state)
      {
        case 0:
          Serial.println(F("Initializing AIO MQTT"));
          // Serial.println(F("\nWiFi connected"));
          // Serial.println(F("IP address: "));
          // Serial.println(WiFi.localIP());
          aio_mqtt_ctrl.state++;
          break;
        case 1:
          aio_mqtt_ctrl.connected =  aio_mqtt_connect();
          if (aio_mqtt_ctrl.connected == 0) 
          {
            aio_mqtt_ctrl.state++;
            aio_mqtt_ctrl.conn_faults = 0;
          }
          break;
        case 2:   
          Serial.print(F("Subscribe: "));
          Serial.println(aio_subs[AIO_SUBS_VA_OD_TEMP]->topic);

          aio_mqtt.subscribe(aio_subs[AIO_SUBS_VA_OD_TEMP]);
          aio_mqtt_ctrl.state++;
          break;
        case 3:
          Serial.print(F("Read Subscription\n"));
          time_to_string(&time_str);
          Serial.println(time_str);
          while ((aio_subscription = aio_mqtt.readSubscription(500))) 
          {
              Serial.println(aio_subscription->topic);
              for (uint8_t sindx = AIO_SUBS_VA_OD_TEMP; sindx < AIO_SUBS_NBR_OF; sindx++ )
              {
                  if (aio_subscription == aio_subs[sindx]) 
                  {
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
          aio_mqtt_ctrl.state++;
          break;
        case 4:
            if (! aio_publ[AIO_PUBL_VA_HOME_MODE]->publish((float)aio_mqtt_ctrl.at_home)) 
            {
              Serial.println(F("Publish Failed"));
            } else 
            {
              Serial.println(F("Publish OK!"));
            }
            if (aio_mqtt_ctrl.at_home > 0) aio_mqtt_ctrl.at_home = 0;
            else aio_mqtt_ctrl.at_home = 1;
            aio_mqtt_ctrl.state = 1;
          break;
        case 5:
          break;
        case 6:
          break;
        case 7:
          break;
        case 8:
          break;

      }
      vTaskDelay( v_delay_ms / portTICK_PERIOD_MS );
    }
}