#ifndef ESP_HELPER_H
#define ESP_HELPER_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <TelnetStream.h>
#include <WiFiUdp.h>
#include <WiFi_Credentials.h>

class espHelper {
public:
    espHelper(void);
    bool setup() { setup(mySSID, myPASSWORD); };
    bool setup(char* ssid, char* password = NULL);

    bool setupOTA(const char* nameprefix) { setupOTA(nameprefix, mySSID, myPASSWORD); };
    bool setupOTA(const char* nameprefix, char* ssid, char* password = NULL);

    void loop();

private:
};
#endif
