#ifndef ESP_HELPER_H
#define ESP_HELPER_H

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <TelnetStream.h>
#include <WiFiUdp.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif
#ifdef ESP32
#include <ESPmDNS.h>
#include <WiFi.h>
#endif

#define LOCAL_CREDENTIALS_FILE
#ifdef LOCAL_CREDENTIALS_FILE
#include "WiFi_Credentials_Local.h"
#else
#include <WiFi_Credentials.h>
#endif

class espHelper {
public:
    espHelper(void);
    bool setup() { return setup(mySSID, myPASSWORD); };
    bool setup(const char* ssid, const char* password = NULL);

    bool setupOTA(const char* nameprefix) { return setupOTA(nameprefix, mySSID, myPASSWORD); };
    bool setupOTA(const char* nameprefix, const char* ssid, const char* password = NULL);

    void loop();

private:
};
#endif
