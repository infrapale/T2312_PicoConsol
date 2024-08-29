#ifndef __MY_WIFI_H__
#define __MY_WIFI_H__

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       WIFI_SSID
#define WLAN_PASS       WIFI_PASS


void my_wifi_initialize(void);

bool my_wifi_connect(void);

bool my_wifi_is_connected(void);

#endif