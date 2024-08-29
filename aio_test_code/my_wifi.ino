#include <WiFi.h>
#include "my_wifi.h"
#include "Arduino.h"

typedef struct 
{
    bool connected;
} my_wifi_st;

my_wifi_st my_wifi;
WiFiClient client;

bool my_wifi_is_connected(void)
{
  return my_wifi.connected;
}

void my_wifi_initialize(void)
{
  my_wifi.connected = false;
}

bool my_wifi_connect(void)
{
  my_wifi.connected = false;
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  my_wifi.connected = true;
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  return my_wifi.connected;
}
