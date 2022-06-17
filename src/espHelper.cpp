#include "espHelper.h"
#include "ezButton.h"
// espHelper::espHelper() { }

ezButton button2(0);
bool espHelper::setup(const char* ssid, const char* password)
{
    uint32_t currentMillis = millis();
    uint32_t previousMillis = currentMillis;

    Serial.println(F("\n**********************\n********-WiFi-********\n**********************"));
    WiFi.begin(ssid, password);
    Serial.print(F("Connecting to "));
    Serial.println(ssid);

    uint8_t atempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        button2.loop();
        currentMillis = millis();
        if (currentMillis - previousMillis > 500) { // delay(500);
            if (atempts >= 20) {
                Serial.println(F("\nWiFi Failed to Connect!"));
                Serial.println(F(""));
                return false;
            }
            atempts++;
            Serial.print(F("."));
            previousMillis = currentMillis;
        } else if (button2.isReleased()) {
            Serial.println(F("\nWiFi User Canceled"));
            Serial.println(F(""));
            return false;
        }
    }
    Serial.println(F("\nWiFi connected!"));
    Serial.println(WiFi.localIP());
    Serial.println(F(""));
    return true;
}
bool espHelper::setupOTA(const char* ssid, const char* password)
{
    Serial.println(F("\n**********************\n******-WiFi&OTA-******\n**********************"));

    uint32_t currentMillis = millis();
    uint32_t previousMillis = currentMillis;
    const int maxlen = 40;
    char fullhostname[maxlen];
    uint8_t mac[6];
    WiFi.macAddress(mac);
    snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", hostName_, mac[3], mac[4], mac[5]);
    ArduinoOTA.setHostname(fullhostname);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    uint8_t atempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        button2.loop();
        currentMillis = millis();
        if (currentMillis - previousMillis > 500) {
            if (atempts >= 20) {
                Serial.println(F("\nWiFi Failed to Connect!"));
                Serial.println(F(""));
                return false;
            }
            Serial.print(F("."));
            atempts++;
            previousMillis = currentMillis;

        } else if (button2.isReleased()) {
            Serial.println(F("\nWiFi User Canceled"));
            Serial.println(F(""));
            return false;
        }
    }

    // while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    //     Serial.println("Connection Failed! Rebooting...");
    //     delay(5000);
    //     ESP.restart();
    // }

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
    });

    ArduinoOTA.begin();
    TELNET.begin();

    Serial.println("OTA Initialized");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    return true;
}

void espHelper::loop()
{
    ArduinoOTA.handle();

#ifdef TELNET_STREAM
    switch (TelnetStream.read()) {
    case 'R':
        TelnetStream.stop();
        delay(100);
        ESP.RESTART;
        break;
    case 'C':
        TelnetStream.println("bye bye");
        TelnetStream.flush();
        TelnetStream.stop();
        break;
    }

#endif
}
