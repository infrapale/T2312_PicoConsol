#include <WiFi.h>
#include "main.h"
#include "my_wifi.h"
#include "Arduino.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "aio_mqtt.h"
#include "secrets.h"



#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    IO_USERNAME
#define AIO_KEY         IO_KEY


typedef struct 
{
    bool connected;
} aio_mqt_st;

aio_mqt_st aio_mqt;

extern WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt_client(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt_client, AIO_USERNAME "/feeds/home-tampere.tre-indoor-humidity");
Adafruit_MQTT_Subscribe slider = Adafruit_MQTT_Subscribe(&mqtt_client, AIO_USERNAME"/feeds/home-tampere.tampere-indoor-temperature");
Adafruit_MQTT_Subscribe tre_id_temp             = Adafruit_MQTT_Subscribe(&mqtt_client, AIO_USERNAME "/feeds/home-tampere.tampere-indoor-temperature");
Adafruit_MQTT_Subscribe tre_id_hum              = Adafruit_MQTT_Subscribe(&mqtt_client, AIO_USERNAME "/feeds/home-tampere.tre-indoor-humidity");

Adafruit_MQTT_Subscribe *aio_subs[AIO_SUBS_NBR_OF] =
{
  [AIO_SUBS_TRE_ID_TEMP] = &tre_id_temp,
  [AIO_SUBS_TRE_ID_HUM]  = &tre_id_hum,
};



void mqtt_initialize(void)
{
  aio_mqt.connected = false;
}

bool mqtt_connect(void)
{
  if (!mqtt_client.connected()) 
  {
    Serial.print(F("Connecting to Adafruit IO… "));
    int8_t ret;
    while ((ret = mqtt_client.connect()) != 0) 
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

        if(ret >= 0) mqtt_client.disconnect();
        Serial.println(F("Retrying connection…"));
        //Watchdog.reset();
        delay(1000);
    }
  }
    //TODO WD 
    aio_mqt.connected = true;
    Serial.println(F("Adafruit IO Connected!"));
    return aio_mqt.connected;
}

void mqtt_subscribe(void)
{
   // Setup MQTT subscription for onoff & slider feed.
  for (uint8_t indx= 0; indx < AIO_SUBS_NBR_OF; indx++ )
  {
    mqtt_client.subscribe(aio_subs[indx]);
  } 
  //mqtt_client.subscribe(&onoffbutton);
  //mqtt_client.subscribe(&slider);
}

void mqtt_read_subscriptions(void)
{
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  Serial.print(F("while subscription( "));
  while ((subscription = mqtt_client.readSubscription(5000))) 
  {
    // Check if its the onoff button feed
    for (uint8_t indx= 0; indx < AIO_SUBS_NBR_OF; indx++ )
    {
      if (subscription == aio_subs[indx]) 
      {
        Serial.print(aio_subs[indx]->topic);
        Serial.print(": ");
        Serial.println((char *)aio_subs[indx]->lastread);
      }
    }
  }
    Serial.println(F(" ...)"));
}

void mqtt_check_connection(void)
{
  // ping the server to keep the mqtt connection alive
  if(! mqtt_client.ping()) 
  {
    mqtt_client.disconnect();
  }
}

